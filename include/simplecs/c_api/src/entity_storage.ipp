#pragma once

#include "simplecs/c_api/entity_storage.hpp"

namespace eld
{
    namespace c_core
    {
    }

    namespace c_api
    {
        void allocate_entities(entity_descriptor *&/*array*/,
                               size_t /*length*/,
                               entity_allocation_error *&/*results*/)
        {
            // TODO: implement
        }

        void deallocate_entities(entity_descriptor *&/*array*/,
                                 size_t /*length*/,
                                 entity_allocation_error *&/*results*/)
        {
            // TODO: implement
        }
    }
}
