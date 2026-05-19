add_executable(bounded_queue
        bounded_queue.h
        tests.cpp
)

# Linking
target_link_libraries(bounded_queue gtest_main)

# Test
add_test(BoundedQueueTests bounded_queue)
