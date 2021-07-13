#pragma once

#include <type_traits>
#include <utility>

namespace eld
{
    namespace custom
    {
        template <typename ContainerT, typename ImplT>
        constexpr bool empty(const ContainerT& container, const ImplT&)
        {
            return container.empty();
        }
    }

    namespace generic
    {
        template<typename ImplT>
        class selector
        {
        public:
            using implementation_type = ImplT;
            using entity_id_type = typename ImplT::entity_id_type;
            using component_id_type = typename ImplT::component_id_type;
            using result_type = typename ImplT::result_type;

            template<bool DefaultConstructible = std::is_default_constructible_v<implementation_type>,
                     std::enable_if_t<DefaultConstructible, int>* = nullptr>
            selector()
                : impl_()
            {}

            template <typename ... ArgsT>
            selector(ArgsT &&...args)
                : impl_(std::forward<ArgsT>(args)...)
            {}

            template<template<class, class> typename MapT,
                     template<class> typename ComponentColumnT,
                     template<class> typename InputContainerT>
            constexpr result_type operator()(const MapT<component_id_type, ComponentColumnT<entity_id_type>> &componentsTable,
                                                   const InputContainerT<component_id_type> &componentsToSelect)
            {
                if (custom::empty(componentsTable, impl_) ||
                    custom::empty(componentsToSelect, impl_))
                    return {};

                auto filteredColumns = impl_.select_columns(componentsTable, componentsToSelect);
                if (custom::empty(filteredColumns, impl_))
                    return {};

                return impl_.set_intersection(std::move(filteredColumns));
            }

        private:
            implementation_type impl_;
        };
    }// namespace generic


}// namespace eld
