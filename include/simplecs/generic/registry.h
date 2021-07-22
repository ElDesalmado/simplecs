#pragma once

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

// TODO: specification

// TODO: add component construction with tuple of forwarded arguments
/**
 * C++ generic API class for registry.
 * Functions:
 * 1. Create Entity
 * 2. Create Several Entities
 * 2. Destroy Entity
 * 2. Add components
 * 3. Emplace Entity (with components)
 * 4. Select Entities with Components with Predicate (that accepts Components)
 * 5. Select Entities without Components
 * 6. Select Entities with Components N and without Components M with Predicate (that accepts
 * Components)
 *
 */

namespace simplecs
{
    template<typename ComponentT, typename... ArgsT>
    struct component_args
    {
        std::tuple<ArgsT &&...> forwardedArgs;
    };

    template<typename ComponentT, typename... ArgsT>
    component_args<ComponentT, ArgsT...> forward_construct(ArgsT &&...args)
    {
        return { std::forward_as_tuple(args...) };
    }

    namespace custom
    {
        template <typename ComponentArgPackT>
        struct arg_pack_component_type;

        template <typename ComponentT, typename ... ArgsT>
        struct arg_pack_component_type<component_args<ComponentT, ArgsT...>>
        {
            using type = ComponentT;
        };

        /**
         * Customization point for entity creation.
         * @tparam ImplT
         * @tparam EntityType
         * @param impl
         * @return New Entity
         */
        template<typename EntityType, typename ImplT>
        EntityType create_entity(ImplT &impl)
        {
            return impl.create_entity();
        }

        /**
         * Customization point for entity creation. Tries to allocate new entity with user-provide
         * descriptor. Shall throw on failure.
         * @tparam ImplT
         * @tparam EntityType
         * @tparam EntityDescriptor
         * @param impl
         * @param descriptor User-specified descriptor.
         * @return New Entity with user-specified id.
         */
        template<typename EntityType, typename ImplT, typename EntityDescriptor = size_t>
        EntityType create_entity(ImplT &impl, const EntityDescriptor &descriptor)
        {
            return impl.create_entity(descriptor);
        }

        template<typename ImplT, typename EntityDescriptorT, typename... ComponentsT>
        std::tuple<ComponentsT &...> add_components(ImplT &impl,
                                                    const EntityDescriptorT &entityDescriptor,
                                                    ComponentsT &&...components)
        {
            return impl.add_components(entityDescriptor, std::forward<ComponentsT>(components)...);
        }

        template<typename EntityType, typename ImplT, typename... ComponentsT>
        std::tuple<EntityType, ComponentsT &...> emplace_components(ImplT &impl,
                                                                    ComponentsT &&...components)
        {
            return impl.emplace_components(std::forward<ComponentsT>(components)...);
        }

//        template<typename SelectionType,
//                 typename ImplT,
//                 typename EntityDescriptorT,
//                 typename... ComponentsT>
//        SelectionType select_entities(ImplT &impl,
//                                      const EntityDescriptorT &entityDescriptor,
//                                      type_list<ComponentsT...> including)
//        {
//            return impl.select_entities(entityDescriptor, including);
//        }

    }   // namespace custom

    namespace generic
    {
        /**
         * Registry public API interface.
         *
         * @tparam ImplT
         * @tparam Traits
         */
        template<typename ImplT, typename Traits>
        class registry
        {
        public:
            using implementation_type = ImplT;
            using traits = Traits;

            using entity_type = typename traits::entity_type;
            using entity_descriptor = typename entity_type::descriptor;

            template<typename... ComponentsT>
            using selection_type =
                typename implementation_type::template selection_type<ComponentsT...>;

            template<
                bool DefaultConstructible = std::is_default_constructible_v<implementation_type>,
                std::enable_if_t<DefaultConstructible, int> * = nullptr>
            registry() : impl_()
            {
            }

            template<typename... ArgsT>
            registry(ArgsT &&...implArgs)   //
              : impl_(std::forward<ArgsT>(implArgs)...)
            {
            }

            /**
             * Create new entity.
             * @return Created entity.
             */
            entity_type create_entity() { return custom::create_entity<entity_type>(impl_); }

            /**
             * Try to create new entity with user-provided descriptor.
             * @throws ExceptionT if descriptor if an entity with this descriptor already exists.
             * @param descriptor
             * @return Created entity.
             */
            entity_type create_entity(const entity_descriptor &descriptor)
            {
                return custom::create_entity<entity_type>(impl_, descriptor);
            }

            /**
             * Add components to an existing entity.
             * @throws ExceptionT if descriptor is invalid.
             * @tparam ComponentsT
             * @param entityDescriptor
             * @param components
             * @return Tuple of references to created components.
             */
            template<typename... ComponentsT>
            std::tuple<ComponentsT &...> add_components(const entity_descriptor &entityDescriptor,
                                                        ComponentsT &&...components)
            {
                return custom::add_components(impl_,
                                              entityDescriptor,
                                              std::forward<ComponentsT>(components)...);
            }

            /**
             * Create new entities with existing components.
             * @tparam ComponentsT
             * @param components
             * @return
             */
            template<typename... ComponentsT>
            std::tuple<entity_type, ComponentsT &...> emplace(ComponentsT &&...components)
            {
                return custom::emplace_components<entity_type>(
                    impl_,
                    std::forward<ComponentsT>(components)...);
            }

            /**
             * Get selection for entities that has components of given types
             * @tparam ComponentsT Components to include on search.
             * @param entityDescriptor
             * @return
             */
//            template<typename... ComponentsT>
//            selection_type<ComponentsT...> select(const entity_descriptor &entityDescriptor)
//            {
//                return custom::select_entities<selection_type<ComponentsT...>>(
//                    impl_,
//                    entityDescriptor,
//                    type_list<ComponentsT...>());
//            }

        private:
            implementation_type impl_;
        };
    }   // namespace generic

    template<typename ImplT, typename Traits, typename... ArgsT>
    generic::registry<ImplT, Traits> make_registry(ArgsT &...implArgs)
    {
        return generic::registry<ImplT, Traits>(std::forward<ArgsT>(implArgs)...);
    }

}   // namespace eld
