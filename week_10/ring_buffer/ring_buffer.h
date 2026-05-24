#pragma once
#include <atomic>

template <typename T, size_t N> class RingBuffer {
  static_assert(N > 0, "N must be positive");
  static_assert((N & (N - 1)) == 0, "N must be a power of 2");

  std::atomic<size_t> m_head;
  std::atomic<size_t> m_tail;
  T m_data[N];

public:
  RingBuffer() = default;

  bool try_push(T data) {
    while (true) {
      size_t head = m_head.load(std::memory_order_relaxed);
      if (const size_t tail = m_tail.load(std::memory_order_acquire);
          head - tail >= N) {
        return false;
      }

      if (m_head.compare_exchange_weak(head, head + 1)) {
        m_data[head & (N - 1)] = std::move(data);
        return true;
      }
    }
  }

  bool try_pop(T &data) {
    while (true) {
      const size_t head = m_head.load(std::memory_order_relaxed);
      size_t tail = m_tail.load(std::memory_order_acquire);
      if (tail >= head) {
        return false;
      }

      if (m_tail.compare_exchange_weak(tail, tail + 1)) {
        data = std::move(m_data[tail & (N - 1)]);
        return true;
      }
    }
  }
};
