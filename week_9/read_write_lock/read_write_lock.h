#pragma once
#include <condition_variable>
#include <mutex>

class ReadWriteLock {
  std::mutex m_mutex;
  std::condition_variable m_cv;

  int m_active_readers = 0;
  int m_active_writers = 0;
  int m_waiting_writers = 0;

public:
  ReadWriteLock() = default;
  ReadWriteLock(const ReadWriteLock &) = delete;
  ReadWriteLock &operator=(const ReadWriteLock &) = delete;

  void reader_lock() {
    std::unique_lock lock(m_mutex);
    m_cv.wait(lock, [this] {
      return m_active_writers == 0 && m_waiting_writers == 0;
    });
    m_active_readers++;
  }

  void reader_unlock() {
    std::unique_lock lock(m_mutex);
    m_active_readers--;
    if (m_active_readers == 0 && m_waiting_writers > 0)
      m_cv.notify_all();
  }

  void writer_lock() {
    std::unique_lock lock(m_mutex);
    m_waiting_writers++;
    m_cv.wait(lock, [this] {
      return m_active_readers == 0 && m_active_writers == 0;
    });
    m_waiting_writers--;
    m_active_writers = 1;
  }

  void writer_unlock() {
    std::unique_lock lock(m_mutex);
    m_active_writers--;
    m_cv.notify_all();
  }
};

class ReadGuard {
  ReadWriteLock &mutex;

public:
  explicit ReadGuard(ReadWriteLock &mutex) : mutex(mutex) {
    mutex.reader_lock();
  }
  ReadGuard(const ReadGuard &) = delete;
  ReadGuard &operator=(const ReadGuard &) = delete;

  ~ReadGuard() { mutex.reader_unlock(); }
};

class WriteGuard {
  ReadWriteLock &mutex;

public:
  explicit WriteGuard(ReadWriteLock &mutex) : mutex(mutex) {
    mutex.writer_lock();
  }
  WriteGuard(const WriteGuard &) = delete;
  WriteGuard &operator=(const WriteGuard &) = delete;

  ~WriteGuard() { mutex.writer_unlock(); }
};
