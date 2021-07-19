#pragma once

#include "simplecs/config.hpp"
#include "simplecs/generic/components.h"

// TODO: use only c_api types
#include "simplecs/c_api/relational.hpp"
#include "simplecs/c_api/storage.hpp"

#include <cstddef>

/**
 * Component implementation based on C api.
 */

namespace simplecs
{
    namespace impl
    {
        /**
         * This is a wrapper class. Do not store it.
         * @tparam ClassType
         */
        template<typename ClassType>
        class component_c
        {
        public:
            using component_descriptor_type = c_api::component_descriptor;
            using entity_descriptor_type = c_api::entity_descriptor;
            using type = ClassType;

            template <typename ... ArgsT>
            explicit component_c(const entity_descriptor_type& entityDescriptor, ArgsT &&...args);

            [[nodiscard]] component_descriptor_type component_descriptor() const
            {
                return descriptor_;
            }

            [[nodiscard]] entity_descriptor_type entity_descriptor() const
            {
                return entityDescriptor_;
            }

            type& get();
            const type& get() const;

            ~component_c();

        private:
            class c_context
            {
            public:
                c_context();
                ~c_context();

                [[nodiscard]] c_api::component_storage_descriptor descriptor() const;

                // TODO: implement construction within C storage

            private:
                c_api::component_storage_descriptor descriptor_;
            };

            SIMPLECS_DECL static c_api::component_storage_descriptor storage_descriptor();

        private:
            component_descriptor_type descriptor_;
            entity_descriptor_type entityDescriptor_;
        };

        // TODO: define and use common traits
        struct component_c_traits
        {
            template<class T>
            using component_descriptor_type = typename T::component_descriptor_type;

            template<class T>
            using type = typename T::type;
        };
    }   // namespace impl

    template <typename ClassT>
    using component_c = generic::component<impl::component_c<ClassT>, impl::component_c_traits>;

}   // namespace eld

#ifdef SIMPLECS_HEADER_ONLY
#    include "simplecs/src/components.ipp"
#endif