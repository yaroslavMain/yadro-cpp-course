#include "vi.h"
#include <gtest/gtest.h>

TEST(VectorOfIntsTest, Simple) {
    const auto vec1 = "1 2 3 _ 4._5"_vi;
    EXPECT_EQ(vec1.size(), 5);
    EXPECT_EQ(vec1, std::vector({1, 2, 3, 4, 5}));

    const auto vec2 = "       ."_vi;
    EXPECT_EQ(vec2.size(), 0);
}


TEST(VectorOfIntsTest, LimitsAndErrors) {
    EXPECT_THROW("100000000000"_vi, std::out_of_range);
    EXPECT_THROW("abc"_vi, std::invalid_argument);
    EXPECT_THROW("123a"_vi, std::invalid_argument);
}


TEST(VectorOfIntsTest, DirtyStrings) {
    const auto vec = ";;;1,,,,2====3   "_vi;
    EXPECT_EQ(vec, std::vector({1, 2, 3}));
    EXPECT_TRUE(""_vi.empty());
    EXPECT_THROW(" ,;=-_ "_vi, std::invalid_argument);
}

TEST(VectorOfIntsTest, NegativeNumbers) {
    const auto vec = "10:-20_-30.40"_vi;
    EXPECT_EQ(vec, std::vector({10, -20, -30, 40}));

    const auto vec2 = "1 -2_-3 _ 4._5"_vi;
    EXPECT_EQ(vec2.size(), 5);
    EXPECT_EQ(vec2, std::vector({1, -2, -3, 4, 5}));
}
