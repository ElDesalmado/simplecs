#pragma once

#include <algorithm>
#include <unordered_map>
#include <vector>

#include <functional>

namespace simplecs::impl
{
    template<typename EntityT, typename ComponentT>
    class selector_std
    {
    public:
        using entity_id_type = EntityT;
        using component_id_type = ComponentT;
        using result_type = std::vector<entity_id_type>;

        template<template<typename> typename ColumnType>
        using column_reference_type = std::reference_wrapper<const ColumnType<entity_id_type>>;

        template<template<typename> typename ColumnType, template<typename> typename InputContainer>
        static auto select_columns(
            const std::unordered_map<component_id_type, ColumnType<entity_id_type>>
                &componentsTable,
            const InputContainer<component_id_type> &selectComponents)
            -> std::vector<column_reference_type<ColumnType>>
        {
            std::vector<column_reference_type<ColumnType>> out{};
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

        template<template<typename> typename ColumnType>
        static result_type set_intersection(
            const std::vector<column_reference_type<ColumnType>> &columns)
        {
            if (columns.empty())
                return {};

            auto sorted = columns;
            std::sort(sorted.begin(),
                      sorted.end(),
                      [](const auto &lhs, const auto &rhs)   //
                      { return lhs.get().size() < rhs.get().size(); });

            auto iter = sorted.cbegin();
            result_type out(iter->get().cbegin(), iter->get().cend()),   //
                currentSet{};
            std::sort(out.begin(), out.end());

            while ((iter = std::next(iter)) != sorted.cend() && !std::empty(out))
            {
                result_type column(iter->get().cbegin(), iter->get().cend());
                std::sort(column.begin(), column.end());

                std::set_intersection(out.cbegin(),
                                      out.cend(),
                                      column.cbegin(),
                                      column.cend(),
                                      std::back_inserter(currentSet));

                std::swap(out, currentSet);
                currentSet.clear();
            }

            return out;
        }

    private:
    };
}   // namespace simplecs::impl