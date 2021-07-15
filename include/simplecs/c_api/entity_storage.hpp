#pragma once

#include "simplecs/c_api/c_core.hpp"
#include "simplecs/config.hpp"
#include "simplecs/impl/id_pool.h"

namespace eld::c_core
{
    class entities
    {
    public:
        static entities& instance();
        static void release();

        c_api::entity_allocation_error allocate(c_api::entity_descriptor &descriptor);

        void allocate(c_api::entity_descriptor *&array, size_t length,
                      c_api::entity_allocation_error *&results);

        c_api::entity_allocation_error release(c_api::entity_descriptor &descriptor);

        void release(c_api::entity_descriptor *&array, size_t length,
                     c_api::entity_allocation_error *&results);

    private:
        using entity_id = size_t;
    private:
        static entities instance_;

        eld::detail::id_pool<entity_id> idPool_;
    };

}

#ifdef SIMPLECS_HEADER_ONLY
#    include "simplecs/c_api/src/entity_storage.ipp"
#endif