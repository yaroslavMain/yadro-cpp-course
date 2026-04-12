add_executable(log_call_tests
        log_call.h
        string_utils.h
        log_config.h
        tests.cpp
)

# Linking
target_link_libraries(log_call_tests gtest_main)

# Test
add_test(LogCallTests log_call_tests)
