#pragma once

#include <algorithm>
#include <unordered_map>
#include <vector>

namespace eld::impl
{
    template<typename EntityT, typename ComponentT>
    class selector_std_impl
    {
    public:
        using entity_id_type = EntityT;
        using component_id_type = ComponentT;
        using component_column_type = std::vector<entity_id_type>;
        using ref_component_column_type = std::reference_wrapper<component_column_type>;
        using result_type = std::vector<entity_id_type>;

        static std::vector<ref_component_column_type> select_columns(
            const std::unordered_map<component_id_type, component_column_type> &componentsTable,
            const std::vector<component_id_type> &selectComponents)
        {
            // TODO: implement
            std::vector<ref_component_column_type> out{};
            std::for_each(selectComponents.cbegin(),
                          selectComponents.cend(),   //
                          [&out, &componentsTable](const component_id_type &componentId) mutable {

                          });

            return out;
        }

        static result_type set_intersection(std::vector<ref_component_column_type> &&columns)
        {
            // TODO: implement
            result_type out{};

            return out;
        }

    private:
    };
}   // namespace eld::impl