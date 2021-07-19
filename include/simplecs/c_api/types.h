#pragma once

#include <cstddef>

namespace eld::c_api
{
    /**
     * Typesafe handle
     */
    struct handle
    {
        size_t h = -1;
    };

    /**
     * C type for type description
     */
    struct type_descriptor
    {
        size_t typeId = -1;
        size_t typeSize = -1;
    };

    /**
     * Handle type for Entity
     */
    struct entity_descriptor
    {
        handle handle{};
    };

    /**
     * C descriptor type for a component.
     * Serves to identify component's type and current component's handle
     */
    struct component_descriptor
    {
        handle handle{};
        type_descriptor typeDescriptor{};
    };

    /**
     * Selection result. Owned by core.
     */
    struct entity_selection
    {
        handle handle{};
        const entity_descriptor *array = nullptr;
        size_t length = 0;
    };

    /**
     * Type wrapper for a component's data pointer.
     */
    struct component_pointer
    {
        void *pObject{};
        component_descriptor componentDescriptor;
    };

    constexpr inline bool operator<(const handle& lhs, const handle& rhs)
    {
        return lhs.h < rhs.h;
    }

    constexpr inline bool operator<(const entity_descriptor &lhs, const entity_descriptor &rhs)
    {
        return lhs.handle < rhs.handle;
    }

    constexpr inline bool operator<(const type_descriptor &lhs, const type_descriptor &rhs)
    {
        return lhs.typeId < rhs.typeId;
    }

    constexpr inline bool operator==(const entity_descriptor &lhs, const entity_descriptor &rhs)
    {
        return !(lhs < rhs) && !(rhs < lhs);
    }

    constexpr inline bool operator<(const component_descriptor &lhs,
                                    const component_descriptor &rhs)
    {
        return lhs.typeDescriptor < rhs.typeDescriptor;
    }

    constexpr inline bool operator==(const component_descriptor &lhs,
                                     const component_descriptor &rhs)
    {
        return !(lhs < rhs) && !(rhs < lhs);
    }

}