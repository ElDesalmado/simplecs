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
    struct entity_descriptor
    {
        size_t id;
    };

    struct component_descriptor
    {
        size_t id;
    };

    /**
     * Selection result. Owned by core.
     */
    struct entity_selection
    {
        size_t handle;
        const entity_descriptor *array;
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

    struct component_storage_descriptor
    {
        component_descriptor component_descriptor;
        size_t component_size;
    };

    struct tuple;

    struct storage_params
    {
        size_t component_size;
        void (*p_in_place_construct)(void *pAllocatedMemory,
                                     size_t allocatedSize,
                                     const tuple &args,
                                     size_t argsSizeBytes);

        void (*p_in_place_destroy)(void *pObject, size_t objectSize);
        void *p_constructor_callable;
        void *p_destructor_callable;
    };

    struct component_pointer
    {
        component_descriptor component_descriptor;
        void *p_object;
        size_t component_size;
    };

    enum class allocation_component_storage_error : uint8_t
    {
        success = 0
    };

    enum class release_component_storage_error : uint8_t
    {
        success = 0
    };

    enum class allocate_component_error : uint8_t
    {
        success = 0
    };

    enum class deallocate_component_error : uint8_t
    {
        success = 0
    };

    enum class get_component_error : uint8_t
    {
        success = 0
    };

    extern "C"
    {
        /**
         * Functionality:
         * 1. Relational table.
         *      Description: Maps entities and components.
         *      - select entities with set of given components
         *      - enumerate components for given entity
         *      - add components to entities
         *      - remove components from entities
         *      - (possible) caches entity selections
         * 2. Component storage:
         *      Description: Manages memory allocation for distinct components.
         *      - allocate storage
         *      - allocate RAII storage (must provide Component's in-place constructor and in-place
         * destructor)
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

        SIMPLECS_DECL void register_components(const entity_descriptor &,
                                               const component_descriptor *array,
                                               size_t length,
                                               reg_error *results);

        SIMPLECS_DECL void unregister_components(const entity_descriptor &,
                                                 const component_descriptor *array,
                                                 size_t length,
                                                 unreg_error *results);

        SIMPLECS_DECL void select_entities_by_components(const component_descriptor *array,
                                                         size_t length,
                                                         entity_selection &result);
        SIMPLECS_DECL void free_entity_selection(entity_selection &);

        /**
         * Allocates entities within the Entity Storage
         * @param array input array of entity descriptors.
         * @param length Length of the input array
         * @param results Array of errors.
         */
        SIMPLECS_DECL void allocate_entities(entity_descriptor *&array,
                                             size_t length,
                                             entity_allocation_error *&results);

        /**
         * Deallocates entities within the Entity Storage
         * @param array input array of entity descriptors.
         * @param length Length of the input array
         * @param results Array of errors.
         */
        SIMPLECS_DECL void deallocate_entities(entity_descriptor *&array,
                                               size_t length,
                                               entity_allocation_error *&results);

        /**
         * Initialize a storage for a component. Assigns new id for the component.
         * @param outputDescriptor Component storage descriptor for .
         * @param inputParams
         */
        SIMPLECS_DECL allocation_component_storage_error
            init_component_storage(component_storage_descriptor &outputDescriptor,
                                   const storage_params &inputParams);

        SIMPLECS_DECL release_component_storage_error
            release_component_storage(component_storage_descriptor &storageDescriptor);

        SIMPLECS_DECL allocate_component_error
            allocate_component(const entity_descriptor &entity,
                               const component_descriptor &componentDescriptor,
                               component_pointer *&pointer);

        SIMPLECS_DECL allocate_component_error
            construct_component(const entity_descriptor &entity,
                                const component_descriptor &component,
                                component_pointer *&pointer,
                                tuple *args,
                                size_t argsSizeBytes);

        SIMPLECS_DECL deallocate_component_error
            deallocate_component(const entity_descriptor &entity, component_pointer *&pointer);

        SIMPLECS_DECL get_component_error get_component(const entity_descriptor &entity,
                                                        const component_descriptor& componentDescriptor,
                                                        component_pointer*& pointer);


    }

    constexpr inline bool operator<(const entity_descriptor &lhs, const entity_descriptor &rhs)
    {
        return lhs.id < rhs.id;
    }

    constexpr inline bool operator==(const entity_descriptor &lhs, const entity_descriptor &rhs)
    {
        return !(lhs < rhs) && !(rhs < lhs);
    }

    constexpr inline bool operator<(const component_descriptor &lhs,
                                    const component_descriptor &rhs)
    {
        return lhs.id < rhs.id;
    }

    constexpr inline bool operator==(const component_descriptor &lhs,
                                     const component_descriptor &rhs)
    {
        return !(lhs < rhs) && !(rhs < lhs);
    }

}   // namespace eld::c_api

#ifdef SIMPLECS_HEADER_ONLY
#    include "simplesc/src/c_registry.ipp"
#endif