#pragma once

#include "simplecs/c_core/storage.hpp"

#include <new>

namespace eld
{
    namespace c_core
    {
        namespace detail
        {
            template<typename Ret, typename... ArgsT>
            std::function<Ret(ArgsT...)> bind(Ret (*pFunc)(ArgsT...), void *pObject)
            {
                struct anonymous;
                using func_type = decltype(pFunc);
                using mem_func_type = Ret (anonymous::*)(ArgsT...);
                static_assert(sizeof(func_type) <= sizeof(mem_func_type),
                              "Can't convert to member function pointer!");

                return [=](ArgsT... args)
                {
                    mem_func_type pMemFunc = nullptr;
                    reinterpret_cast<void *&>(pMemFunc) = reinterpret_cast<void *>(pFunc);
                    return (reinterpret_cast<anonymous *>(pObject)->*pMemFunc)(args...);
                };
            }

        }   // namespace detail

        component_storage::component_storage(component_id id, const c_api::storage_params &params)
          : componentId_(id),
            componentSize_(params.componentSize),
            pInPlaceConstructor_(component_storage::make_constructor(params)),
            pInPlaceDestructor_(component_storage::make_destructor(params))
        {
        }

        component_storage::constructor_type component_storage::make_constructor(
            const c_api::storage_params &params)
        {
            if (!params.pInPlaceConstruct)
                return {};

            if (!params.pConstructorCallable)
                return params.pInPlaceConstruct;

            return detail::bind(params.pInPlaceConstruct, params.pConstructorCallable);
        }

        component_storage::destructor_type component_storage::make_destructor(
            const c_api::storage_params &params)
        {
            if (!params.pInPlaceDestroy)
                return {};

            if (!params.pDestructorCallable)
                return params.pInPlaceDestroy;

            return detail::bind(params.pInPlaceDestroy, params.pDestructorCallable);
        }

        c_api::allocate_component_error component_storage::allocate(
            const c_api::entity_descriptor &entity,
            c_api::component_pointer *&pointer)
        {
            auto optionalFound = find_component(entity);
            if (!optionalFound)
                return c_api::allocate_component_error::already_exists;

            void *pAllocatedMemory = operator new(componentSize_);
            pointer->pObject = pAllocatedMemory;
            pointer->componentSize = componentSize_;
            pointer->componentDescriptor.id = componentId_;

            return c_api::allocate_component_error::success;
        }

        c_api::deallocate_component_error component_storage::deallocate(
            const c_api::entity_descriptor &entity,
            c_api::component_pointer *&pointer)
        {
            auto optionalFound = find_component(entity);
            if (!optionalFound)
                return c_api::deallocate_component_error::invalid_entity;

            void *pObject = *optionalFound;

            if (pInPlaceDestructor_)
                pInPlaceDestructor_(pObject, componentSize_);

            operator delete(pObject);
            *pointer = {};

            return c_api::deallocate_component_error::success;
        }

        c_api::allocate_component_error component_storage::construct(
            const c_api::entity_descriptor &entity,
            c_api::component_pointer *&pointer,
            c_api::tuple *args,
            size_t argsSizeBytes)
        {
            if (!pInPlaceConstructor_)
                return c_api::allocate_component_error::invalid_constructor;

            auto res = allocate(entity, pointer);
            if ((bool)res)
                return res;

            pInPlaceConstructor_(pointer->pObject, componentSize_, args, argsSizeBytes);
            return c_api::allocate_component_error::success;
        }

        std::optional<void *> component_storage::find_component(
            const c_api::entity_descriptor &entity)
        {
            auto found = components_.find(entity);
            if (found != components_.cend())
                return std::nullopt;

            return reinterpret_cast<void *>(found->second);
        }

        c_api::get_component_error component_storage::get_component(
            const c_api::entity_descriptor &entity,
            c_api::component_pointer *&pointer)
        {
            auto optionalFound = find_component(entity);
            if (!optionalFound)
                return c_api::get_component_error::invalid_entity;

            pointer->pObject = *optionalFound;
            pointer->componentSize = componentSize_;
            pointer->componentDescriptor.id = componentId_;

            return c_api::get_component_error::success;
        }

    }   // namespace c_core

    namespace c_api
    {
        allocation_component_storage_error init_component_storage(
            component_storage_descriptor &outputDescriptor,
            const storage_params &inputParams)
        {
            // TODO: implement
        }

        release_component_storage_error release_component_storage(
            component_storage_descriptor &storageDescriptor)
        {
            // TODO: implement
        }

        allocate_component_error allocate_component(const entity_descriptor &entity,
                                                    const component_descriptor &componentDescriptor,
                                                    component_pointer *&pointer)
        {
            // TODO: implement
            return allocate_component_error::success;
        }

        allocate_component_error construct_component(const entity_descriptor &entity,
                                                     const component_descriptor &component,
                                                     component_pointer *&pointer,
                                                     const tuple *args,
                                                     size_t argsSizeBytes)
        {
            // TODO: implement
            return allocate_component_error::success;
        }

        deallocate_component_error deallocate_component(const entity_descriptor &entity,
                                                        component_pointer *&pointer)
        {
            // TODO: implement
            return deallocate_component_error::success;
        }

        get_component_error get_component(const entity_descriptor &entity,
                                          const component_descriptor &componentDescriptor,
                                          component_pointer *&pointer)
        {
            // TODO: implement
            return get_component_error::success;
        }
    }   // namespace c_api

}   // namespace eld
