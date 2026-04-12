#include <gtest/gtest.h>
#include "src/rpn.h"

TEST(RPNTests, BasicOperations) {
    EXPECT_EQ("10 5 +"_rpn, 15);
    EXPECT_EQ("10 5 - "_rpn, 5);
    EXPECT_EQ("10 5 *"_rpn, 50);
    EXPECT_EQ("10 5 /"_rpn, 2);
    EXPECT_EQ("2 3 ^"_rpn, 8);
    EXPECT_EQ("1 10 %"_rpn, 1);
}

TEST(RPNTests, NegativeNumbers) {
    EXPECT_EQ("-10 5 +"_rpn, -5);
    EXPECT_EQ("-10 -5 *"_rpn, 50);
    EXPECT_EQ("0 10 - "_rpn, -10);
    EXPECT_EQ("-2 2 ^"_rpn, 4);
}


TEST(RPNTests, ComplexChains) {
    EXPECT_EQ("10 2 + 3 * 2 2 ^ /"_rpn, 9);
    EXPECT_EQ("100 10 % 5 +"_rpn, 5);
}

TEST(RPNTests, EdgeCases) {
    EXPECT_EQ("0"_rpn, 0);
    EXPECT_EQ("   42     "_rpn, 42);
    EXPECT_EQ("5 5 5 * *"_rpn, 125);
}

TEST(RPNTests, Exceptions) {
    EXPECT_ANY_THROW("10 0 /"_rpn);
    EXPECT_ANY_THROW("5 +"_rpn);
    EXPECT_ANY_THROW("+ 5 5"_rpn);
    EXPECT_ANY_THROW("10 20 30 +"_rpn);
    EXPECT_ANY_THROW("10 5 @"_rpn);
}

TEST(RPNTests, CompileTime) {
    static_assert("2 2 2 * *"_rpn == 8);
    static_assert("10 5 %"_rpn == 0);
    static_assert("-1 -1 *"_rpn == 1);
}
