#pragma once

#include "simplecs/c_core/storage.hpp"

#include <algorithm>
#include <cassert>
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

            in_place_destroy(*optionalFound);
            component_storage::free(*optionalFound);
            *pointer = {};
            return c_api::deallocate_component_error::success;
        }

        c_api::allocate_component_error component_storage::construct(
            const c_api::entity_descriptor &entity,
            c_api::component_pointer *&pointer,
            const c_api::tuple *args,
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

        std::optional<component_storage::pointer_type> component_storage::find_component(
            const c_api::entity_descriptor &entity)
        {
            auto found = components_.find(entity.id);
            if (found != components_.cend())
                return std::nullopt;

            return found->second;
        }

        c_api::get_component_error component_storage::get_component(
            const c_api::entity_descriptor &entity,
            c_api::component_pointer *&pointer)
        {
            auto optionalFound = find_component(entity);
            if (!optionalFound)
                return c_api::get_component_error::invalid_entity;

            pointer->pObject = reinterpret_cast<decltype(pointer->pObject)>(*optionalFound);
            pointer->componentSize = componentSize_;
            pointer->componentDescriptor.id = componentId_;

            return c_api::get_component_error::success;
        }

        void component_storage::in_place_destroy(component_storage::pointer_type pObject)
        {
            if (pInPlaceDestructor_)
                pInPlaceDestructor_(reinterpret_cast<void *>(pObject), componentSize_);
        }

        void component_storage::free(component_storage::pointer_type pointer)
        {
            operator delete(reinterpret_cast<void *>(pointer));
        }

        component_storage::~component_storage()
        {
            std::for_each(components_.begin(),
                          components_.end(),
                          [this](auto pair)
                          {
                              this->in_place_destroy(pair.second);
                              component_storage::free(pair.second);
                          });
        }

        storages storages::instance_{};

        storages &storages::instance() { return instance_; }

        std::optional<std::reference_wrapper<component_storage>> storages::get_storage(
            const c_api::component_descriptor &descriptor)
        {
            auto found = map_.find(descriptor.id);
            if (found == map_.cend())
                return std::nullopt;

            return found->second;
        }

        size_t storages::next_available_id()
        {
            if (freedInstances_.empty())
                return instances_++;

            const auto id = freedInstances_.top();
            freedInstances_.pop();
            return id;
        }

        c_api::allocation_component_storage_error storages::init_storage(
            c_api::component_storage_descriptor &outputDescriptor,
            const c_api::storage_params &inputParams)
        {
            if (!inputParams.componentSize)
                return c_api::allocation_component_storage_error::invalid_component_size;

            const auto id = next_available_id();
            [[maybe_unused]] auto res =
                map_.emplace(std::make_pair(id, component_storage(id, inputParams)));
            assert(res.second && "Filed to emplace new component storage!");

            outputDescriptor.componentSize = inputParams.componentSize;
            outputDescriptor.componentDescriptor.id = id;

            return c_api::allocation_component_storage_error::success;
        }

        c_api::release_component_storage_error storages::release(
            c_api::component_storage_descriptor &storageDescriptor)
        {
            auto found = map_.find(storageDescriptor.componentDescriptor.id);
            if (found == map_.cend())
                return c_api::release_component_storage_error::invalid_component_descriptor;

            map_.erase(found);
            return c_api::release_component_storage_error::success;
        }

    }   // namespace c_core

    namespace c_api
    {
        allocation_component_storage_error init_component_storage(
            component_storage_descriptor &outputDescriptor,
            const storage_params &inputParams)
        {
            return c_core::storages::instance().init_storage(outputDescriptor, inputParams);
        }

        release_component_storage_error release_component_storage(
            component_storage_descriptor &storageDescriptor)
        {
            return c_core::storages::instance().release(storageDescriptor);
        }

        allocate_component_error allocate_component(const entity_descriptor &entity,
                                                    const component_descriptor &componentDescriptor,
                                                    component_pointer *&pointer)
        {
            auto optionalStorage = c_core::storages::instance().get_storage(componentDescriptor);
            if (!optionalStorage)
                return allocate_component_error::invalid_component_descriptor;

            return optionalStorage->get().allocate(entity, pointer);
        }

        allocate_component_error construct_component(
            const entity_descriptor &entity,
            const component_descriptor &componentDescriptor,
            component_pointer *&pointer,
            const tuple *args,
            size_t argsSizeBytes)
        {
            auto optionalStorage = c_core::storages::instance().get_storage(componentDescriptor);
            if (!optionalStorage)
                return allocate_component_error::invalid_component_descriptor;

            return optionalStorage->get().construct(entity, pointer, args, argsSizeBytes);
        }

        deallocate_component_error deallocate_component(const entity_descriptor &entity,
                                                        component_pointer *&pointer)
        {
            auto optionalStorage =
                c_core::storages::instance().get_storage(pointer->componentDescriptor);
            if (!optionalStorage)
                return deallocate_component_error::invalid_component_descriptor;

            return optionalStorage->get().deallocate(entity, pointer);
        }

        get_component_error get_component(const entity_descriptor &entity,
                                          const component_descriptor &componentDescriptor,
                                          component_pointer *&pointer)
        {
            auto optionalStorage = c_core::storages::instance().get_storage(componentDescriptor);
            if (!optionalStorage)
                return get_component_error::invalid_component_id;

            return optionalStorage->get().get_component(entity, pointer);
        }
    }   // namespace c_api

}   // namespace eld
