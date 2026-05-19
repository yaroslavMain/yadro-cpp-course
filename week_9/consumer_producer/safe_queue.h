#pragma once
#include <mutex>
#include <queue>
#include <condition_variable>

template <typename T> class SafeQueue {
  std::queue<T> m_queue;
  std::mutex m_mutex;
  std::condition_variable m_cv;
  bool finished;

public:
  SafeQueue() : finished(false) {}

  void push(T val) {
    {
      std::lock_guard lock(m_mutex);
      m_queue.push(val);
    }
    m_cv.notify_one();
  }

  bool pop(T &val) {
    std::unique_lock lock(m_mutex);
    m_cv.wait(lock, [this] { return !m_queue.empty() || finished; });

    if (m_queue.empty() && finished) {
      return false;
    }

    val = m_queue.front();
    m_queue.pop();
    return true;
  }

  void shutdown() {
    {
      std::lock_guard lock(m_mutex);
      finished = true;
    }
    m_cv.notify_all();
  }
};