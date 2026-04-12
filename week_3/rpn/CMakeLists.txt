add_executable(rpn_tests
        tests.cpp
        src/config.h
        src/lexer.h
        src/rpn.h
)

# Linking
target_link_libraries(rpn_tests gtest_main)

# Test
add_test(RPNTests rpn_tests)
