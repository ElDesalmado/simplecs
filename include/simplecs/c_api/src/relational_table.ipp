#pragma once

#include "simplecs/c_api/c_core.hpp"
#include "simplecs/c_api/relational_table.hpp"
#include "simplecs/generic/relational_table.h"

#include <iterator>

namespace simplecs
{


    namespace c_core
    {
        static auto relationalTable = relational_table();
    }   // namespace c_core

    namespace c_api
    {
        void register_components(const entity_descriptor &owningEntity,
                                 const component_descriptor *array,
                                 size_t length,
                                 reg_error *results,
                                 const policy *)
        {
            if (!array ||   //
                !length)
                return;

            c_core::relationalTable.register_components(owningEntity, array, length, results);
        }

        void unregister_components(const entity_descriptor &owningEntity,
                                   const type_descriptor *array,
                                   size_t length,
                                   unreg_error *results,
                                   const policy *)
        {
            if (!array ||   //
                !length)
                return;

            c_core::relationalTable.unregister_components(owningEntity, array, length, results);
        }

        void select_entities_by_components(const type_descriptor *array,
                                           size_t length,
                                           entity_selection &result,
                                           const policy *)
        {
            if (!array ||   //
                !length)
                return;

            // TODO: errors
            using difference_type = std::iterator_traits<decltype(array)>::difference_type;
            auto componentTypes = std::vector(array, std::next(array, difference_type(length)));

            result = c_core::relationalTable.select_entities(componentTypes);
        }

        void free_entity_selection(entity_selection & selection)
        {
            c_core::relationalTable.free_selection(selection.handle);
            selection = {};
        }

        void get_components_by_selection(const entity_selection &/*entitySelection*/,
                                         const type_descriptor &/*typeDescriptor*/,
                                         component_descriptor */*array*/,
                                         size_t /*length*/,
                                         selection_error */*results*/,
                                         const policy *)
        {

        }

    }   // namespace c_api
}   // namespace simplecs