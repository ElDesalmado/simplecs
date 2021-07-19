#pragma once

#include <type_traits>

namespace eld
{
    namespace custom
    {
        /**
         * Emplace several
         * @tparam ImplT
         * @tparam ComponentArgsTuples
         * @param impl
         * @param componentArgsTuples
         * @return Container of References to emplaced components
         * \todo template return type?
         */
        template<typename ImplT, typename... ComponentArgsTuples>
        auto emplace_components(ImplT &impl, ComponentArgsTuples &&...componentArgsTuples)
        {
            return impl.emplace_components(
                std::forward<ComponentArgsTuples>(componentArgsTuples)...);
        }

        template<typename ImplT, template<typename> class Container, typename ComponentHandleT>
        auto remove_components(ImplT &impl, const Container<ComponentHandleT> &handles)
        {
            return impl.remove_components(handles);
        }

        template<typename ImplT, typename ComponentHandleT>
        auto get_component(ImplT &impl, const ComponentHandleT &componentHandle)
        {
            return impl.get_component(componentHandle);
        }

        template<typename ImplT, typename ComponentHandleT>
        const auto get_component(const ImplT &impl, const ComponentHandleT &componentHandle)
        {
            return impl.get_component(componentHandle);
        }

    }   // namespace custom

    namespace generic
    {
        /**
         * Generic C++ template interface for a component storage.
         * @tparam ImplT
         * @tparam ComponentTraits
         */
        template<typename ImplT, typename ComponentTraits = typename ImplT::component_traits>
        class component_storage
        {
        public:
            using implementation_type = ImplT;
            using component_traits = ComponentTraits;
            using handle_type = typename component_traits::handle_type;
            using component_type = typename component_traits::component_type;
            using component_reference_type = typename component_traits::component_reference_type;

            template <bool DefaultConstructible = std::is_default_constructible_v<implementation_type>,
                std::enable_if_t<DefaultConstructible, int>* = nullptr>
            component_storage()
                : impl_()
            {}

            template <typename ... ArgsT>
            explicit component_storage(ArgsT &&... args)
                : impl_(std::forward<ArgsT>(args)...)
            {}

            /**
             * Emplace several components.
             * @tparam TupleArgsT
             * @param argsTuples
             * @returns Container of references
             */
            template<typename... TupleArgsT>
            auto emplace(TupleArgsT &&...argsTuples)
            {
                return custom::emplace_components(impl_, std::forward<TupleArgsT>(argsTuples)...);
            }

            /**
             * Remove components from the storage.
             * @tparam ContainerType
             * @param handles
             * @return
             */
            template<template<typename> class ContainerType>
            auto remove(const ContainerType<handle_type> &handles)
            {
                return custom::remove_components(impl_, handles);
            }

            // TODO: get reference
            component_reference_type get(const handle_type &componentHandle)
            {
                return custom::get_component(impl_, componentHandle);
            }

            // TODO: const reference?
            const component_reference_type get(const handle_type &componentHandle) const
            {
                return custom::get_component(impl_, componentHandle);
            }

        private:
            implementation_type impl_;
        };
    }   // namespace generic

    template <typename ImplT, typename TraitsT = typename ImplT::component_traits, typename ... ArgsT>
    generic::component_storage<ImplT, TraitsT> make_component_storage(ArgsT &&... args)
    {
        return generic::component_storage<ImplT, TraitsT>(std::forward<ArgsT>(args)...);
    }
}   // namespace eld