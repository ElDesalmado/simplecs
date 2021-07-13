#pragma once

#include "simplesc/c_registry_api.h"

#include "simplesc/generic/selector.h"
#include "simplesc/impl/selector.h"

#include <cassert>
#include <iterator>
#include <unordered_map>

namespace std
{
    template<>
    struct hash<eld::c_api::component>
    {
        size_t operator()(const eld::c_api::component &component) const noexcept
        {
            return hash<size_t>()(component.id);
        }
    };
}   // namespace std

namespace eld
{
    class entity_selection
    {
    public:
        explicit entity_selection(std::vector<c_api::entity> &&selection)
          : selection_(std::move(selection))
        {
        }

        entity_selection(entity_selection &&) = default;
        entity_selection &operator=(entity_selection &&) = default;

    private:
        std::vector<c_api::entity> selection_;
    };

    /**
     * Component-Entity table.
     */
    class components_table
    {
    public:
        static components_table &instance() { return components_table::instance_; }

        c_api::entity_selection select(const std::vector<c_api::component> &selectComponents)
        {
            auto selected = selector_(componentsTable_, selectComponents);
            const auto id = get_next_id();

            c_api::entity_selection selection{ id, selected.data(), selected.size() };

            [[maybe_unused]] auto res =
                selections_.emplace(std::make_pair(id, std::move(selected)));
            assert(res.second && "Unexpected ID conflict");

            return selection;
        }

        void free(const c_api::entity_selection &selection) { selections_.erase(selection.handle); }

    private:
        size_t get_next_id()
        {
            if (!freedSelection_)
                return selectionsCounter_++;

            auto temp = freedSelection_;
            freedSelection_ = 0;
            return temp;
        }

    private:
        static components_table instance_;

        eld::generic::selector<eld::impl::selector_std<c_api::entity, c_api::component>> selector_;

        std::unordered_map<c_api::component, std::vector<c_api::entity>> componentsTable_;

        // TODO: standalone class
        std::unordered_map<size_t, entity_selection> selections_;
        size_t selectionsCounter_ = 0,   //
            freedSelection_ = 0;
    };

    components_table components_table::instance_{};

    namespace c_api
    {
        void select_entities_by_components(const component *array,
                                           size_t length,
                                           entity_selection &result)
        {
            // TODO: implement
            using difference_type = std::iterator_traits<decltype(array)>::difference_type;
            result = components_table::instance().select(
                std::vector<component>(array, std::next(array, difference_type(length))));
        }

        void free_entity_selection(const entity_selection &selection)
        {
            components_table::instance().free(selection);
        }

    }   // namespace c_api

}   // namespace eld