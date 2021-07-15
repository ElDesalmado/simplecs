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

message(STATUS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

add_executable(test.id_pool test_id_pool.cpp)
target_link_libraries(test.id_pool simplecs GTest::gtest)
add_test(NAME test.id_pool
        COMMAND test.id_pool)
