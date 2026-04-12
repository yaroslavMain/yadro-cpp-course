add_executable(bitset_tests
        bitset_tests.cpp
        src/bitset.cpp
        src/bitset.h
)

# Linking
target_link_libraries(bitset_tests gtest_main)

# Test
add_test(BitsetTests bitset_tests)
