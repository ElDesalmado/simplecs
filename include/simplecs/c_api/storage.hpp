#pragma once

#include "simplecs/c_api/types.h"
#include "simplecs/config.hpp"

#include "simplecs/impl/id_pool.h"

#include <functional>
#include <limits>
#include <optional>
#include <tuple>
#include <unordered_map>
#include <vector>

template<>
struct std::hash<eld::c_api::handle>
{
    size_t operator()(const eld::c_api::handle &handle) const
    {
        return std::hash<decltype(handle.h)>{}(handle.h);
    }
};

namespace eld::c_core
{
    using component_id = size_t;

    struct component_traits
    {
        using handle_type = c_api::handle;
        using component_type = void;
        using component_reference_type = c_api::component_pointer;
    };

    class component_storage_impl
    {
    public:
        using component_traits = c_core::component_traits;
        using handle_type = typename component_traits::handle_type;
        using component_reference_type = typename component_traits::component_reference_type;

        using component_size = decltype(std::declval<c_api::type_descriptor>().typeSize);

        component_storage_impl(c_api::storage_params storageParams,
                               c_api::type_descriptor typeDescriptor)
          : typeDescriptor_(typeDescriptor),
            deallocate_(component_storage_impl::make_destructor(storageParams.pDestroy,
                                                                storageParams.pCallable))
        {
            // TODO: checks
        }

        std::vector<component_reference_type> emplace_components(int num)
        {
            assert(num >= 0 && "Component num must not be negative!");
            std::vector<component_reference_type> out;
            out.reserve(num);

            while (num--)
            {
                const auto handle = handlePool_.next_available();
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
        auto remove_components(const Container<handle_type> &handles)
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

        component_reference_type get_component(const handle_type &componentHandle)
        {
            auto found = map_.find(componentHandle);
            if (found == map_.cend())
            {
                throw std::invalid_argument("component_storage_impl: invalid component handle");
            }

            return { found->second, c_api::component_descriptor{ found->first, typeDescriptor_ } };
        }

        // TODO: const reference type?
        component_reference_type get_component(const handle_type &componentHandle) const
        {
            auto found = map_.find(componentHandle);
            if (found == map_.cend())
            {
                throw std::invalid_argument("component_storage_impl: invalid component handle");
            }

            return { found->second,   //
                     c_api::component_descriptor{ found->first, typeDescriptor_ } };
        }

    private:
        static c_api::object *allocate(component_size componentSize)
        {
            return static_cast<c_api::object *>(operator new(componentSize));
        }
        static std::function<void(c_api::object *pObject)> make_destructor(
            void (*pFunction)(c_api::callable *, c_api::object *),
            c_api::callable *pCallable)
        {
            assert(pFunction && "Function is nullptr!");
            return [=](c_api::object *pObject) { pFunction(pCallable, pObject); };
        }

        void deallocate(c_api::object *pObject)
        {
            assert(deallocate_ && "Invalid deallocate_");
            deallocate_(pObject);
        }

    private:
        c_api::type_descriptor typeDescriptor_;
        std::unordered_map<handle_type, c_api::object *> map_;
        eld::detail::id_pool<handle_type> handlePool_;
        std::function<void(c_api::object *pObject)> deallocate_;
    };

}   // namespace eld::c_core

#ifdef SIMPLECS_HEADER_ONLY
#    include "simplecs/c_api/src/storage.ipp"
#endif