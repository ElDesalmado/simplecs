#pragma once

#include <cstddef>
#include <cstdint>
#include <utility>

// TODO: clarification for handles and descriptors

namespace simplecs::c_api
{
    struct object;
    struct callable;

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
        object *pObject{};
        component_descriptor componentDescriptor;
    };

    struct storage_params
    {
        size_t componentSize{};
        void (*pDestroy)(c_api::callable *, c_api::object *){};
        c_api::callable *pCallable{};
    };

    enum class reg_error : uint8_t
    {
        success = 0,
        component_already_registered
    };

    constexpr inline bool operator<(const handle& lhs, const handle& rhs)
    {
        return lhs.h < rhs.h;
    }

    constexpr inline bool operator==(const handle &lhs, const handle &rhs)
    {
        return lhs.h == rhs.h;
    }

    constexpr inline handle& operator++(handle &h)
    {
        ++h.h;
        return h;
    }

    constexpr inline const handle operator++(handle &h, int)
    {
        auto temp = h;
        ++h.h;
        return temp;
    }

    constexpr inline bool operator<(const entity_descriptor &lhs, const entity_descriptor &rhs)
    {
        return lhs.handle < rhs.handle;
    }

    constexpr inline bool operator==(const entity_descriptor &lhs, const entity_descriptor &rhs)
    {
        return !(lhs < rhs) && !(rhs < lhs);
    }

    constexpr inline bool operator<(const type_descriptor &lhs, const type_descriptor &rhs)
    {
        return lhs.typeId < rhs.typeId;
    }

    constexpr inline bool operator==(const type_descriptor &lhs, const type_descriptor &rhs)
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

namespace simplecs
{
    /**
     * Make wrapped stateless destructor for \tp T
     * @tparam T
     * @return
     */
    template <typename T>
    constexpr auto make_destructor()
    {
        return [](c_api::callable *, c_api::object *pObject)
        {
            static_cast<T*>(static_cast<void*>(pObject))->~T();
        };
    }
}