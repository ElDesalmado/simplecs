#pragma once

#define SIMPLECS_SOURCE

#include "simplecs/config.hpp"

#if defined(SIMPLESC_HEADER_ONLY)
#    error "Do not compile Simplesec library source with SIMPLESC_HEADER_ONLY defined"
#endif

#include "simplecs/src/c_registry.ipp"
#include "simplecs/c_api/src/relational.ipp"
#include "simplecs/c_api/src/storage.ipp"
