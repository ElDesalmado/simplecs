#pragma once

#include "simplecs/c_api/types.h"
#include "simplecs/config.hpp"

#include <cstddef>
#include <cstdint>

// TODO: C-compatible types

namespace eld::c_api
{
    /**
     * Typedef equivalent for void*. Referenced object is expected to store a tuple of parameters.
     */
    struct tuple;

    /**
     * Typedef for a callable object. Equivalent to void*
     */
    struct callable;

    /**
     * Typedef for a void* object.
     */
    struct object;

    /**
     * Typedef for a policy to extend behavior. Equivalent to void*.
     */
    struct policy;

    struct handle;
    struct type_descriptor;
    struct entity_descriptor;
    struct component_descriptor;
    struct entity_selection;
    struct component_pointer;

    struct storage_params
    {
        size_t componentSize{};
        void (*pDestroy)(c_api::callable *, c_api::object *){};
        c_api::callable *pCallable{};
    };

    enum error_result
    {
        success = 0,
        invalid_entity_descriptor
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

    enum class entity_allocation_error : uint8_t
    {
        success = error_result::success,
        invalid_entity_descriptor = error_result::invalid_entity_descriptor
    };

    struct component_storage_descriptor
    {
        component_descriptor componentDescriptor{};
        size_t componentSize{};
    };

    enum class allocation_component_storage_error : uint8_t
    {
        success = 0,
        invalid_component_size
    };

    enum class release_component_storage_error : uint8_t
    {
        success = 0,
        invalid_component_descriptor
    };

    enum class allocate_component_error : uint8_t
    {
        success = 0,
        already_exists,
        invalid_constructor,
        invalid_component_descriptor,
        invalid_entity
    };

    enum class deallocate_component_error : uint8_t
    {
        success = 0,
        invalid_component_descriptor,
        invalid_entity,
    };

    enum class get_component_error : uint8_t
    {
        success = 0,
        invalid_entity,
        invalid_component_descriptor,
    };

    enum class selection_error : uint8_t
    {
        success = error_result::success
    };

    extern "C"
    {
        /**
         * Functionality:
         * 1. Relational table.
         *      Description: Maps entities and components.
         *      - select entities with set of given components
         *      - get components handle by component id from the given entity
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

        // TODO: finite specification
        /**
         * Associates an array of components to the given entity. All the components must have
         * unique IDs. One entity can be associated with only one component, though distinct
         * components may be shared between entities. If a component with new handle is passed,
         * reassociates the entity with a new component instance.
         * @param entityDescriptor Entity descriptor to associate components with.
         * @param array Array of components descriptors. Owned by the caller.
         * @param length Length of \p array and \p results if \p results is not NULL.
         * @param results Array of results for each component. May be NULL.
         * @param policy Reserved parameter for customization. NULL by default.
         */
        SIMPLECS_DECL void register_components(const entity_descriptor &entityDescriptor,
                                               const component_descriptor *array,
                                               size_t length,
                                               reg_error *results,
                                               const policy *policy);

        /**
         * Removes association of an array of components to the given entity.
         * @param entityDescriptor Entity descriptor to remove association from
         * @param array Array of components' type descriptors.
         * @param length Length of \p array and \p results if \p results is not NULL.
         * @param results Array of results for each component. May be NULL.
         * @param policy Reserved parameter for customization. NULL by default.
         */
        SIMPLECS_DECL void unregister_components(const entity_descriptor &entityDescriptor,
                                                 const type_descriptor *array,
                                                 size_t length,
                                                 unreg_error *results,
                                                 const policy *policy);

        /**
         * Select all the entities associated with a given set of components' type descriptors
         * @param array User-allocated array of components' type descriptors. Owned by caller.
         * @param length Length of \p array
         * @param result Output selection. Data is owned by Core. Must be freed after.
         * @param policy Reserved parameter for customization. NULL by default.
         */
        SIMPLECS_DECL void select_entities_by_components(const type_descriptor *array,
                                                         size_t length,
                                                         entity_selection &result,
                                                         const policy *policy);

        /**
         * Free selection.
         */
        SIMPLECS_DECL void free_entity_selection(entity_selection &);

        /**
         * Get components' descriptors for a given entities selection.
         * @param entitySelection Selection of entity descriptors. May be owned by the caller or
         * acquired from \ref select_entities_by_components
         * @param typeDescriptor Component's type descriptor.
         * @param array User-allocated Array of components' descriptors. Owned by the caller.
         * @param length Length of \p array and \p results if \p results is not NULL. Should be
         * equal to the length of \p entitySelection. If less, only \p length descriptors will be
         * returned.
         * @param results
         * @param policy Reserved parameter for customization. NULL by default.
         */
        SIMPLECS_DECL void get_components_by_selection(const entity_selection &entitySelection,
                                                       const type_descriptor &typeDescriptor,
                                                       component_descriptor *array,
                                                       size_t length,
                                                       selection_error *results,
                                                       const policy *policy);

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
         * Initialize new component storage.
         * @param storageParams
         * @param outputDescriptor
         * @return
         */
        SIMPLECS_DECL allocation_component_storage_error
            init_component_storage(const storage_params &storageParams,
                                   type_descriptor &outputDescriptor,
                                   const policy *);

        SIMPLECS_DECL release_component_storage_error
            release_component_storage(type_descriptor &storageDescriptor);

        // TODO: clarify. Can a component exist without an entity?
        SIMPLECS_DECL allocate_component_error
            allocate_component(const type_descriptor &typeDescriptor,
                               component_pointer &pointer);

        SIMPLECS_DECL deallocate_component_error
            deallocate_component(const component_descriptor &componentDescriptor);

        SIMPLECS_DECL get_component_error
            get_component(const component_descriptor & componentDescriptor,
                          component_pointer &pointer);

        SIMPLECS_DECL void release_context();
    }

}   // namespace eld::c_api

#ifdef SIMPLECS_HEADER_ONLY
#    include "simplecs/c_api/src/relational.ipp"
#    include "simplecs/c_api/src/storage.ipp"
#endif