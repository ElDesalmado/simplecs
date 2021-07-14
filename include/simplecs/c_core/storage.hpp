#pragma once

#include "simplecs/c_core_api.hpp"
#include "simplecs/config.hpp"

#include <functional>
#include <tuple>
#include <vector>
#include <optional>

namespace std
{
    template<>
    struct hash<eld::c_api::entity_descriptor>
    {
        size_t operator()(const eld::c_api::entity_descriptor &entity) const noexcept
        {
            return std::hash<size_t>{}(entity.id);
        }
    };
}   // namespace std

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
                                                  c_api::tuple *args,
                                                  size_t argsSizeBytes);

        c_api::get_component_error get_component(const c_api::entity_descriptor &entity,
                                                 c_api::component_pointer *&pointer);

    private:
        using constructor_type = std::function<void(void *pAllocatedMemory,
                                                    size_t allocatedSize,
                                                    const c_api::tuple *args,
                                                    size_t argsSizeBytes)>;

        using destructor_type = std::function<void(void *pObject, size_t objectSize)>;

        static constructor_type make_constructor(const c_api::storage_params &params);
        static destructor_type make_destructor(const c_api::storage_params &params);

        std::optional<void*> find_component(const c_api::entity_descriptor &entity);

    private:
        const component_id componentId_;
        const size_t componentSize_;

        constructor_type pInPlaceConstructor_{};
        destructor_type pInPlaceDestructor_{};

        // TODO: use contiguous storage
        std::unordered_map<c_api::entity_descriptor, size_t> components_;
    };

}   // namespace eld::c_core

#ifdef SIMPLECS_HEADER_ONLY
#    include "simplecs/c_core/src/storage.ipp"
#endif