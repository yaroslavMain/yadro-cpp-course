#include "ti.h"
#include <gtest/gtest.h>

TEST(TITests, SingleValue) {
    const auto root = "42"_ti;
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->value, 42);
    EXPECT_EQ(root->left, nullptr);
    EXPECT_EQ(root->right, nullptr);
}

TEST(TITests, SimpleTree) {
    const auto root = "<1 | 2 | 3>"_ti;

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->value, 2);
    ASSERT_NE(root->left, nullptr);
    EXPECT_EQ(root->left->value, 1);
    ASSERT_NE(root->right, nullptr);
    EXPECT_EQ(root->right->value, 3);
}

TEST(TITests, EmptyBranches) {
    const auto root = "< | 10 | >"_ti;
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->value, 10);
    EXPECT_EQ(root->left, nullptr);
    EXPECT_EQ(root->right, nullptr);
}

TEST(TITests, NestedTree) {
    const auto root = "<<1 | 2 | 3> | 4 | <5 | 6 | 7>>"_ti;

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->value, 4);
    EXPECT_EQ(root->left->value, 2);
    EXPECT_EQ(root->left->left->value, 1);
    EXPECT_EQ(root->right->value, 6);
    EXPECT_EQ(root->right->right->value, 7);
}

TEST(TITests, Whitespaces) {
    const auto root = "  < 1|2 |3  >  "_ti;

    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->value, 2);
    EXPECT_EQ(root->left->value, 1);
    EXPECT_EQ(root->right->value, 3);
}

TEST(TITests, InvalidInput) {
    EXPECT_THROW("abc"_ti, std::invalid_argument);
    EXPECT_THROW("-"_ti, std::invalid_argument);
}
