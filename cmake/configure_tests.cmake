
get_directory_property(IS_EXCLUDED EXCLUDE_FROM_ALL)

if (NOT IS_EXCLUDED)
    option(BUILD_TESTS "Build tests" OFF)
elseif()
    set(BUILD_TESTS FALSE)
endif()