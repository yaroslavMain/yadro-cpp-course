add_executable(document
        document.h
        objs.h
        tests.cpp
)

# Linking
target_link_libraries(document gtest_main)

# Test
add_test(DocumentTests document)
