#pragma once

#include "simplecs/c_api/entity_storage.hpp"

namespace eld
{
    namespace c_core
    {
        entities entities::instance_{};

        entities &entities::instance() { return instance_; }

        c_api::entity_allocation_error entities::allocate(c_api::entity_descriptor &/*descriptor*/)
        {
//            descriptor.handle = idPool_.next_available();
            return c_api::entity_allocation_error::success;
        }

        void entities::allocate(c_api::entity_descriptor *&array,
                                size_t length,
                                c_api::entity_allocation_error *&results)
        {
            for (size_t i = 0; i != length; ++i)
            {
                const auto res = allocate(array[i]);
                if (results)
                    results[i] = res;
            }
        }

        c_api::entity_allocation_error entities::release(c_api::entity_descriptor &/*descriptor*/)
        {
//            if (idPool_.is_free(descriptor.id))
//                return c_api::entity_allocation_error::invalid_entity_descriptor;
//
//            idPool_.free(descriptor.id);
//            descriptor = {};

            return c_api::entity_allocation_error::success;
        }

        void entities::release(c_api::entity_descriptor *&array,
                               size_t length,
                               c_api::entity_allocation_error *&results)
        {
            for (size_t i = 0; i != length; ++i)
            {
                const auto res = release(array[i]);
                if (results)
                    results[i] = res;
            }
        }

        void entities::release() { entities::instance_ = {}; }
    }   // namespace c_core

    namespace c_api
    {
        void allocate_entities(entity_descriptor *&/*array*/,
                               size_t /*length*/,
                               entity_allocation_error *&/*results*/)
        {
//            c_core::entities::instance().allocate(array, length, results);
        }

        void deallocate_entities(entity_descriptor *&/*array*/,
                                 size_t /*length*/,
                                 entity_allocation_error *&/*results*/)
        {
//            c_core::entities::instance().release(array, length, results);
        }
    }   // namespace c_api
}   // namespace eld
