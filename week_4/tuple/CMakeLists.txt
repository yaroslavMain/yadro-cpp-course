add_executable(tuple_tests
        tuple.h
        tests.cpp
)

# Linking
target_link_libraries(tuple_tests gtest_main)

# Test
add_test(TupleTests tuple_tests)
