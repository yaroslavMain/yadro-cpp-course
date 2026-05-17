#pragma once
#include <iostream>
#include <vector>
#include "write_preferred_mutex.h"

template <typename T> class SharedDataSystem {
  std::vector<T> m_data;
  mutable WritePreferredMutex m_mutex;

public:
  SharedDataSystem() = default;
  SharedDataSystem(const SharedDataSystem &) = delete;
  SharedDataSystem &operator=(const SharedDataSystem &) = delete;
  SharedDataSystem(SharedDataSystem &&other) noexcept = delete;
  SharedDataSystem &operator=(SharedDataSystem &&other) noexcept = delete;

  size_t get_size() const {
    ReadGuard guard(m_mutex);
    return m_data.size();
  }

  T get_item_at(const size_t index) const {
    ReadGuard guard(m_mutex);
    if (index < m_data.size()) {
      return m_data[index];
    }
    return T{};
  }

  void create_item(T data) {
    WriteGuard guard(m_mutex);
    m_data.push_back(std::move(data));
  }

  void clear_all() {
    WriteGuard guard(m_mutex);
    m_data.clear();
  }
};
