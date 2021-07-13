#pragma once

#define SIMPLESC_SOURCE

#include "simplesc/config.hpp"

#if defined(SIMPLESC_HEADER_ONLY)
#    error "Do not compile Simplesec library source with SIMPLESC_HEADER_ONLY defined"
#endif

#include "simplesc/src/c_registry.ipp"