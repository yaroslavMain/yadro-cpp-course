add_executable(vi_tests
        vi.h
        tests.cpp
)

# Linking
target_link_libraries(vi_tests gtest_main)

# Test
add_test(VectorOfIntsTest vi_tests)
