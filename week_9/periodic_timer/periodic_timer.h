#pragma once
#include <chrono>
#include <mutex>
#include <thread>
#include <functional>

class PeriodicTimer {
  std::chrono::milliseconds const m_period;
  std::function<void()> m_callback;
  std::mutex m_mutex;
  std::condition_variable m_cv;
  bool m_is_stopping;
  std::thread m_thread;

  void work() {
    std::unique_lock lock(m_mutex);
    while (true) {
      m_cv.wait_for(lock, m_period, [this] { return m_is_stopping; });
      if (m_is_stopping)
        break;

      if (m_callback)
        m_callback();
    }
  }

public:
  PeriodicTimer(std::chrono::milliseconds const period,
                const std::function<void()> &callback)
      : m_period(period), m_callback(callback), m_is_stopping(false),
        m_thread(&PeriodicTimer::work, this) {}

  ~PeriodicTimer() {
    {
      std::lock_guard lock(m_mutex);
      m_is_stopping = true;
    }
    m_cv.notify_one();
    m_thread.join();
  }
};