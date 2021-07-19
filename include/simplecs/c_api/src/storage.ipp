#pragma once

#include "simplecs/c_api/storage.hpp"
#include "simplecs/generic/component_storage.h"

#include "simplecs/impl/id_pool.h"

#include <algorithm>
#include <cassert>
#include <new>

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
                template<typename... ArgsT>
                c_api::type_descriptor emplace(ArgsT &&...args)
                {
                    const auto id = idPool_.next_available();

                    auto res = componentStorages_.template emplace(
                        std::make_pair(id,
                                       eld::make_component_storage<c_core::component_storage_impl>(
                                           std::forward<ArgsT>(args)...)));

                    assert(res.second && "Failed to emplace new component storage!");
                    return res.first->first;
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
            component_storage_descriptor & /*outputDescriptor*/,
            const storage_params & /*inputParams*/)
        {
            return {};
        }

        release_component_storage_error release_component_storage(
            component_storage_descriptor & /*storageDescriptor*/)
        {
            return {};
        }

        allocate_component_error allocate_component(
            const entity_descriptor & /*entity*/,
            const component_descriptor & /*componentDescriptor*/,
            component_pointer & /*pointer*/)
        {
            return {};
        }

        deallocate_component_error deallocate_component(const entity_descriptor & /*entity*/,
                                                        component_pointer & /*pointer*/)
        {
            return {};
        }

        get_component_error get_component(const entity_descriptor & /*entity*/,
                                          const component_descriptor & /*componentDescriptor*/,
                                          component_pointer & /*pointer*/)
        {
            return {};
        }
    }   // namespace c_api

}   // namespace eld
