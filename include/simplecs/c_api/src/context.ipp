#pragma once

#include "simplecs/c_api/context.hpp"

#include "simplecs/c_api/relational.hpp"
#include "simplecs/c_api/storage.hpp"
#include "simplecs/c_api/entity_storage.hpp"

namespace eld::c_api
{
    void release_context()
    {
        eld::c_core::relational_table::release();
        eld::c_core::selections::release();
        eld::c_core::entities::release();
        eld::c_core::storages::release();
    }
}

