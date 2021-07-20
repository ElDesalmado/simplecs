#pragma once

#include "simplecs/generic/component_storage.h"
#include "simplecs/c_api/component_storage.hpp"
#include "simplecs/c_api/c_core.hpp"

#include "simplecs/impl/id_pool.h"

#include <algorithm>
#include <cassert>
#include <new>
#include <optional>
#include <stdexcept>

template<>
struct std::hash<simplecs::c_api::type_descriptor>
{
    size_t operator()(const simplecs::c_api::type_descriptor &d) const
    {
        return std::hash<size_t>{}(d.typeId);
    }
};

namespace simplecs
{
    namespace c_core
    {
        component_storage_impl::component_storage_impl(c_api::storage_params storageParams,
                                                       c_api::type_descriptor typeDescriptor) noexcept
            : typeDescriptor_(typeDescriptor),
              deallocate_(component_storage_impl::make_destructor(storageParams.pDestroy,
                                                                  storageParams.pCallable))
        {
            // TODO: checks
        }

        std::vector<component_storage_impl::component_reference_type> component_storage_impl::emplace_components(int num)
        {
            assert(num >= 0 && "Component num must not be negative!");
            std::vector<component_reference_type> out;
            out.reserve(num);

            while (num--)
            {
                const handle_type handle{ handlePool_.next_available() };
                assert(map_.find(handle) == map_.cend() && "Handle already registered!");

                const auto allocated = component_storage_impl::allocate(typeDescriptor_.typeSize);
                auto emplaced = map_.emplace(std::make_pair(handle, allocated));
                assert(emplaced.second && "Failed to emplace new component!");

                const auto componentDescriptor =
                    c_api::component_descriptor{ emplaced.first->first, typeDescriptor_ };
                out.emplace_back(component_reference_type{ allocated, componentDescriptor });
            }

            return out;
        }

        template<template<typename> class Container>
        auto component_storage_impl::remove_components(const Container<handle_type> &handles)
        {
            using namespace c_api;
            std::vector<deallocate_component_error> errors;
            errors.reserve(handles.size());

            std::for_each(handles.cbegin(),
                          handles.cend(),
                          [this, &errors](const handle_type &handle)
                          {
                            auto found = map_.find(handle);
                            if (found == map_.cend())
                            {
                                errors.push_back(
                                    deallocate_component_error::invalid_component_descriptor);
                                return;
                            }
                            this->deallocate(found->second);
                            map_.erase(found);
                          });
        }

        component_storage_impl::component_reference_type component_storage_impl::get_component(
            const component_storage_impl::handle_type &componentHandle)
        {
            auto found = map_.find(componentHandle);
            if (found == map_.cend())
            {
                throw std::invalid_argument("component_storage_impl: invalid component handle");
            }

            return { found->second, c_api::component_descriptor{ found->first, typeDescriptor_ } };
        }

        component_storage_impl::component_reference_type component_storage_impl::get_component(
            const component_storage_impl::handle_type &componentHandle) const
        {
            auto found = map_.find(componentHandle);
            if (found == map_.cend())
            {
                throw std::invalid_argument("component_storage_impl: invalid component handle");
            }

            return { found->second,   //
                     c_api::component_descriptor{ found->first, typeDescriptor_ } };
        }

        component_storage_impl::~component_storage_impl()
        {
            std::for_each(map_.begin(),
                          map_.end(),
                          [this](const auto &pair) { this->deallocate(pair.second); });
        }

        std::function<void(c_api::object *pObject)> component_storage_impl::make_destructor(
            void (*pFunction)(c_api::callable *, c_api::object *),
            c_api::callable *pCallable)
        {
            if (!pFunction)
                return [](c_api::object *pObject) { operator delete(pObject); };

            return [=](c_api::object *pObject)
            {
              pFunction(pCallable, pObject);
              operator delete(pObject);
            };
        }

        c_api::object *component_storage_impl::allocate(
            component_storage_impl::component_size componentSize)
        {
            return static_cast<c_api::object *>(operator new(componentSize));
        }

        void component_storage_impl::deallocate(c_api::object *pObject)
        {
            assert(deallocate_ && "Invalid deallocate_");
            deallocate_(pObject);
        }

        using c_component_storage = simplecs::generic::component_storage<component_storage_impl>;

        namespace detail
        {
            class ComponentStorages
            {
            public:
                c_api::type_descriptor emplace(const c_api::storage_params &storageParams)
                {
                    const auto id = idPool_.next_available();

                    auto res = componentStorages_.template emplace(std::make_pair(
                        c_api::type_descriptor{ id, storageParams.componentSize },
                        simplecs::make_component_storage<c_core::component_storage_impl>(
                            storageParams,
                            c_api::type_descriptor{ id, storageParams.componentSize })));

                    assert(res.second && "Failed to emplace new component storage!");
                    return res.first->first;
                }

                void release(const c_api::type_descriptor &typeDescriptor)
                {
                    // TODO: handle errors
                    componentStorages_.erase(typeDescriptor);
                }

                void clear()
                {
                    idPool_ = {};
                    componentStorages_.clear();
                }

                std::optional<std::reference_wrapper<c_component_storage>> get(
                    const c_api::type_descriptor &descriptor)
                {
                    auto found = componentStorages_.find(descriptor);
                    if (found == componentStorages_.cend())
                        return std::nullopt;

                    return found->second;
                }

            private:
                std::unordered_map<c_api::type_descriptor, c_component_storage> componentStorages_;
                simplecs::detail::id_pool<size_t> idPool_;
            };
        }   // namespace detail

        static detail::ComponentStorages componentStorages;

    }   // namespace c_core

    namespace c_api
    {
        allocation_component_storage_error init_component_storage(
            const storage_params &storageParams,
            type_descriptor &outputDescriptor,
            const policy *)
        {
            // TODO: try-catch
            outputDescriptor = c_core::componentStorages.emplace(storageParams);
            return allocation_component_storage_error::success;
        }

        release_component_storage_error release_component_storage(
            type_descriptor &storageDescriptor)
        {
            // TODO: try-catch?
            c_core::componentStorages.release(storageDescriptor);
            storageDescriptor = {};
            return release_component_storage_error::success;
        }

        allocate_component_error allocate_component(const type_descriptor &typeDescriptor,
                                                    component_pointer &pointer)
        {
            auto optionalStorage = c_core::componentStorages.get(typeDescriptor);
            if (!optionalStorage)
                return allocate_component_error::invalid_component_descriptor;

            c_core::c_component_storage &storage = *optionalStorage;

            pointer = storage.emplace(1)[0];
            return allocate_component_error::success;
        }

        deallocate_component_error deallocate_component(const component_descriptor &componentDescriptor)
        {
            auto optionalStorage = c_core::componentStorages.get(componentDescriptor.typeDescriptor);
            if (!optionalStorage)
                return deallocate_component_error::invalid_component_descriptor;

            c_core::c_component_storage &storage = *optionalStorage;

            // TODO: errors
            storage.remove(std::vector{componentDescriptor.handle});

            return deallocate_component_error::success;
        }

        get_component_error get_component(const component_descriptor & componentDescriptor,
                                          component_pointer &pointer)
        {
            auto optionalStorage = c_core::componentStorages.get(componentDescriptor.typeDescriptor);
            if (!optionalStorage)
                return get_component_error::invalid_component_descriptor;

            c_core::c_component_storage &storage = *optionalStorage;

            // TODO: errors

            pointer = storage.get(componentDescriptor.handle);

            return get_component_error::success;
        }
    }   // namespace c_api

}   // namespace eld
