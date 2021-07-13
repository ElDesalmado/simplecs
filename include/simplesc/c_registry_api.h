#pragma once

#include "simplesc/config.hpp"

#include <cstddef>
#include <cstdint>

namespace eld::c_api
{
    extern "C"
    {
        /**
         * Handle type for Entity
         */
        struct entity
        {
            size_t id;
        };

        /**
         * Handle type for Component
         */
        struct component
        {
            size_t id;
        };

        /**
         * Selection result. Owned by core.
         */
        struct entity_selection
        {
            size_t handle;
            const entity *array;
            size_t length;
        };



        SIMPLESC_DECL void select_entities_by_components(const component *array,
                                                         size_t length,
                                                         entity_selection &result);
        SIMPLESC_DECL void free_entity_selection(const entity_selection &);

    }

    constexpr inline bool operator<(const entity &lhs, const entity &rhs)
    {
        return lhs.id < rhs.id;
    }

    constexpr inline bool operator==(const entity &lhs, const entity &rhs)
    {
        return !(lhs < rhs) && !(rhs < lhs);
    }

    constexpr inline bool operator<(const component &lhs, const component &rhs)
    {
        return lhs.id < rhs.id;
    }

    constexpr inline bool operator==(const component &lhs, const component &rhs)
    {
        return !(lhs < rhs) && !(rhs < lhs);
    }

}   // namespace eld::c_api

#ifdef SIMPLESC_HEADER_ONLY
#    include "simplesc/src/c_registry.ipp"
#endif