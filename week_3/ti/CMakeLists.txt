add_executable(ti_tests
        tests.cpp
        ti.h
)

# Linking
target_link_libraries(ti_tests gtest_main)

# Test
add_test(TITests ti_tests)
