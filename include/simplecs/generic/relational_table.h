#pragma once

#include <tuple>
#include <type_traits>

namespace simplecs
{
    namespace custom
    {
        template<template<typename> typename ContainerT,
                 typename ComponentDescriptorT,
                 typename ImplT>
        constexpr auto select_entities(ImplT &impl,
                                       const ContainerT<ComponentDescriptorT> &includeTypes,
                                       const ContainerT<ComponentDescriptorT> &excludeTypes)
        {
            return impl.select_entities(includeTypes, excludeTypes);
        }

        /**
         * Customisation point for registering components.
         * @throws Implementation-defined exception on error.
         * @tparam EntityHandleT
         * @tparam ImplT
         * @tparam ContainerT
         * @tparam ComponentHandleT
         * @param impl
         * @param entityHandle
         * @param componentHandles
         */
        template<typename EntityHandleT,
                 typename ImplT,
                 template<typename>
                 typename ContainerT,
                 typename ComponentHandleT>
        auto register_components(ImplT &impl,
                                 const EntityHandleT &entityHandle,
                                 const ContainerT<ComponentHandleT> &componentHandles)
        {
            return impl.register_components(entityHandle, componentHandles);
        }

        /**
         * Customisation point for registering components.
         * @tparam ComponentsT
         * @tparam EntityDescriptorT
         * @tparam ImplT
         * @param impl
         * @param entityDescriptorT
         */
        template<typename EntityHandleT,
                 typename ImplT,
                 template<typename>
                 typename ContainerT,
                 typename ComponentDescriptorT>
        void unregister_components(ImplT &impl,
                                   const EntityHandleT &entityHandle,
                                   const ContainerT<ComponentDescriptorT> &componentDescriptors)
        {
            impl.unregister_components(entityHandle, componentDescriptors);
        }
    }   // namespace custom

    template<typename...>
    struct type_list
    {
    };

    namespace generic
    {
        /**
         * Generic C++ interface for ECS relational table
         * @tparam TraitsT Traits collection for implementation
         * @tparam ImplT Composition type for implementation.
         */
        template<typename ImplT, typename TraitsT = typename ImplT::traits>
        class relational_table
        {
        public:
            using implementation_type = ImplT;
            using traits = TraitsT;
            using entity_handle_type = typename traits::entity_handle_type;
            using component_descriptor_type = typename traits::component_descriptor_type;
            using component_handle_type = typename traits::component_handle_type;

            template<
                bool DefaultConstructible = std::is_default_constructible_v<implementation_type>,
                std::enable_if_t<DefaultConstructible, int> * = nullptr>
            relational_table()   //
              : impl_()
            {
            }

            template<typename... ArgsT>
            explicit relational_table(ArgsT &&...args)   //
              : impl_(std::forward<ArgsT>(args)...)
            {
            }

            /**
             * Select entities associated with a given set of Components excluding specific
             * components.
             */
            template<template<typename> typename ContainerT>
            constexpr auto select_entities(
                const ContainerT<component_descriptor_type> &includeTypes,
                const ContainerT<component_descriptor_type> &excludeTypes)
            {
                return custom::select_entities(impl_, includeTypes, excludeTypes);
            }

            /**
             * Associate a set of \p ComponentsT with a given entity.
             * @throws implementation-defined exception
             * @tparam ComponentsT
             * @param entityDescriptor
             */
            template<template<typename> typename ContainerT>
            auto register_components(const entity_handle_type &entityDescriptor,
                                     const ContainerT<component_handle_type> &componentHandles)
            {
                return custom::register_components(impl_, entityDescriptor, componentHandles);
            }

            /**
             * Remove association of a given \p entityDescriptor with \p ComponentsT
             * @tparam ComponentsT
             * @param entityDescriptor
             */
            template<template<typename> typename ContainerT>
            void unregister_components(
                const entity_handle_type &entityDescriptor,
                const ContainerT<component_descriptor_type> &componentDescriptors)
            {
                return custom::unregister_components(impl_, entityDescriptor, componentDescriptors);
            }

        private:
            implementation_type impl_;
        };

    }   // namespace generic

    template<typename ImplT, typename TraitsT = typename ImplT::traits, typename... ImplArgsT>
    constexpr generic::relational_table<ImplT, TraitsT> make_relational_table(ImplArgsT &&...args)
    {
        return generic::relational_table<ImplT, TraitsT>(std::forward<ImplArgsT>(args)...);
    }

}   // namespace simplecs
