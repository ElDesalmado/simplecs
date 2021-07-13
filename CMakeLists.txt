cmake_minimum_required(VERSION 3.7.2 FATAL_ERROR)
project(registry VERSION 0.0.1 LANGUAGES CXX C)

message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "System name: ${CMAKE_SYSTEM_NAME}")

include(cmake/clean_build.cmake)
include(cmake/configure_tests.cmake)

# TODO: make configurable
set(CMAKE_CXX_STANDARD 17)

# TODO: dynamic and header only versions
add_library(registry SHARED c_core.cpp)
target_include_directories(registry PUBLIC include)

if(BUILD_TESTS)
    enable_testing()
    find_package(GTest CONFIG REQUIRED)
    add_subdirectory(test)
endif()