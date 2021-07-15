add_executable(sandbox sandbox.cpp)
target_link_libraries(sandbox simplecs)

add_executable(test.selector test_selector.cpp)
target_link_libraries(test.selector simplecs GTest::gtest)
add_test(NAME test.selector
        COMMAND test.selector)

add_executable(test.c_api test_c_api.cpp)
target_link_libraries(test.c_api eld::simplecs_shared GTest::gtest)
add_test(NAME test.c_api
        COMMAND test.c_api
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

# TODO: find a convenient approach
add_library(test.export_components_lib SHARED test_export_components_lib.cpp)
target_link_libraries(test.export_components_lib PUBLIC eld::simplecs)
target_compile_definitions(test.export_components_lib PUBLIC SIMPLECS_SHARED)

# workaround for MinGW
if (WIN32 AND CMAKE_COMPILER_IS_GNUCC)
    target_link_options(test.export_components_lib PRIVATE "-Wl,--exclude-all-symbols")
endif()


message(STATUS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
