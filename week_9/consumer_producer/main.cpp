#include <iostream>
#include <thread>

#include "safe_queue.h"

void producer(SafeQueue<int> &queue, const int n_items, const int start = 42) {
  for (int i = 0; i < n_items; i++) {
    const int el = start * 100 + i;
    queue.push(el);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void consumer(const int id, SafeQueue<int> &queue) {
  int val;
  while (queue.pop(val)) {
    std::cout << "[Consumer " << id << "] processed: " << val << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

int main() {
  SafeQueue<int> queue;

  constexpr int num_producers = 3;
  constexpr int items_per_producer = 5;
  constexpr int num_consumers = 2;

  std::vector<std::thread> producers; std::vector<std::thread> consumers;
  producers.reserve(num_producers); consumers.reserve(num_consumers);

  for (int i = 0; i < num_producers; i++) {
    producers.emplace_back(producer, std::ref(queue), items_per_producer,
                           42 + i);
  }
  for (int i = 0; i < num_consumers; i++) {
    consumers.emplace_back(consumer, i + 1, std::ref(queue));
  }

  for (auto &producer : producers)
    if (producer.joinable())
      producer.join();

  queue.shutdown();
  for (auto &consumer : consumers)
    if (consumer.joinable())
      consumer.join();

  std::cout << "All work done!" << std::endl;

  return 0;
}