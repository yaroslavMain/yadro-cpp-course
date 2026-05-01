add_executable(meta_computer
        tests.cpp
        meta_computer.h
        checker.h
        ops.h
        name.h
)

# Linking
target_link_libraries(meta_computer gtest_main)

# Test
add_test(MetaComputerTests meta_computer)
