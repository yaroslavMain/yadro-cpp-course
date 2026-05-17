#pragma once
#include <memory>

template <typename T> struct Task {
  size_t id;
  std::unique_ptr<T> data;

  Task(const size_t t_id, T t_data)
      : id(t_id), data(std::make_unique<T>(std::move(t_data))) {}
};
