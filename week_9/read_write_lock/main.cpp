#include <cassert>
#include <iostream>
#include <thread>
#include <vector>

#include "read_write_lock.h"

int main() {
  ReadWriteLock rw_lock;
  int shared_resource = 0;
  constexpr int num_iterations = 10000;

  auto reader_func = [&] {
    for (int i = 0; i < num_iterations; ++i) {
      ReadGuard guard(rw_lock);
      int value = shared_resource;
      std::this_thread::yield();
    }
  };

  auto writer_func = [&]() {
    for (int i = 0; i < num_iterations; ++i) {
      WriteGuard guard(rw_lock);
      shared_resource++;
      std::this_thread::yield();
    }
  };

  std::vector<std::thread> threads; threads.reserve(8);
  for (int i = 0; i < 8; ++i)
    threads.emplace_back(reader_func);
  for (int i = 0; i < 4; ++i)
    threads.emplace_back(writer_func);

  for (auto &t : threads)
    t.join();

  std::cout << "All done. Final value: " << shared_resource << std::endl;
  assert(shared_resource == 4 * num_iterations);

  return 0;
}
