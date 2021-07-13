#pragma once

/*
 * API for exporting when registry is compiled as a shared library.
 */

#include <cstdint>

extern "C" {

struct entity
{
    size_t id;
};

void create_entity(entity&);
void destroy_entity(entity&);

/**
 * Internal storage for a specific component type.
 */
struct component_storage {
    void *handle;
    const size_t component_id;
    const size_t component_size;

    void (*pAllocate)(void *&);
    void (*pDestroy)(void *);
};

struct entity_view {

    struct iterator {
        void *handle;
        entity entity_id;
    };

    void (*const pBegin)(iterator &);
    void (*const pEnd)(iterator &);

    size_t (*const pDist)(const iterator &begin, const iterator &end);

    void (*pNext)(iterator &, size_t);
    void (*pPrev)(iterator &, size_t);
};

void get_view(int entityId, int *componentsIdArray, size_t arraySize, entity_view &);
void free_view(entity_view&);

}
