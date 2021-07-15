#pragma once

#include "simplecs/c_api/c_core.hpp"

namespace eld
{

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


    }   // namespace c_api

}   // namespace eld