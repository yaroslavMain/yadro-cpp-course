add_executable(any_func
        any.h
        tests.cpp
)

# Linking
target_link_libraries(any_func gtest_main)

# Test
add_test(AnyFuncTests any_func)
