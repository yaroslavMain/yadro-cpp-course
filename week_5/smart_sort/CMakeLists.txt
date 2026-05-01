add_executable(smart_sort
        smart_sort.h
        tests.cpp
)

# Linking
target_link_libraries(smart_sort gtest_main)

# Test
add_test(SmartSortTests smart_sort)
