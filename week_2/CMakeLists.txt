add_executable(buffering_writer_tests
        tests.cpp
        src/buffering_writer.h
        src/buffering_writer.cpp
        src/record.h
)

# Linking
target_link_libraries(buffering_writer_tests gtest_main)

# Test
add_test(BufferingWriteTests buffering_writer_tests)
