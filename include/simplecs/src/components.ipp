#pragma once

#include "simplecs/components.hpp"

#include <iostream>

namespace eld::impl
{
    template<typename ClassType>
    c_api::component_storage_descriptor component_c<ClassType>::storage_descriptor()
    {
        static c_context context{};
        return context.descriptor();
    }
    template<typename ClassType>
    template<typename... ArgsT>
    component_c<ClassType>::component_c(const entity_descriptor_type &entityDescriptor,
                                        ArgsT &&...args)
      : descriptor_(storage_descriptor().componentDescriptor),
        entityDescriptor_(entityDescriptor)
    {
        // TODO: standalone function
        c_api::component_pointer pointer{};

        auto error =
            c_api::allocate_component(entityDescriptor, this->component_descriptor(), pointer);
        if ((bool)error)
            throw std::invalid_argument("Invalid argument for component");

        new (static_cast<type *>(pointer.pObject)) type(std::forward<ArgsT>(args)...);
    }

    template<typename ClassType>
    component_c<ClassType>::~component_c()
    {
        // TODO: refactor this
        c_api::component_pointer pointer{};
        auto error = c_api::get_component(entity_descriptor(), component_descriptor(), pointer);
        if ((bool)error)
            std::terminate();

        reinterpret_cast<type*>(pointer.pObject)->~type();
//        c_api::deallocate_component(entity_descriptor(), pointer);
    }

    template<typename ClassType>
    typename component_c<ClassType>::type &component_c<ClassType>::get()
    {
        c_api::component_pointer pointer{};
        auto error = c_api::get_component(entity_descriptor(), component_descriptor(), pointer);
        if ((bool)error)
            throw std::invalid_argument("component_c::get: invalid argument");

        return *reinterpret_cast<type *>(pointer.pObject);
    }

    template<typename ClassType>
    const typename component_c<ClassType>::type &component_c<ClassType>::get() const
    {
        c_api::component_pointer pointer{};
        auto error = c_api::get_component(entity_descriptor(), component_descriptor(), pointer);
        if ((bool)error)
            throw std::invalid_argument("component_c::get: invalid argument");

        return *reinterpret_cast<type *>(pointer.pObject);
    }

    template<typename ClassType>
    component_c<ClassType>::c_context::c_context()
    {
        // TODO: errors, params
//        [[maybe_unused]] auto error =
//            c_api::init_component_storage(descriptor_, c_api::storage_params{ sizeof(type) });
    }

    template<typename ClassType>
    component_c<ClassType>::c_context::~c_context()
    {
//        [[maybe_unused]] auto error = c_api::release_component_storage(descriptor_);
    }

    template<typename ClassType>
    c_api::component_storage_descriptor component_c<ClassType>::c_context::descriptor() const
    {
        return descriptor_;
    }

}   // namespace eld::impl
