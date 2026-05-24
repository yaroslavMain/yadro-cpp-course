#include <iostream>
#include <gtest/gtest.h>
#include "ring_buffer.h"

TEST(RingBufferTests, OverflowAndUnderflow) {
  RingBuffer<int, 2> ring_buffer{};
  int data = 0;

  EXPECT_FALSE(ring_buffer.try_pop(data));

  EXPECT_TRUE(ring_buffer.try_push(100));
  EXPECT_TRUE(ring_buffer.try_push(200));
  EXPECT_FALSE(ring_buffer.try_push(300));

  EXPECT_TRUE(ring_buffer.try_pop(data));
  EXPECT_EQ(data, 100);
  EXPECT_TRUE(ring_buffer.try_pop(data));
  EXPECT_EQ(data, 200);

  EXPECT_FALSE(ring_buffer.try_pop(data));
}

TEST(RingBufferTests, CyclingUsage) {
  RingBuffer<int, 4> ring_buffer{};
  int data = 0;

  for (int i = 0; i < 10; ++i) {
    EXPECT_TRUE(ring_buffer.try_push(i));
    EXPECT_TRUE(ring_buffer.try_push(i + 10));

    EXPECT_TRUE(ring_buffer.try_pop(data));
    EXPECT_EQ(data, i);

    EXPECT_TRUE(ring_buffer.try_pop(data));
    EXPECT_EQ(data, i + 10);
  }

  EXPECT_FALSE(ring_buffer.try_pop(data));
}
