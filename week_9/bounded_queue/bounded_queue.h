#pragma once
#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

template <typename T> class BoundedQueue {
  std::queue<T> m_queue;
  size_t m_max_capacity;
  std::mutex m_mutex;
  std::condition_variable m_not_full;
  std::condition_variable m_not_empty;

  template <typename U> void push_impl(U &&val) {
    std::unique_lock lock(m_mutex);
    m_not_full.wait(lock, [this] { return m_queue.size() < m_max_capacity; });

    m_queue.push(std::forward<U>(val));
    m_not_empty.notify_one();
  }

  template <typename U> bool non_block_push_impl(U &&val) {
    std::unique_lock lock(m_mutex);
    if (m_queue.size() >= m_max_capacity) {
      return false;
    }

    m_queue.push(std::forward<U>(val));
    m_not_empty.notify_one();
    return true;
  }

public:
  explicit BoundedQueue(const size_t max_capacity)
      : m_max_capacity(max_capacity) {
    if (m_max_capacity <= 0) {
      throw std::invalid_argument("max_capacity must be greater than 0");
    }
  }
  BoundedQueue(const BoundedQueue &other) = delete;
  BoundedQueue &operator=(const BoundedQueue &other) = delete;
  BoundedQueue(BoundedQueue &&other) = delete;
  BoundedQueue &operator=(BoundedQueue &&other) = delete;

  // Blocking ops
  void push(const T &val) { push_impl(val); }

  void push(T &&val) { push_impl(std::move(val)); }

  T pop() {
    std::unique_lock lock(m_mutex);
    m_not_empty.wait(lock, [this] { return !m_queue.empty(); });

    auto val = std::move(m_queue.front());
    m_queue.pop();
    m_not_full.notify_one();

    return val;
  }

  // Non-blocking ops
  bool try_push(const T &val) { return non_block_push_impl(val); }

  bool try_push(T &&val) { return non_block_push_impl(std::move(val)); }

  std::optional<T> try_pop() {
    std::unique_lock lock(m_mutex);
    if (m_queue.empty()) {
      return std::nullopt;
    }

    auto val = std::move(m_queue.front());
    m_queue.pop();
    m_not_full.notify_one();

    return val;
  }
};