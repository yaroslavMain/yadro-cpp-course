add_executable(action_store
        action_store.h
        tests.cpp
)

# Linking
target_link_libraries(action_store gtest_main)

# Test
add_test(ActionStoreTests action_store)
