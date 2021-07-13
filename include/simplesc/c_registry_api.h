#pragma once

#include "simplesc/config.hpp"

#include <cstddef>
#include <cstdint>
#include <limits>
#include <numeric>

namespace eld::c_api
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

    enum class reg_result : uint8_t
    {
        success = 0,
        component_already_registered
    };

    enum class unreg_result : uint8_t
    {
        success = 0,
        component_not_found,
        component_not_registered
    };

    constexpr size_t invalid_id = std::numeric_limits<size_t>::max();

    extern "C"
    {
        SIMPLESC_DECL void register_components(const entity &,
                                               const component *array,
                                               size_t length,
                                               reg_result *results);

        SIMPLESC_DECL void unregister_components(const entity &,
                                                 const component *array,
                                                 size_t length,
                                                 unreg_result *results);

        SIMPLESC_DECL void select_entities_by_components(const component *array,
                                                         size_t length,
                                                         entity_selection &result);
        SIMPLESC_DECL void free_entity_selection(entity_selection &);
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