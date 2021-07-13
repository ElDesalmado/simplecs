add_executable(sandbox sandbox.cpp)
target_link_libraries(sandbox registry)

add_executable(test.selector test_selector.cpp)
target_link_libraries(test.selector registry GTest::gtest)

add_test(NAME test.selector
        COMMAND test.selector)
