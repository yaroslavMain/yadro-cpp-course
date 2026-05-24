add_executable(concurrent_bag_tests
        tests.cpp
        concurrent_bag.h
)

# Linking
target_link_libraries(concurrent_bag_tests gtest_main)

# Test
add_test(ConcurrentBagTests concurrent_bag_tests)
