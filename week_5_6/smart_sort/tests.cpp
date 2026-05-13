#include <gtest/gtest.h>
#include <iostream>
#include <vector>

#include "./smart_sort.h"

struct HeavyObject {
  int id;
  double matrix[100];

  bool operator>(const HeavyObject &other) const { return id > other.id; }
  bool operator<(const HeavyObject &other) const { return id < other.id; }
};

TEST(SmartSortTests, BasicTests) {
  std::vector data = {12, 15, 12, 4534, 234523, 23, 1290, 3, 1, 10};
  const std::vector data_sorted = {1,  3,  10,   12,   12,
                                   15, 23, 1290, 4534, 234523};

  smart_sort(data);
  ASSERT_EQ(data, data_sorted);
}

TEST(SmartSortTests, HeavyObjectTest) {
  std::vector<HeavyObject> data = {{10, {0}}, {1, {0}}, {5, {0}}};
  smart_sort(data);

  ASSERT_EQ(data[0].id, 1);
  ASSERT_EQ(data[1].id, 5);
  ASSERT_EQ(data[2].id, 10);
}

TEST(SmartSortTests, EdgeCases) {
  std::vector<int> empty_vec;
  smart_sort(empty_vec);
  ASSERT_TRUE(empty_vec.empty());

  std::vector single_vec = {42};
  smart_sort(single_vec);
  ASSERT_EQ(single_vec[0], 42);
}

TEST(SmartSortTests, CharAndBool) {
  std::vector chars = {'d', 'a', 'c', 'b'};
  smart_sort(chars);
  const std::vector expected = {'a', 'b', 'c', 'd'};
  ASSERT_EQ(chars, expected);

  std::vector bools = {true, false, true};
  smart_sort(bools);
  ASSERT_FALSE(bools[0]);
}

TEST(TraitTests, LogicCheck) {
  EXPECT_FALSE(is_heavy_v<int>);
  EXPECT_TRUE(is_heavy_v<HeavyObject>);
  EXPECT_FALSE(!std::is_arithmetic_v<float>);
  EXPECT_TRUE(!std::is_arithmetic_v<std::string>);
}