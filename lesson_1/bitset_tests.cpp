#include <gtest/gtest.h>
#include "src/bitset.h"

TEST(BitsetTests, Constructors) {
    const bitset bs1;
    EXPECT_TRUE(bs1.empty());

    bitset bs2(100);
    EXPECT_TRUE(bs2.empty());

    bs2.set(12, true);
    EXPECT_EQ(bs2.size(), 64 * 2);
}


TEST(BitsetTests, SetAndTest) {
    bitset bs(100);

    bs.set(10, false);
    bs.set(65, true);

    for (size_t i = 1; i < 64; i++) {
        EXPECT_FALSE(bs.test(i));
        EXPECT_FALSE(bs[i]);
    }

    EXPECT_FALSE(bs.test(10));
    EXPECT_TRUE(bs.test(65));
    EXPECT_FALSE(bs.test(200));
    EXPECT_FALSE(bs.test(101));
}


TEST(BitsetTests, CopyConstructor) {
    bitset bs1(64);
    bs1.set(10, true);

    bitset bs2 = bs1;
    bs2.set(10, false);

    EXPECT_TRUE(bs1.test(10));
    EXPECT_FALSE(bs2.test(10));
}

TEST(BitsetTests, CopyAssignmentOperator) {
    bitset bs1(64);
    bs1.set(5, true);

    bitset bs2(128);
    bs2 = bs1;

    EXPECT_EQ(bs2.size(), bs1.size());
    EXPECT_TRUE(bs2.test(5));
}

TEST(BitsetTests, MoveConstructor) {
    bitset bs1(64);
    bs1.set(5, true);

    bitset bs2(std::move(bs1));

    EXPECT_EQ(bs2.size(), 64);
    EXPECT_TRUE(bs2.test(5));
    EXPECT_TRUE(bs1.empty());
    EXPECT_FALSE(bs2.test(10));
}


TEST(BitsetTests, MoveAssigentOperator) {
    bitset bs1(64);
    bs1.set(5, true);

    bitset bs2(128);
    bs2 = std::move(bs1);

    EXPECT_EQ(bs2.size(), 64);
    EXPECT_TRUE(bs2.test(5));
    EXPECT_EQ(bs1.size(), 0);
    EXPECT_FALSE(bs2.test(10));
}

TEST(BitsetTests, SelfAssignment) {
    bitset bs(64);
    bs.set(10, true);
    bs = bs;
    EXPECT_TRUE(bs.test(10));

    bs = std::move(bs);
    EXPECT_TRUE(bs.test(10));
}


TEST(BitsetTests, Operations) {
    bitset bs1(64);

    // Empty
    EXPECT_TRUE(bs1.empty());
    bs1.set(5, true);
    EXPECT_TRUE(bs1.test(5));
    EXPECT_FALSE(bs1.empty());

    // Size
    EXPECT_EQ(bs1.size(), 64);

    // Clear
    bs1.set(10, true);
    EXPECT_FALSE(bs1.empty());
    bs1.clear();
    EXPECT_TRUE(bs1.empty());
}


TEST(BitsetTests, UnionWithDifferentSizes) {
    bitset small(64);
    small.set(10, true);

    bitset big(128);
    big.set(100, true);

    const bitset res1 = small.union_with(big);
    EXPECT_EQ(res1.size(), 64 * 2);
    EXPECT_TRUE(res1.test(10));
    EXPECT_TRUE(res1.test(100));

    const bitset res2 = big.union_with(small);
    EXPECT_EQ(res2.size(), 128);
    EXPECT_TRUE(res2.test(10));
    EXPECT_TRUE(res2.test(100));
}

TEST(BitsetTests, IntersectionBasic) {
    bitset b1(128);
    b1.set(10, true);
    b1.set(100, true);

    bitset b2(128);
    b2.set(10, true);
    b2.set(50, true);

    bitset res = b1.intersection(b2);
    EXPECT_TRUE(res.test(10));
    EXPECT_FALSE(res.test(100));
    EXPECT_FALSE(res.test(50));
}

TEST(BitsetTests, IntersectionEmptyAssert) {
    bitset b1(64);
    bitset b2(64);

    bitset res = b1.intersection(b2);
    EXPECT_TRUE(res.empty());
}


TEST(BitsetTests, IsSubset) {
    bitset parent(128);
    parent.set(10, true);
    parent.set(100, true);

    bitset child(64);
    child.set(10, true);

    EXPECT_TRUE(child.is_subset(parent));

    child.set(20, true);
    EXPECT_FALSE(child.is_subset(parent));

    bitset big_child(256);
    big_child.set(10, true);
    big_child.set(200, true);
    EXPECT_FALSE(big_child.is_subset(parent));
}
