# hide exported symbols by default

# this does not work with MinGW for some reason
if (MSVC)
elseif(CMAKE_COMPILER_IS_GNUCC)
    message(STATUS "Export symbols are hidden by default")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
endif()
