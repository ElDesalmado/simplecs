#pragma once

#if !defined(SIMPLESC_HEADER_ONLY)
#    if !defined(SIMPLESC_SHARED)
#        define SIMPLESC_HEADER_ONLY
#    endif
#endif

// TODO: fix SIMPLESC_DECL to export symbols with GCC on Ubuntu
#if defined(SIMPLESC_HEADER_ONLY)
#    define SIMPLESC_DECL inline
#else
#    if defined(__WIN32) || defined(_MSC_VER) || defined(__BORLANDC__) || defined(__CODEGEARC__)
#        if defined(SIMPLESC_SHARED)
#            if defined(SIMPLESC_SOURCE)
#                define SIMPLESC_DECL __declspec(dllexport)
#            else
#                define SIMPLESC_DECL __declspec(dllimport)
#            endif
#        endif
#    endif
#endif