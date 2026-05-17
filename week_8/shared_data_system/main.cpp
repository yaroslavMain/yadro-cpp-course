#include "data_system.h"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

void reader_worker(const SharedDataSystem<std::string> &system, int id) {
  for (int i = 0; i < 15; ++i) {
    if (const size_t size = system.get_size(); size > 0) {
      std::string item = system.get_item_at(size - 1);
      std::cout << "[Reader " << id << "] read: " << item << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(150));
  }
}

void writer_worker(SharedDataSystem<std::string> &system) {
  std::vector<std::string> fruits = {"Apple", "Banana", "Cherry", "Date"};

  for (const auto &fruit : fruits) {
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
    std::cout << "[Writer] WANTS TO WRITE: " << fruit << "..." << std::endl;
    system.create_item(fruit);
    std::cout << "[Writer] SUCCESSFULLY WRITTEN: " << fruit << std::endl;
  }
}

int main() {
  SharedDataSystem<std::string> data_system;
  std::vector<std::thread> readers;

  for (int i = 1; i <= 5; ++i) {
    readers.emplace_back(reader_worker, std::ref(data_system), i);
  }

  std::thread writer(writer_worker, std::ref(data_system));
  writer.join();
  std::this_thread::sleep_for(std::chrono::milliseconds(300));

  for (auto &reader : readers) {
    if (reader.joinable()) {
      reader.join();
    }
  }
}
