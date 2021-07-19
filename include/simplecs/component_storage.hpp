#pragma once

#include "simplecs/c_api/types.h"
#include "simplecs/config.hpp"

// TODO: dllexport/dllimport typedefs to export component types

#include <vector>

namespace simplecs::eld
{
    template<typename ClassType>
    class component_reference
    {
    public:
        using component_type = ClassType;
        using handle_type = decltype(std::declval<c_api::component_descriptor>().handle);

        explicit component_reference(simplecs::c_api::component_pointer pointer)   //
          : pointer_(pointer)
        {
        }

        constexpr component_type &get()
        {
            return *static_cast<component_type *>(static_cast<void *>(pointer_.pObject));
        }

        const component_type &get() const
        {
            return *static_cast<component_type *>(static_cast<void *>(pointer_.pObject));
        }

        component_type &operator*() { return get(); }

        const component_type &operator*() const { return get(); }

    private:
        simplecs::c_api::component_pointer pointer_;
    };

    /**
     * C++ Component Storage interface implementation.
     * \todo singleton?
     * @tparam ClassType
     */
    template<typename ClassType>
    class component_storage_impl
    {
    public:
        using handle_type = decltype(std::declval<c_api::component_descriptor>().handle);
        using component_type = ClassType;
        using component_reference_type = component_reference<component_type>;

        struct component_traits
        {
            using handle_type = typename component_storage_impl::handle_type;
            using component_type = typename component_storage_impl::component_type;
            using component_reference_type = component_storage_impl::component_reference_type;
        };

        component_storage_impl();

        template<typename... ComponentArgsTuples>
        std::vector<component_reference_type> emplace_components(
            ComponentArgsTuples &&...argsTuples)
        {
            return { component_storage_impl::allocate_component(
                std::forward<ComponentArgsTuples>(argsTuples),
                std::make_index_sequence<std::tuple_size_v<ComponentArgsTuples>>())... };
        }

        template<template<typename> class ContainerT>
        void remove_components(const ContainerT<handle_type> &handles);

        component_reference_type get_component(const handle_type &componentHandle)
        {
            const auto descriptor =
                c_api::component_descriptor{ componentHandle,
                                             component_storage_impl::get_type_descriptor() };
            c_api::component_pointer pointer{};
            auto error = c_api::get_component(descriptor, pointer);
            if (bool(error))
                throw std::invalid_argument(
                    "eld::component_storage::get_component: invalid component descriptor");

            return component_reference_type(pointer);
        }

        const component_reference_type get_component(const handle_type &componentHandle) const
        {
            const auto descriptor =
                c_api::component_descriptor{ componentHandle,
                                             component_storage_impl::get_type_descriptor() };
            c_api::component_pointer pointer{};
            auto error = c_api::get_component(descriptor, pointer);
            if (bool(error))
                throw std::invalid_argument(
                    "eld::component_storage::get_component: invalid component descriptor");

            return component_reference_type(pointer);
        }

    private:
        static SIMPLECS_DECL simplecs::c_api::type_descriptor get_type_descriptor();

        template<typename... ArgsT, size_t... I>
        static component_reference_type allocate_component(std::tuple<ArgsT &&...> args,
                                                                      std::index_sequence<I...>)
        {
            c_api::component_pointer componentPointer{};
            auto error = c_api::allocate_component(get_type_descriptor(), componentPointer);
            if (bool(error))
                throw std::invalid_argument(
                    "eld::component_storage: failed to allocate new component!");

            new (componentPointer.pObject)
                component_type(std::forward<ArgsT>(std::get<I>(args))...);

            return component_reference_type(componentPointer);
        }

    private:
        // TODO:  RAII for emplaced components
    };

}   // namespace simplecs::eld

#ifdef SIMPLECS_HEADER_ONLY
#    include "simplecs/src/component_storage.ipp"
#endif