#pragma once

#include "simplecs/c_api/types.h"
#include "simplecs/config.hpp"

#include "simplecs/impl/id_pool.h"

#include <functional>
#include <limits>
#include <optional>
#include <tuple>
#include <unordered_map>
#include <vector>

template<>
struct std::hash<simplecs::c_api::handle>
{
    size_t operator()(const simplecs::c_api::handle &handle) const
    {
        return std::hash<decltype(handle.h)>{}(handle.h);
    }
};

namespace simplecs::c_core
{
    using component_id = size_t;

    struct component_traits
    {
        using handle_type = c_api::handle;
        using component_type = void;
        using component_reference_type = c_api::component_pointer;
    };

    class component_storage_impl
    {
    public:
        using component_traits = c_core::component_traits;
        using handle_type = typename component_traits::handle_type;
        using component_reference_type = typename component_traits::component_reference_type;

        using component_size = decltype(std::declval<c_api::type_descriptor>().typeSize);

        component_storage_impl(c_api::storage_params storageParams,
                               c_api::type_descriptor typeDescriptor) noexcept;

        std::vector<component_reference_type> emplace_components(int num);

        template<template<typename> class Container>
        auto remove_components(const Container<handle_type> &handles);

        component_reference_type get_component(const handle_type &componentHandle);

        // TODO: const reference type?
        component_reference_type get_component(const handle_type &componentHandle) const;

        ~component_storage_impl();

    private:
        static c_api::object *allocate(component_size componentSize);

        static std::function<void(c_api::object *pObject)> make_destructor(
            void (*pFunction)(c_api::callable *, c_api::object *),
            c_api::callable *pCallable);

        void deallocate(c_api::object *pObject);

    private:
        c_api::type_descriptor typeDescriptor_;
        std::unordered_map<handle_type, c_api::object *> map_;
        simplecs::detail::id_pool<decltype(std::declval<handle_type>().h)> handlePool_;
        std::function<void(c_api::object *pObject)> deallocate_;
    };


}   // namespace eld::c_core

#ifdef SIMPLECS_HEADER_ONLY
#    include "simplecs/c_api/src/component_storage.ipp"
#endif