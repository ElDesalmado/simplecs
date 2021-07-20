#pragma once

#include "simplecs/c_api/types.h"
#include "simplecs/config.hpp"

#include <vector>

namespace simplecs::eld
{
    /**
     * C++ implementation for relational_table using C API.
     */
    class relational_table_impl
    {
    public:
        using entity_handle_type = c_api::entity_descriptor;
        using component_descriptor_type = c_api::type_descriptor;
        using component_handle_type = c_api::component_descriptor;

        struct traits
        {
            using entity_handle_type = entity_handle_type;
            using component_descriptor_type = component_descriptor_type;
            using component_handle_type = component_handle_type;
        };

        template<template<typename> typename ContainerT>
        /*constexpr*/ auto select_entities(
            const ContainerT<component_descriptor_type> &includeComponents,
            const ContainerT<component_descriptor_type> & /*excludeComponents*/)
            -> std::vector<entity_handle_type>;

        template<template<typename> typename ContainerT>
        std::vector<c_api::reg_error> register_components(const entity_handle_type &entityDescriptor,
                                 const ContainerT<component_handle_type> &componentHandles);

        template<template<typename> typename ContainerT>
        std::vector<c_api::reg_error> unregister_components(const entity_handle_type &entityDescriptor,
                                 const ContainerT<component_descriptor_type> &componentDescriptors);

    private:
    };

}   // namespace simplecs::eld

#ifdef SIMPLECS_HEADER_ONLY
#    include "simplecs/src/relational_table.ipp"
#endif