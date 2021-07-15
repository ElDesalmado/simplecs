#pragma once

#include "simplecs/c_api/c_core.hpp"
#include "simplecs/config.hpp"

#include "simplecs/generic/selector.h"
#include "simplecs/impl/selector.h"
#include "simplecs/impl/id_pool.h"

#include <unordered_map>
#include <vector>

namespace std
{
    template<>
    struct hash<eld::c_api::component_descriptor>
    {
        size_t operator()(const eld::c_api::component_descriptor &component) const noexcept
        {
            return hash<size_t>()(component.id);
        }
    };
}   // namespace std

namespace eld::c_core
{
    class entity_selection
    {
    public:
        explicit entity_selection(std::vector<c_api::entity_descriptor> &&selection);

        entity_selection(entity_selection &&) = default;
        entity_selection &operator=(entity_selection &&) = default;

        [[nodiscard]] auto data() const -> const c_api::entity_descriptor*;

        [[nodiscard]] size_t size() const;

    private:
        std::vector<c_api::entity_descriptor> selection_;
    };

    class relational_table
    {
    public:
        static relational_table &instance();
        static void release();

        void register_components(const c_api::entity_descriptor &owningEntity,
                                 const c_api::component_descriptor *componentArray,
                                 size_t length,
                                 c_api::reg_error *resArray);

        c_api::reg_error register_component(const c_api::entity_descriptor &owningEntity,
                                            const c_api::component_descriptor &component);

        void unregister_components(const c_api::entity_descriptor &owningEntity,
                                   const c_api::component_descriptor *componentArray,
                                   size_t length,
                                   c_api::unreg_error *resArray);

        c_api::unreg_error unregister_component(const c_api::entity_descriptor &owningEntity,
                                                const c_api::component_descriptor &component);

        entity_selection select(const std::vector<c_api::component_descriptor> &selectComponents)
        {
            return entity_selection(selector_(componentsTable_, selectComponents));
        }

    private:
        using component_column = std::vector<c_api::entity_descriptor>;

        auto get_column(const c_api::component_descriptor &component)
            -> std::optional<std::reference_wrapper<component_column>>;

    private:
        static relational_table instance_;

        eld::generic::selector<
            eld::impl::selector_std<c_api::entity_descriptor, c_api::component_descriptor>>
            selector_;

        std::unordered_map<c_api::component_descriptor, component_column> componentsTable_;
    };

    class selections
    {
    public:
        static selections& instance();
        static void release();

        c_api::entity_selection store(c_core::entity_selection &&selection);
        void free(c_api::entity_selection &selection);

    private:
        using selection_id = size_t;

    private:

        static selections instance_;

        std::unordered_map<size_t, entity_selection> selections_;
        eld::detail::id_pool<selection_id> pool_;
    };

}   // namespace eld::c_core

#ifdef SIMPLECS_HEADER_ONLY
#    include "simplecs/c_api/src/relational.ipp"
#endif