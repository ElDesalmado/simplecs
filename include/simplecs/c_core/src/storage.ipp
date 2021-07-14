#pragma once

#include "simplecs/c_core/storage.hpp"

namespace eld
{
    namespace c_core
    {
    }

    namespace c_api
    {
        allocation_component_storage_error init_component_storage(
            component_storage_descriptor &outputDescriptor,
            const storage_params &inputParams)
        {
            // TODO: implement
        }

        release_component_storage_error release_component_storage(
            component_storage_descriptor &storageDescriptor)
        {
            // TODO: implement
        }

        allocate_component_error allocate_component(const entity_descriptor &entity,
                                                    const component_descriptor &componentDescriptor,
                                                    component_pointer *&pointer)
        {
            // TODO: implement
            return allocate_component_error::success;
        }

        allocate_component_error construct_component(const entity_descriptor &entity,
                                                     const component_descriptor &component,
                                                     component_pointer *&pointer,
                                                     tuple *args,
                                                     size_t argsSizeBytes)
        {
            // TODO: implement
            return allocate_component_error::success;
        }

        deallocate_component_error deallocate_component(const entity_descriptor &entity,
                                                        component_pointer *&pointer)
        {
            // TODO: implement
            return deallocate_component_error::success;
        }

        get_component_error get_component(const entity_descriptor &entity,
                                          const component_descriptor &componentDescriptor,
                                          component_pointer *&pointer)
        {
            // TODO: implement
            return get_component_error::success;
        }
    }   // namespace c_api

}   // namespace eld
