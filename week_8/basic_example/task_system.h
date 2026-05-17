#pragma once
#include "task.h"
#include <mutex>
#include <queue>
#include <thread>

template <typename T> class TaskSystem {
  std::queue<std::unique_ptr<Task<T>>> m_queue;
  std::mutex m_mutex;
  std::condition_variable m_cv;
  bool m_is_working;
  std::vector<std::thread> m_threads;

  template <typename F> void worker_thread(F func) {
    while (true) {
      std::unique_ptr<Task<T>> task;
      {
        std::unique_lock lock(m_mutex);
        m_cv.wait(lock, [this] { return !m_is_working || !m_queue.empty(); });
        if (!m_is_working && m_queue.empty()) {
          return;
        }

        task = std::move(m_queue.front());
        m_queue.pop();
      }
      if (task) {
        func(std::move(task));
      }
    }
  }

public:
  TaskSystem() : m_is_working(true) {}
  ~TaskSystem() { stop_workers(); }

  void push_tasks(size_t id, T data) {
    auto task = std::make_unique<Task<T>>(id, data);
    {
      std::lock_guard lock(m_mutex);
      m_queue.push(std::move(task));
    }
    m_cv.notify_one();
  }

  template <typename F> void add_consumer(F &&func) {
    std::lock_guard lock(m_mutex);
    m_threads.emplace_back([this, f = std::forward<F>(func)] mutable {
      this->worker_thread(std::move(f));
    });
  }

  void stop_workers() {
    {
      std::lock_guard lock(m_mutex);
      if (!m_is_working)
        return;
      m_is_working = false;
    }
    m_cv.notify_all();

    for (auto &t : m_threads) {
      if (t.joinable()) {
        t.join();
      }
    }
    m_threads.clear();
  }
};
