#pragma once

#if !defined(SIMPLECS_HEADER_ONLY)
#    if !defined(SIMPLECS_SHARED)
#        define SIMPLECS_HEADER_ONLY
#    endif
#endif

// TODO: fix SIMPLECS_DECL to export symbols with GCC on Ubuntu
#if defined(SIMPLECS_HEADER_ONLY)
#    define SIMPLECS_DECL inline
#else
#    if defined(__WIN32) || defined(_MSC_VER) || defined(__BORLANDC__) || defined(__CODEGEARC__)
#        if defined(SIMPLECS_SHARED)
#            if defined(SIMPLECS_SOURCE)
#                define SIMPLECS_DECL __declspec(dllexport)
#            else
#                define SIMPLECS_DECL __declspec(dllimport)
#            endif
#        endif
#    elif defined(__GNUC__)
#        if defined(SIMPLECS_SHARED)
#            if defined(SIMPLECS_SOURCE)
#                define SIMPLECS_DECL __attribute__((visibility("default")))
#            else
#                define SIMPLECS_DECL /*__attribute__((visibility ("default")))*/
#            endif
#        endif
#    endif
#endif