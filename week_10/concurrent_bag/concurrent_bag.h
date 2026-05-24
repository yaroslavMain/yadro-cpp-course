#pragma once
#include <atomic>
#include <cassert>
#include <stdexcept>

template <typename T, size_t N> class ConcurrentBag {
  std::atomic<size_t> m_count;
  std::atomic<bool> m_flag[N];
  alignas(64) T m_data[N];

public:
  ConcurrentBag() {
    for (size_t i = 0; i < N; i++)
      m_flag[i].store(false);
  }

  size_t put(T value) {
    if (const size_t n = m_count.fetch_add(1, std::memory_order_acquire);
        n >= N) {
      m_count.fetch_sub(1, std::memory_order_release);
      throw std::overflow_error("ConcurrentBag overflow");
    }

    for (size_t i = 0; i < N; i++) {
      if (bool expected = false;
          m_flag[i].compare_exchange_strong(expected, true)) {
        m_data[i] = std::move(value);
        return i;
      }
    }
    assert(false && "ConcurrentBag overflow");
  }

  T get(const size_t index) {
    if (index >= N) {
      throw std::out_of_range("ConcurrentBag out of range");
    }

    T value = m_data[index];
    if (bool expected = true;
        !m_flag[index].compare_exchange_strong(expected, false)) {
      throw std::runtime_error("Cell is not occupied");
    }

    m_count.fetch_sub(1, std::memory_order_release);
    return value;
  }
};
