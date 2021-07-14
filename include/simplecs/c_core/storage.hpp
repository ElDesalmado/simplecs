#pragma once

#include "simplecs/c_core_api.hpp"
#include "simplecs/config.hpp"

#include <functional>
#include <limits>
#include <optional>
#include <stack>
#include <tuple>
#include <vector>

namespace eld::c_core
{
    using component_id = size_t;

    class component_storage
    {
    public:
        component_storage(component_id id, const c_api::storage_params &params);

        c_api::allocate_component_error allocate(const c_api::entity_descriptor &entity,
                                                 c_api::component_pointer *&pointer);
        c_api::deallocate_component_error deallocate(const c_api::entity_descriptor &entity,
                                                     c_api::component_pointer *&pointer);

        c_api::allocate_component_error construct(const c_api::entity_descriptor &entity,
                                                  c_api::component_pointer *&pointer,
                                                  const c_api::tuple *args,
                                                  size_t argsSizeBytes);

        c_api::get_component_error get_component(const c_api::entity_descriptor &entity,
                                                 c_api::component_pointer *&pointer);

        ~component_storage();

    private:
        using entity_id = size_t;
        using pointer_type = size_t;
        using constructor_type = std::function<void(void *pAllocatedMemory,
                                                    size_t allocatedSize,
                                                    const c_api::tuple *args,
                                                    size_t argsSizeBytes)>;

        using destructor_type = std::function<void(void *pObject, size_t objectSize)>;

        static constructor_type make_constructor(const c_api::storage_params &params);
        static destructor_type make_destructor(const c_api::storage_params &params);

        std::optional<pointer_type> find_component(const c_api::entity_descriptor &entity);

        void in_place_destroy(pointer_type pObject);
        static void free(pointer_type pointer);

    private:
        const component_id componentId_;
        const size_t componentSize_;

        constructor_type pInPlaceConstructor_{};
        destructor_type pInPlaceDestructor_{};

        // TODO: use contiguous storage
        // TODO: use custom objects?
        std::unordered_map<entity_id, pointer_type, std::hash<entity_id>> components_;
    };

    class storages
    {
    public:
        static storages &instance();

        std::optional<std::reference_wrapper<component_storage>> get_storage(
            const c_api::component_descriptor &descriptor);

        c_api::allocation_component_storage_error init_storage(
            c_api::component_storage_descriptor &outputDescriptor,
            const c_api::storage_params &inputParams);

        c_api::release_component_storage_error release(
            c_api::component_storage_descriptor &storageDescriptor);

    private:
        size_t next_available_id();

    private:
        static storages instance_;

        // TODO: use descriptor ?
        std::unordered_map<size_t, component_storage> map_;
        size_t instances_;
        std::stack<size_t> freedInstances_;
    };

}   // namespace eld::c_core

#ifdef SIMPLECS_HEADER_ONLY
#    include "simplecs/c_core/src/storage.ipp"
#endif