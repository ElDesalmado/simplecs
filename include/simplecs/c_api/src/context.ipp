#pragma once

#include "simplecs/c_api/context.hpp"

#include "simplecs/c_api/component_storage.hpp"
#include "simplecs/c_api/entity_storage.hpp"
//#include "simplecs/c_api/relational.hpp"

namespace simplecs::c_api
{
    void release_context()
    {
        //        eld::c_core::relational_table::release();
        //        eld::c_core::selections::release();
        //        eld::c_core::entities::release();
        //        eld::c_core::storages::release();

        simplecs::c_core::componentStorages.clear();
    }
}   // namespace eld::c_api
