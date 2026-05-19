#include <atomic>
#include <chrono>
#include <gtest/gtest.h>
#include <memory>
#include <thread>
#include <vector>

#include "bounded_queue.h"

TEST(BoundedQueueTests, SingleThreadedNonBlockingOps) {
  BoundedQueue<int> queue(2);

  EXPECT_TRUE(queue.try_push(1));
  EXPECT_TRUE(queue.try_push(2));
  EXPECT_FALSE(queue.try_push(3));

  auto val1 = queue.try_pop();
  ASSERT_TRUE(val1.has_value());
  EXPECT_EQ(*val1, 1);

  auto val2 = queue.try_pop();
  ASSERT_TRUE(val2.has_value());
  EXPECT_EQ(*val2, 2);

  auto val3 = queue.try_pop();
  EXPECT_FALSE(val3.has_value());
}

TEST(BoundedQueueTests, InvalidCapacityThrows) {
  EXPECT_THROW(BoundedQueue<int> queue(0), std::invalid_argument);
}

TEST(BoundedQueueTests, PushBlocksWhenFull) {
  BoundedQueue<int> queue(1);
  queue.push(10);

  std::atomic consumer_ran(false);

  std::thread consumer([&queue, &consumer_ran]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    consumer_ran = true;
    EXPECT_EQ(queue.pop(), 10);
  });

  auto start = std::chrono::steady_clock::now();

  queue.push(20);

  auto end = std::chrono::steady_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  EXPECT_TRUE(consumer_ran);
  EXPECT_GE(elapsed, 90);
  EXPECT_EQ(queue.pop(), 20);

  consumer.join();
}

TEST(BoundedQueueTests, PopBlocksWhenEmpty) {
  BoundedQueue<int> queue(2);
  std::atomic<bool> producer_ran(false);

  std::thread producer([&queue, &producer_ran]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    producer_ran = true;
    queue.push(100);
  });

  auto start = std::chrono::steady_clock::now();
  int val = queue.pop();

  auto end = std::chrono::steady_clock::now();
  auto elapsed =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  EXPECT_TRUE(producer_ran);
  EXPECT_GE(elapsed, 90);
  EXPECT_EQ(val, 100);

  producer.join();
}

TEST(BoundedQueueTests, MoveOnlyTypesSupport) {
  BoundedQueue<std::unique_ptr<int>> queue(2);

  auto ptr1 = std::make_unique<int>(42);
  EXPECT_TRUE(queue.try_push(std::move(ptr1)));

  queue.push(std::make_unique<int>(84));

  auto res1 = queue.pop();
  ASSERT_NE(res1, nullptr);
  EXPECT_EQ(*res1, 42);

  auto res2 = queue.try_pop();
  ASSERT_TRUE(res2.has_value());
  ASSERT_NE(*res2, nullptr);
  EXPECT_EQ(**res2, 84);
}

TEST(BoundedQueueTests, MultiProducerMultiConsumerStressTest) {
  BoundedQueue<int> queue(10);

  constexpr int num_producers = 4;
  constexpr int num_consumers = 4;
  constexpr int items_per_producer = 1000;

  std::atomic total_consumed_sum(0);
  std::vector<std::thread> threads;

  threads.reserve(num_consumers);
  for (int i = 0; i < num_consumers; ++i) {
    threads.emplace_back([&queue, &total_consumed_sum, items_per_producer]() {
      int local_sum = 0;
      for (int j = 0; j < items_per_producer; ++j) {
        local_sum += queue.pop();
      }
      total_consumed_sum += local_sum;
    });
  }

  for (int i = 0; i < num_producers; ++i) {
    threads.emplace_back([&queue, items_per_producer]() {
      for (int j = 0; j < items_per_producer; ++j) {
        queue.push(1);
      }
    });
  }

  for (auto &th : threads) {
    if (th.joinable()) {
      th.join();
    }
  }

  EXPECT_EQ(total_consumed_sum.load(), num_producers * items_per_producer);
  EXPECT_TRUE(queue.try_pop() == std::nullopt);
}
