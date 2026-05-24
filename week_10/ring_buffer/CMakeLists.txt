add_executable(ring_buffer_tests
        tests.cpp
        ring_buffer.h
)

# Linking
target_link_libraries(ring_buffer_tests gtest_main)

# Test
add_test(RingBufferTests ring_buffer_tests)
