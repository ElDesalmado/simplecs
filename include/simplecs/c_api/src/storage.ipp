#pragma once

#include "simplecs/c_api/storage.hpp"
#include "simplecs/generic/component_storage.h"

#include "simplecs/impl/id_pool.h"

#include <algorithm>
#include <cassert>
#include <new>
#include <optional>

template<>
struct std::hash<eld::c_api::type_descriptor>
{
    size_t operator()(const eld::c_api::type_descriptor &d) const
    {
        return std::hash<size_t>{}(d.typeId);
    }
};

namespace eld
{
    namespace c_core
    {
        using c_component_storage = eld::generic::component_storage<component_storage_impl>;

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
                        eld::make_component_storage<c_core::component_storage_impl>(
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

                void clear() { componentStorages_.clear(); }

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
                eld::detail::id_pool<size_t> idPool_;
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
