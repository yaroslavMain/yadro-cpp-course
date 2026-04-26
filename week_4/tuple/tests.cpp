#include "tuple.h"
#include <gtest/gtest.h>
#include <string>

TEST(TupleTests, BasicAccess) {
  auto tuple = MakeTuple(123, "string", 'c');

  EXPECT_EQ(get<0>(tuple), 123);
  EXPECT_STREQ(get<1>(tuple), "string");
  EXPECT_EQ(get<2>(tuple), 'c');
}

TEST(TupleTests, Modification) {
  auto tuple = MakeTuple(1, 2.5);

  get<0>(tuple) = 42;
  get<1>(tuple) = 10.5;

  EXPECT_EQ(get<0>(tuple), 42);
  EXPECT_DOUBLE_EQ(get<1>(tuple), 10.5);
}

TEST(TupleTests, StdStringSupport) {
  const std::string s = "long_string_test";
  auto tuple = MakeTuple(s, 100);

  EXPECT_EQ(get<0>(tuple), "long_string_test");
  get<0>(tuple) = "changed";
  EXPECT_EQ(get<0>(tuple), "changed");
}

TEST(TupleTests, SingleElement) {
  auto tuple = MakeTuple(999);
  EXPECT_EQ(get<0>(tuple), 999);
}

TEST(TupleTests, NestedTuples) {
  const auto inner = MakeTuple(10, 20);
  auto outer = MakeTuple(inner, 30);

  EXPECT_EQ(get<0>(get<0>(outer)), 10);
  EXPECT_EQ(get<1>(outer), 30);
}
