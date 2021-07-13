cmake_minimum_required(VERSION 3.7.2 FATAL_ERROR)
project(simplesc VERSION 0.0.1 LANGUAGES CXX C)

message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "System name: ${CMAKE_SYSTEM_NAME}")

include(cmake/clean_build.cmake)
include(cmake/configure_tests.cmake)
include(cmake/export_visibility.cmake)

# TODO: make configurable
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY lib)

# TODO: dynamic and header only versions
add_library(simplesc INTERFACE)
target_include_directories(simplesc INTERFACE include)
add_library(eld::simplesc ALIAS simplesc)

add_library(simplesc_shared SHARED shared_lib.cpp)
target_link_libraries(simplesc_shared PUBLIC simplesc)
target_compile_definitions(simplesc_shared PUBLIC SIMPLESC_SHARED)
add_library(eld::simplesc_shared ALIAS simplesc_shared)

# workaround for MinGW
if (CMAKE_COMPILER_IS_GNUCC)
    target_link_options(simplesc_shared PRIVATE "-Wl,--exclude-all-symbols")
endif()

if (BUILD_TESTS)
    enable_testing()
    find_package(GTest CONFIG REQUIRED)
    add_subdirectory(test)
endif ()