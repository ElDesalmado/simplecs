#pragma once

#include <type_traits>
#include <utility>

// TODO: specification

/**
 * C++ public API for Component.
 *
 *
 * - access data
 * - access data const
 *
 */

namespace eld
{
    namespace custom
    {
        /**
         * Get component descriptor.
         * @tparam ComponentDescriptorT
         * @tparam ImplT
         * @param impl
         * @return
         */
        template<typename ComponentDescriptorT, typename ImplT>
        constexpr ComponentDescriptorT component_descriptor(const ImplT &impl)
        {
            return impl.component_descriptor_type();
        }

    }   // namespace custom

    namespace generic
    {
        template<typename ImplT, typename TraitsT>
        class component
        {
        public:
            using implementation_type = ImplT;
            using traits = TraitsT;

            using component_descriptor_type = typename traits::component_descriptor_type;

            /**
             * Typename of contained data.
             */
            using type = typename component_descriptor_type::type;

            template<
                bool DefaultConstructible = std::is_default_constructible_v<implementation_type>,
                std::enable_if_t<DefaultConstructible, int> * = nullptr>
            component()   //
              : impl_()
            {
            }

            template<typename... ArgsT>
            component(ArgsT &&...implArgs)   //
              : impl_(std::forward<ArgsT>(implArgs)...)
            {
            }

            constexpr component_descriptor_type descriptor() const
            {
                return custom::component_descriptor<component_descriptor_type>(impl_);
            }

        private:
            implementation_type impl_;
        };
    }   // namespace generic

    template<typename ImplT, typename Traits, typename... ArgsT>
    generic::component<ImplT, Traits> component(ArgsT &...implArgs)
    {
        return generic::component<ImplT, Traits>(std::forward<ArgsT>(implArgs)...);
    }
}   // namespace eld
