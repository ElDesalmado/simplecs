option(CLEAN_BUILD "Clean build with no warnings allowed" ON)
message(STATUS "Clean build, warnings are not allowed: " ${CLEAN_BUILD})

option(ALLOW_DEPRECATED "Exclude deprecated as error" OFF)
message(STATUS "Exclude deprecated as error: " ${ALLOW_DEPRECATED})

if (MSVC)
    add_compile_options(/W4)
    # warnings as errors
    if (${CLEAN_BUILD})
        add_compile_options(/WX)
        #exclude deprecated
        if (NOT ${ALLOW_DEPRECATED})
            add_compile_options(-wd4996)
        endif()
    endif ()
else ()
    add_compile_options(-Wall -Wextra -pedantic)
    # warnings as errors
    if (${CLEAN_BUILD})
        add_compile_options(-Werror)
        # exclude deprecated
        if (${ALLOW_DEPRECATED})
            add_compile_options(-Wno-error=deprecated-declarations)
        endif ()
    endif ()
endif ()
