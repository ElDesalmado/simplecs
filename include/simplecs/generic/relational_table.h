#pragma once

#include <tuple>
#include <type_traits>

namespace eld
{
    namespace custom
    {
        /**
         * Customization point for select_entities function.
         * @tparam SelectionT
         * @tparam TagIncludeT
         * @tparam TagExcludeT
         * @tparam ComponentT
         * @tparam ExcludeComponentT
         * @tparam ImplT
         * @param impl
         * @param tagInclude
         * @param tagExclude
         * @return
         */
        template<template<typename...> class SelectionT,
                 template<typename...>
                 class TagIncludeT,
                 template<typename...>
                 class TagExcludeT,
                 typename... ComponentT,
                 typename... ExcludeComponentT,
                 typename ImplT>
        SelectionT<ComponentT...> select_entities(ImplT &impl,
                                                  TagIncludeT<ComponentT...> tagInclude,
                                                  TagExcludeT<ExcludeComponentT...> tagExclude)
        {
            return impl.select_entities(tagInclude, tagExclude);
        }

        /**
         * Customisation point for registering components.
         * @tparam ComponentsT
         * @tparam EntityDescriptorT
         * @tparam ImplT
         * @param impl
         * @param entityDescriptorT
         */
        template<typename... ComponentsT, typename EntityDescriptorT, typename ImplT>
        void register_components(ImplT &impl, const EntityDescriptorT &entityDescriptorT)
        {
            impl.template register_components<ComponentsT...>(entityDescriptorT);
        }

        /**
         * Customisation point for registering components.
         * @tparam ComponentReferencesTupleT Tuple type of component references.
         * @tparam EntityDescriptorT
         * @tparam ImplT
         * @param impl
         * @param entityDescriptorT
         * @param componentsTuple
         */
        template<typename ComponentReferencesTupleT, typename EntityDescriptorT, typename ImplT>
        void register_components(ImplT &impl,
                                 const EntityDescriptorT &entityDescriptorT,
                                 ComponentReferencesTupleT &&componentsTuple)
        {
            impl.template register_components(
                entityDescriptorT,
                std::forward<ComponentReferencesTupleT>(componentsTuple));
        }

        /**
         * Customisation point for registering components.
         * @tparam ComponentsT
         * @tparam EntityDescriptorT
         * @tparam ImplT
         * @param impl
         * @param entityDescriptorT
         */
        template<typename... ComponentsT, typename EntityDescriptorT, typename ImplT>
        void unregister_components(ImplT &impl, const EntityDescriptorT &entityDescriptorT)
        {
            impl.template unregister_components<ComponentsT...>(entityDescriptorT);
        }

    }   // namespace custom

    namespace generic
    {
        template<typename... Components>
        struct include_components
        {
        };

        template<typename... Components>
        struct exclude_components
        {
        };

        /**
         * Generic C++ interface for ECS relational table
         * @tparam TraitsT Traits collection for implementation
         * @tparam ImplT Composition type for implementation.
         */
        template<typename ImplT, typename TraitsT>
        class relational_table
        {
        public:
            using implementation_type = ImplT;
            using traits = TraitsT;
            using entity_traits = typename traits::entity_traits;
            using component_traits = typename traits::component_traits;

            using entity_descriptor_type = typename entity_traits::entity_descriptor_type;

            template<typename ComponentT>
            using component_reference_type =
                typename component_traits::template component_reference_type<ComponentT>;

            template<typename... ComponentT>
            using entity_selection_type =
                typename implementation_type::template entity_selection_type<ComponentT...>;

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
             * Select entities associated with a given set of Components
             * @tparam ComponentsT
             * @param includeTag
             * @return
             */
            template<typename... ComponentsT>
            entity_selection_type<ComponentsT...> select_entities(
                include_components<ComponentsT...> includeTag)
            {
                return custom::select_entities<entity_selection_type>(impl_,
                                                                      includeTag,
                                                                      exclude_components<>());
            }

            /**
             * Select entities associated with a given set of Components excluding specific
             * components.
             * @tparam ComponentsT
             * @tparam ExcludeComponentsT
             * @param includeTag
             * @param excludeTag
             * @return
             */
            template<typename... ComponentsT, typename... ExcludeComponentsT>
            entity_selection_type<ComponentsT...> select_entities(
                include_components<ComponentsT...> includeTag,
                exclude_components<ExcludeComponentsT...> excludeTag)
            {
                return custom::select_entities<entity_selection_type>(impl_,
                                                                      includeTag,
                                                                      excludeTag);
            }

            /**
             * Associate a set of \p ComponentsT with a given entity.
             * @throws implementation-defined exception
             * @tparam ComponentsT
             * @param entityDescriptor
             */
            template<typename... ComponentsT>
            void register_components(const entity_descriptor_type &entityDescriptor)
            {
                return custom::register_components<ComponentsT...>(impl_, entityDescriptor);
            }

            /**
             * Associate a set of \p ComponentsT with a given entity. Allows to associate (share)
             * individual components to multiple entities.
             * @tparam ComponentsT
             * @param entityDescriptor
             * @param componentReferences
             */
            template<typename... ComponentsT>
            void register_components(
                const entity_descriptor_type &entityDescriptor,
                std::tuple<component_reference_type<ComponentsT>...> componentReferences)
            {
                return custom::register_components(impl_, entityDescriptor, componentReferences);
            }

            /**
             * Remove association of a given \p entityDescriptor with \p ComponentsT
             * @tparam ComponentsT
             * @param entityDescriptor
             */
            template<typename... ComponentsT>
            void unregister_components(const entity_descriptor_type &entityDescriptor)
            {
                return custom::unregister_components<ComponentsT...>(impl_, entityDescriptor);
            }

        private:
            implementation_type impl_;
        };

    }   // namespace generic

    template<typename ImplT, typename TraitsT, typename... ImplArgsT>
    constexpr generic::relational_table<ImplT, TraitsT> make_relational_table(ImplArgsT &&...args)
    {
        return generic::relational_table<ImplT, TraitsT>(std::forward<ImplArgsT>(args)...);
    }

}   // namespace eld
