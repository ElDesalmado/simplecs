#pragma once

#include <type_traits>
#include <utility>

/**
 * // TODO: std-like descriptions
 * Types:
 * - exported: available from other dynamically linked units
 * - local: visible only within this unit
 *
 * ECS Registry API:
 * - emplace(Components...) - create entity with given components
 */

namespace eld
{
    namespace traits
    {
        template<typename, typename = void>
        struct get_traits
        {
            using type = void;
        };

        template<typename T>
        struct get_traits<T, std::void_t<typename T::traits>>
        {
            using type = typename T::traits;
        };

        template<typename T>
        using get_traits_t = typename get_traits<T>::type;

    }   // namespace traits

    namespace custom
    {
        template<typename ImplT>
        constexpr auto create_entity(ImplT &impl)
        {
            return impl.create_entity();
        }

        template<typename ImplT, typename EntityType>
        constexpr void destroy_entity(ImplT &impl, const EntityType &id)
        {
            impl.destroy_entity(id);
        }

        template<typename ImplT, typename EntityType, typename... Components>
        constexpr auto emplace_components(ImplT &impl,
                                          const EntityType &id,
                                          Components &&...components)
        {
            return impl.emplace_components(id, std::forward<Components>(components)...);
        }

    }   // namespace custom

    namespace generic
    {
        template<typename ImplT, typename Traits = traits::get_traits_t<ImplT>>
        class view
        {
        public:
        private:
        };

        /**
         *
         * @tparam ImplT
         * @tparam Traits
         */
        template<typename ImplT, typename Traits = traits::get_traits_t<ImplT>>
        class registry
        {
            static_assert(!std::is_void<Traits>(),
                          "Traits must be provided for registry's implementation.");

        public:
            using impl_type = ImplT;
            using traits = Traits;

            using entity_type = typename traits::entity_type;

            template<typename... ArgsT>
            explicit registry(ArgsT &&...implArgs)
              : implementation_(std::forward<ArgsT>(implArgs)...)
            {
            }

            template<bool DefaultConstructible = std::is_default_constructible_v<impl_type>,
                     std::enable_if_t<DefaultConstructible, int> * = nullptr>
            registry()
            {
            }

            constexpr entity_type create() { return custom::create_entity(implementation_); }

            constexpr void destroy(const entity_type &id)
            {
                custom::destroy_entity(implementation_, id);
            }

            template<typename... Components>
            constexpr auto emplace(const entity_type &id, Components &&...components)
            {
                return custom::emplace_components(implementation_,
                                                  id,
                                                  std::forward<Components>(components)...);
            }

            /*
             *
             */

        private:
            impl_type implementation_;
        };
    }   // namespace generic

    //    template <typename EntityType, typename >
    //    class scoped_entity
    //    {
    //    public:
    //        using entity_type = EntityType;
    //
    //        scoped_entity(EntityType rawEntity)
    //            : raw_(rawEntity)
    //        {}
    //
    //
    //
    //    private:
    //        entity_type raw_;
    //    };
    //
}   // namespace eld