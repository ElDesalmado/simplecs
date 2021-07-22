#pragma once

#include "simplecs/c_api/c_core.hpp"
#include "simplecs/c_api/relational_table.hpp"

#include <iterator>

namespace simplecs
{
    namespace c_core
    {
        void relational_table::register_components(
            const relational_table::entity_handle_type &owningEntity,
            const relational_table::component_handle_type *array,
            size_t length,
            c_api::reg_error *)
        {
            using difference_type = std::iterator_traits<decltype(array)>::difference_type;

            // TODO: errors
            std::for_each(
                array,
                std::next(array, difference_type(length)),
                [&owningEntity, this](const component_handle_type &componentHandle)
                {
                  // insert if not found
                  componentToEntityMap_[componentHandle.typeDescriptor].template emplace(
                      owningEntity);
                  entityToComponentMap_[owningEntity] = componentHandle;
                });
        }

        void relational_table::unregister_components(const c_api::entity_descriptor &owningEntity,
                                                     const c_api::type_descriptor *array,
                                                     size_t length,
                                                     c_api::unreg_error *)
        {
            using difference_type = std::iterator_traits<decltype(array)>::difference_type;

            // TODO: errors
            std::for_each(array,
                          std::next(array, difference_type(length)),
                          [&owningEntity, this](const component_type &componentHandle)
                          {
                            // insert if not found
                            componentToEntityMap_[componentHandle].erase(owningEntity);
                            entityToComponentMap_.erase(owningEntity);
                          });
        }

        c_api::entity_selection relational_table::select_entities(
            const std::vector<component_type> &componentsToSelect)
        {
            const auto handle = c_api::handle{ selectionHandlePool_.next_available() };
            std::vector<entity_handle_type> selectedEntities =
                selector_(componentToEntityMap_, componentsToSelect);

            c_api::entity_selection selection{handle, selectedEntities.data(), selectedEntities.size()};
            selections_.emplace(std::make_pair(handle, std::move(selectedEntities)));

            return selection;
        }

        void relational_table::free_selection(const c_api::handle &handle)
        {
            selections_.erase(handle);
            selectionHandlePool_.free(handle.h);
        }

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