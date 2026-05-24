#include <gtest/gtest.h>
#include <iostream>
#include <thread>
#include <vector>

#include "concurrent_bag.h"

TEST(ConcurrentBagTests, Simple) {
  ConcurrentBag<int, 10> bag{};

  for (int i = 0; i < 10; i++)
    ASSERT_EQ(bag.put(i), i);

  for (int i = 0; i < 10; i++)
    ASSERT_EQ(bag.get(i), i);
}

TEST(ConcurrentBagTests, Overflow) {
  ConcurrentBag<int, 5> bag{};
  for (int i = 0; i < 5; i++) {
    ASSERT_EQ(bag.put(i), i);
  }
  ASSERT_THROW(bag.put(100), std::overflow_error);
}

TEST(ConcurrentBagTests, GetNonExistent) {
  ConcurrentBag<int, 10> bag{};
  bag.put(5);
  ASSERT_THROW(bag.get(99), std::out_of_range);
}
