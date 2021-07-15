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

namespace eld
{
    namespace impl
    {

        template<typename ClassType>
        class component_c
        {
        public:
            using component_descriptor_type = c_api::component_descriptor;
            using type = ClassType;

            SIMPLECS_DECL static component_descriptor_type component_descriptor();

        private:
            class c_context
            {
            public:
                c_context();
                ~c_context();

                [[nodiscard]] c_api::component_storage_descriptor descriptor() const;
            private:
                c_api::component_storage_descriptor descriptor_;
            };

        private:
        };

    }   // namespace impl

    template <typename ClassT>
    using component_c = impl::component_c<ClassT>;

}   // namespace eld

#ifdef SIMPLECS_HEADER_ONLY
#    include "simplecs/src/components.ipp"
#endif