cmake_minimum_required(VERSION 3.7.2 FATAL_ERROR)
project(simplecs VERSION 0.0.1 LANGUAGES CXX C)

message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "System name: ${CMAKE_SYSTEM_NAME}")

include(cmake/clean_build.cmake)
include(cmake/configure_tests.cmake)
include(cmake/export_visibility.cmake)

# TODO: make configurable
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# TODO: dynamic and header only versions
add_library(simplecs INTERFACE)
target_include_directories(simplecs INTERFACE include)
add_library(eld::simplecs ALIAS simplecs)

add_library(simplecs_shared SHARED shared_lib.cpp)
target_link_libraries(simplecs_shared PUBLIC simplecs)
target_compile_definitions(simplecs_shared PUBLIC SIMPLESC_SHARED)
add_library(eld::simplecs_shared ALIAS simplecs_shared)

# workaround for MinGW
if (WIN32 AND CMAKE_COMPILER_IS_GNUCC)
    target_link_options(simplecs_shared PRIVATE "-Wl,--exclude-all-symbols")
endif()

if (BUILD_TESTS)
    enable_testing()
    find_package(GTest CONFIG REQUIRED)
    add_subdirectory(test)
endif ()