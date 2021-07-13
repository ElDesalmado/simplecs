add_executable(sandbox sandbox.cpp)
target_link_libraries(sandbox simplesc)

add_executable(test.selector test_selector.cpp)
target_link_libraries(test.selector simplesc GTest::gtest)
add_test(NAME test.selector
        COMMAND test.selector)

add_executable(test.c_api test_c_api.cpp)
target_link_libraries(test.c_api eld::simplesc_shared GTest::gtest)
add_test(NAME test.c_api
        COMMAND test.c_api
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

message(STATUS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
