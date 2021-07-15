#pragma once

#include "simplecs/components.hpp"

namespace eld::impl
{
    template<typename ClassType>
    component_c<ClassType>::c_context::c_context()
    {
        // TODO: errors, params
        auto error =
            c_api::init_component_storage(descriptor_, c_api::storage_params{ sizeof(type) });
    }

    template<typename ClassType>
    component_c<ClassType>::c_context::~c_context()
    {
        auto error = c_api::release_component_storage(descriptor_);
    }

    template<typename ClassType>
    c_api::component_storage_descriptor component_c<ClassType>::c_context::descriptor() const
    {
        return descriptor_;
    }

}   // namespace eld::impl
