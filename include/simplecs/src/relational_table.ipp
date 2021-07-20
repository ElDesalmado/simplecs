#pragma once

#include "simplecs/relational_table.hpp"

#include <iterator>
#include <stdexcept>
#include <type_traits>

namespace simplecs::eld
{
    namespace detail
    {
        template<typename ContainerT>
        using iterator_category =
            typename std::iterator_traits<typename ContainerT::iterator>::iterator_category;

        template<typename ContainerT>
        using value_type = typename std::iterator_traits<typename ContainerT::iterator>::value_type;

        template<typename ContainerT,
                 bool /*false*/ =
                     std::is_same_v<iterator_category<ContainerT>, std::random_access_iterator_tag>>
        class array_provider
        {
        public:
            using value_type = value_type<ContainerT>;

            explicit array_provider(const ContainerT &container)
              : data_(container.cbegin(), container.cend())
            {
            }

            value_type *data() { return data_.data(); }

            [[nodiscard]] size_t size() const { return data_.size(); }

        private:
            std::vector<value_type> data_;
        };

        template<typename ContainerT>
        class array_provider<ContainerT, true>
        {
        public:
            using value_type = value_type<ContainerT>;

            explicit array_provider(const ContainerT &container)   //
              : container_(container)
            {
            }

            value_type *data() { return container_.data(); }

            [[nodiscard]] size_t size() const { return container_.size(); }

        private:
            const ContainerT &container_;
        };

    }   // namespace detail

    template<template<typename> typename ContainerT>
    /*constexpr*/ auto relational_table_impl::select_entities(
        const ContainerT<component_descriptor_type> &includeComponents,
        const ContainerT<component_descriptor_type> & /*excludeComponents*/)
        -> std::vector<entity_handle_type>
    {
        // TODO: implement exclusion

        auto arrayProvider = detail::array_provider(includeComponents);
        c_api::entity_selection selectionRes{};

        c_api::select_entities_by_components(arrayProvider.data(),
                                             arrayProvider.size(),
                                             selectionRes,
                                             nullptr);

        // TODO: errors
        // if length == 0, array must be nullptr
        assert(selectionRes.length || !selectionRes.array && "Unexpected selection result!");

        using difference_type =
            typename std::iterator_traits<decltype(selectionRes.array)>::difference_type;

        auto ret = std::vector(selectionRes.array,
                               std::next(selectionRes.array, difference_type(selectionRes.length)));

        c_api::free_entity_selection(selectionRes);

        return ret;
    }

    template<template<typename> typename ContainerT>
    std::vector<c_api::reg_error> relational_table_impl::register_components(
        const relational_table_impl::entity_handle_type &entityDescriptor,
        const ContainerT<component_handle_type> &componentHandles)
    {
        std::vector<c_api::reg_error> results(componentHandles.size());
        auto componentHandlesArray = detail::array_provider(componentHandles);

        c_api::register_components(entityDescriptor,
                                   componentHandlesArray.data(),
                                   componentHandlesArray.size(),
                                   results.data(),
                                   nullptr);

        // TODO: handle errors?

        return results;
    }

    template<template<typename> typename ContainerT>
    std::vector<c_api::reg_error> relational_table_impl::unregister_components(
        const relational_table_impl::entity_handle_type &entityDescriptor,
        const ContainerT<component_descriptor_type> &componentDescriptors)
    {
        std::vector<c_api::reg_error> results(componentDescriptors.size());
        auto componentHandlesArray = detail::array_provider(componentDescriptors);

        c_api::unregister_components(entityDescriptor,
                                     componentHandlesArray.data(),
                                     componentHandlesArray.size(),
                                     results.data(),
                                     nullptr);

        // TODO: handle errors?

        return results;
    }
}   // namespace simplecs::eld
