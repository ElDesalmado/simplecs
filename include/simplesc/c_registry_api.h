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

    struct component_id
    {
        size_t id;
    };

    struct component_pointer
    {
        component_id component_id;
        void *p_object;
        size_t component_size;
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

    enum class reg_error : uint8_t
    {
        success = 0,
        component_already_registered
    };

    enum class unreg_error : uint8_t
    {
        success = 0,
        component_not_found,
        component_not_registered
    };

    constexpr size_t invalid_id = std::numeric_limits<size_t>::max();

    enum class entity_allocation_error : uint8_t 
    {
        success = 0
    };

    struct component_storage_descr
    {
        component_id component;
        size_t component_size;

    };

    struct storage_params
    {

    };

    extern "C"
    {
        /**
         * Functionality:
         * 1. Lookup table.
         *      Description: Maps entities and components.
         *      - select entities with set of given components
         *      - enumerate components for given entity
         *      - add components to entities
         *      - remove components from entities
         *      - (possible) caches entity selections
         * 2. Component storage:
         *      Description: Manages memory allocation for distinct components.
         *      - allocate storage
         *      - allocate RAII storage (must provide Component's in-place constructor and in-place destructor)
         *      - allocate component for entity
         *      - destroy component component for entity
         *      - deallocate storage
         *      - get pointer to entity's component
         * 3. Entity storage:
         *      Description: Keeps track of entities.
         *      - create entity
         *      - destroy entity
         * 4. Context:
         *      - release. Clears all the resources. Destroys components within the RAII storage.
         */

        SIMPLECS_DECL void register_components(const entity &,
                                               const component_id *array,
                                               size_t length,
                                               reg_error *results);

        SIMPLECS_DECL void unregister_components(const entity &,
                                                 const component_id *array,
                                                 size_t length,
                                                 unreg_error *results);

        SIMPLECS_DECL void select_entities_by_components(const component_id *array,
                                                         size_t length,
                                                         entity_selection &result);
        SIMPLECS_DECL void free_entity_selection(entity_selection &);

        SIMPLECS_DECL void allocate_entities(entity *& array,
                                             size_t length,
                                             entity_allocation_error *&results);

        SIMPLECS_DECL void deallocate_entities(entity *& array,
                                               size_t length,
                                               entity_allocation_error *&results);

        SIMPLECS_DECL void init_component_storage();
        SIMPLECS_DECL void deinit_component_storage();
        
    }

    constexpr inline bool operator<(const entity &lhs, const entity &rhs)
    {
        return lhs.id < rhs.id;
    }

    constexpr inline bool operator==(const entity &lhs, const entity &rhs)
    {
        return !(lhs < rhs) && !(rhs < lhs);
    }

    constexpr inline bool operator<(const component_id &lhs, const component_id &rhs)
    {
        return lhs.id < rhs.id;
    }

    constexpr inline bool operator==(const component_id &lhs, const component_id &rhs)
    {
        return !(lhs < rhs) && !(rhs < lhs);
    }

}   // namespace eld::c_api

#ifdef SIMPLECS_HEADER_ONLY
#    include "simplesc/src/c_registry.ipp"
#endif