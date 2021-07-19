#pragma once

#include "simplecs/component_storage.hpp"

#include "simplecs/c_api/storage.hpp"

#include <cassert>

namespace simplecs::eld
{
    template<typename ClassType>
    component_storage_impl<ClassType>::component_storage_impl()
    {
    }

    template<typename ClassType>
    simplecs::c_api::type_descriptor component_storage_impl<ClassType>::get_type_descriptor()
    {
        static simplecs::c_api::type_descriptor typeDescriptor_;
        constexpr auto storageParams =
            c_api::storage_params{ sizeof(ClassType), simplecs::make_destructor<ClassType>() };

        [[maybe_unused]] static auto error =
            c_api::init_component_storage(storageParams, typeDescriptor_, nullptr);
        assert(!bool(error) && "Unhandled component initialization failure");

        return typeDescriptor_;
    }

    template<typename ClassType>
    template<template<typename> class ContainerT>
    void component_storage_impl<ClassType>::remove_components(
        const ContainerT<handle_type> &handles)
    {
        std::for_each(
            handles.cbegin(),
            handles.cend(),
            [](const handle_type &handle)
            {
                auto error = c_api::deallocate_component(
                    c_api::component_descriptor{ handle,
                                                 component_storage_impl::get_type_descriptor() });
                if (bool(error))
                    throw std::invalid_argument("eld::component_storage::remove_components: "
                                                "invalid component descriptor");
            });
    }
}   // namespace simplecs::eld
