#pragma once

#include "simplecs/config.hpp"

#include "simplecs/generic/component_storage.h"
#include "simplecs/generic/registry.h"
#include "simplecs/generic/relational_table.h"

#include "simplecs/component_storage.hpp"
#include "simplecs/relational_table.hpp"

namespace simplecs
{
    namespace eld
    {
        template<typename ClassType>
        using component_storage =
            simplecs::generic::component_storage<component_storage_impl<ClassType>>;

        template<typename ClassType>
        eld::component_storage<ClassType> get_component_storage()
        {
            return simplecs::make_component_storage<component_storage_impl<ClassType>>();
        }

        using relational_table = simplecs::generic::relational_table<relational_table_impl>;

        inline relational_table get_relational_table()
        {
            return simplecs::make_relational_table<relational_table_impl>();
        }

    }   // namespace eld

}   // namespace simplecs

#ifdef SIMPLECS_HEADER_ONLY
#    include "simplecs/src/registry.ipp"
#endif