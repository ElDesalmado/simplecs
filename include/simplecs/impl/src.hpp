#pragma once

#define SIMPLECS_SOURCE

#include "simplecs/config.hpp"

#if defined(SIMPLESC_HEADER_ONLY)
#    error "Do not compile Simplesec library source with SIMPLESC_HEADER_ONLY defined"
#endif

#include "simplecs/c_api/src/relational.ipp"
#include "simplecs/c_api/src/storage.ipp"
#include "simplecs/c_api/src/entity_storage.ipp"
#include "simplecs/c_api/src/context.ipp"

#include "simplecs/src/registry.ipp"
#include "simplecs/src/components.ipp"