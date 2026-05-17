#pragma once
#include <condition_variable>
#include <mutex>

class WritePreferredMutex {
  std::mutex m_mutex;
  int m_activate_readers = 0;
  int m_active_writers = 0;
  int m_waiting_writers = 0;
  std::condition_variable m_cv;

public:
  WritePreferredMutex() = default;
  WritePreferredMutex(const WritePreferredMutex &) = delete;
  WritePreferredMutex &operator=(const WritePreferredMutex &) = delete;

  void lock_shared() {
    std::unique_lock lock(m_mutex);
    m_cv.wait(lock, [this] {
      return m_active_writers == 0 && m_waiting_writers == 0;
    });
    m_activate_readers++;
  }

  void unlock_shared() {
    std::unique_lock lock(m_mutex);
    m_activate_readers--;
    if (m_activate_readers == 0) {
      m_cv.notify_all();
    }
  }

  // For writer
  void lock() {
    std::unique_lock lock(m_mutex);
    m_waiting_writers++;
    m_cv.wait(lock, [this] {
      return m_activate_readers == 0 && m_active_writers == 0;
    });
    m_waiting_writers--;
    m_active_writers = 1;
  }

  // For writer
  void unlock() {
    std::unique_lock lock(m_mutex);
    m_active_writers = 0;
    m_cv.notify_all();
  }
};

class WriteGuard {
  WritePreferredMutex &m_mutex;

public:
  explicit WriteGuard(WritePreferredMutex &mutes) : m_mutex(mutes) {
    m_mutex.lock();
  }
  ~WriteGuard() { m_mutex.unlock(); }
};

class ReadGuard {
  WritePreferredMutex &m_mutex;

public:
  explicit ReadGuard(WritePreferredMutex &mutes) : m_mutex(mutes) {
    m_mutex.lock_shared();
  }
  ~ReadGuard() { m_mutex.unlock_shared(); }
};
