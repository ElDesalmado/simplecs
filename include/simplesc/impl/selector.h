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
        using const_ref_component_column_type = std::reference_wrapper<const component_column_type>;
        using result_type = std::vector<entity_id_type>;

        static std::vector<const_ref_component_column_type> select_columns(
            const std::unordered_map<component_id_type, component_column_type> &componentsTable,
            const std::vector<component_id_type> &selectComponents)
        {
            std::vector<const_ref_component_column_type> out{};
            std::for_each(selectComponents.cbegin(),
                          selectComponents.cend(),   //
                          [&out, &componentsTable](const component_id_type &componentId) mutable
                          {
                              auto found = componentsTable.find(componentId);
                              if (found == componentsTable.cend())
                                  return;
                              out.push_back(found->second);
                          });
            return out;
        }

        static result_type set_intersection(
            const std::vector<const_ref_component_column_type> &columns)
        {
            if (columns.empty())
                return { };

            auto sorted = columns;
            std::sort(sorted.begin(),
                      sorted.end(),
                      [](const auto &lhs, const auto &rhs)   //
                      { return lhs.get().size() < rhs.get().size(); });

            auto iter = sorted.cbegin();
            result_type out = *iter,
                currentSet{};

            while ((iter = std::next(iter)) != sorted.cend() &&
                   !std::empty(out))
            {
                auto &column = *iter;
                std::set_intersection(out.cbegin(), out.cend(),
                                      column.get().cbegin(), column.get().cend(),
                                      std::back_inserter(currentSet));

                std::swap(out, currentSet);
            }

            return out;
        }

    private:
    };
}   // namespace eld::impl