#pragma once

#include "simplecs/c_api/relational.hpp"
#include "simplecs/generic/relational_table.h"
#include "simplecs/impl/relational_table.h"

#include <cassert>
#include <iterator>
#include <optional>
#include <simplecs/c_api/c_core.hpp>

namespace eld
{
    namespace c_core
    {
        entity_selection::entity_selection(std::vector<c_api::entity_descriptor> &&selection)
          : selection_(std::move(selection))
        {
        }

        auto entity_selection::data() const -> const c_api::entity_descriptor *
        {
            return selection_.data();
        }

        size_t entity_selection::size() const { return selection_.size(); }

        relational_table relational_table::instance_ = {};
        relational_table &relational_table::instance() { return relational_table::instance_; }

        void relational_table::register_components(
            const c_api::entity_descriptor &owningEntity,
            const c_api::component_descriptor *componentArray,
            size_t length,
            c_api::reg_error *resArray)
        {
            for (size_t i = 0; i != length; ++i)
            {
                const auto res = register_component(owningEntity, componentArray[i]);
                if (resArray)
                    resArray[i] = res;
            }
        }

        c_api::reg_error relational_table::register_component(
            const c_api::entity_descriptor &owningEntity,
            const c_api::component_descriptor &component)
        {
            auto optionalColumn = get_column(component);
            if (!optionalColumn)
            {
                componentsTable_.emplace(std::make_pair(component, std::vector{ owningEntity }));
                return c_api::reg_error::success;
            }

            component_column &column = *optionalColumn;
            auto iter = std::find(column.cbegin(), column.cend(), owningEntity);
            if (iter != column.cend())
                return c_api::reg_error::component_already_registered;

            column.push_back(owningEntity);
            std::sort(column.begin(), column.end());

            return c_api::reg_error::success;
        }

        void relational_table::unregister_components(
            const c_api::entity_descriptor &owningEntity,
            const c_api::component_descriptor *componentArray,
            size_t length,
            c_api::unreg_error *resArray)
        {
            for (size_t i = 0; i != length; ++i)
            {
                const auto res = unregister_component(owningEntity, componentArray[i]);
                if (resArray)
                    resArray[i] = res;
            }
        }

        c_api::unreg_error relational_table::unregister_component(
            const c_api::entity_descriptor &owningEntity,
            const c_api::component_descriptor &component)
        {
            auto optionalColumn = get_column(component);
            if (!optionalColumn)
                return c_api::unreg_error::component_not_found;

            component_column &column = *optionalColumn;

            auto iter = std::find(column.cbegin(), column.cend(), owningEntity);
            if (iter == column.cend())
                return c_api::unreg_error::component_not_registered;

            column.erase(iter);
            return c_api::unreg_error::success;
        }

        auto relational_table::get_column(const c_api::component_descriptor &component)
            -> std::optional<std::reference_wrapper<component_column>>
        {
            auto found = componentsTable_.find(component);
            if (found == componentsTable_.cend())
                return std::nullopt;

            return found->second;
        }

        void relational_table::release() { relational_table::instance_ = {}; }

        selections selections::instance_{};

        selections &selections::instance() { return selections::instance_; }

        c_api::entity_selection selections::store(entity_selection &&selection)
        {
            const auto id = pool_.next_available();

            c_api::entity_selection out{ id, selection.data(), selection.size() };

            [[maybe_unused]] auto res =
                selections_.emplace(std::make_pair(id, std::move(selection)));
            assert(res.second && "Unexpected ID conflict");

            return out;
        }

        void selections::free(c_api::entity_selection &selection)
        {
            selections_.erase(selection.handle);
            pool_.free(selection.handle);
            selection = {};
        }

        void selections::release() { selections::instance_ = {}; }

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

            c_core::relational_table::instance().register_components(owningEntity,
                                                                     array,
                                                                     length,
                                                                     results);
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

            c_core::relational_table::instance().unregister_components(owningEntity,
                                                                       array,
                                                                       length,
                                                                       results);
        }

        void select_entities_by_components(const component_descriptor *array,
                                           size_t length,
                                           entity_selection &result)
        {
            using difference_type = std::iterator_traits<decltype(array)>::difference_type;
            result = c_core::selections::instance().store(   //
                c_core::relational_table::instance().select(
                    std::vector<component_descriptor>(array,
                                                      std::next(array, difference_type(length)))));
        }

        void free_entity_selection(entity_selection &selection)
        {
            c_core::selections::instance().free(selection);
        }
    }   // namespace c_api
}   // namespace eld
