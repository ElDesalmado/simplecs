#pragma once

namespace simplecs
{
    namespace custom
    {
    }

    namespace generic
    {
        // TODO: do I need this?
        /**
         * Generic template C++ interface for a selection of entities.
         * @tparam ImplT
         * @tparam TraitsT
         * @tparam ComponentsT
         */
        template<typename ImplT, typename TraitsT, typename... ComponentsT>
        class entity_selection
        {
        public:
            using implementation_type = ImplT;
            using traits = TraitsT;

            using entity_traits = typename traits::entity_traits;
            using component_traits = typename traits::component_traits;

            using entity_descriptor_type = typename entity_traits::entity_descriptor_type;

            template<typename T>
            using iterator_type = typename implementation_type::template iterator_type<T>;

            template<typename T>
            using const_iterator_type =
                typename implementation_type::template const_iterator_type<T>;

            template<typename ComponentT>
            using component_reference_type =
                typename component_traits::template component_reference_type<ComponentT>;

            template<
                bool DefaultConstructible = std::is_default_constructible_v<implementation_type>,
                std::enable_if_t<DefaultConstructible, int> * = nullptr>
            entity_selection()   //
              : impl_()
            {
            }

            template<typename... ArgsT>
            explicit entity_selection(ArgsT &&...args)   //
              : impl_(std::forward<ArgsT>(args)...)
            {
            }

            iterator_type<entity_descriptor_type> begin()
            {
                return impl_.begin();
            }

            const_iterator_type<entity_descriptor_type> begin() const
            {
                return impl_.begin();
            }

            const_iterator_type<entity_descriptor_type> cbegin() const
            {
                return impl_.cbegin();
            }

            iterator_type<entity_descriptor_type> end()
            {
                return impl_.end();
            }

            const_iterator_type<entity_descriptor_type> end() const
            {
                return impl_.end();
            }

            const_iterator_type<entity_descriptor_type> cend() const
            {
                return impl_.cend();
            }

        private:
            implementation_type impl_;
        };
    }   // namespace generic
}   // namespace eld
