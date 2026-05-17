#include "task.h"
#include "task_system.h"
#include <iostream>
#include <mutex>

struct Data {
  std::string name;
  size_t age;
};

std::mutex cout_mutex; // only for education reasons

void worker(const std::unique_ptr<Task<Data>> task) {
  {
    std::lock_guard lock(cout_mutex);
    std::cout << "[Worker " << std::this_thread::get_id() << "] started task #"
              << task->id << std::endl;
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  {
    std::lock_guard lock(cout_mutex);
    std::cout << "[Worker " << std::this_thread::get_id() << "] FINISHED task #"
              << task->id << " (Name: " << task->data->name << ")" << std::endl;
  }
}

int main() {
  TaskSystem<Data> tasks_system;

  tasks_system.add_consumer(worker);
  tasks_system.add_consumer(worker);
  tasks_system.add_consumer(worker);

  for (size_t i = 0; i < 10; i++) {
    tasks_system.push_tasks(i, {"Yaroslav T.", i});
  }

  {
    std::lock_guard lock(cout_mutex);
    std::cout << "[Main] All tasks added, waiting for workers to finish..."
              << std::endl;
  }
}
