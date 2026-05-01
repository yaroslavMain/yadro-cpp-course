#pragma once
#include <vector>

using uint = unsigned int;

constexpr static uint BIG_ELEMENT_SIZE = 16;
template <typename T> struct is_heavy {
  static constexpr bool value = sizeof(T) > BIG_ELEMENT_SIZE;
};

template <typename T> inline constexpr bool is_heavy_v = is_heavy<T>::value;

namespace algo {

template <typename T> void fast_sort(std::vector<T> &data) {
  std::sort(data.begin(), data.end());
}

template <typename T> void bubble_sort(std::vector<T> &data) {
  for (size_t i = 0; i < data.size(); i++)
    for (size_t j = 0; j < data.size() - i - 1; j++)
      if (data[j] > data[j + 1])
        std::swap(data[j], data[j + 1]);
}

} // namespace algo

template <typename T> void smart_sort(std::vector<T> &data) {
  if constexpr (is_heavy_v<T> || !std::is_arithmetic_v<T>) {
    algo::bubble_sort(data);
  } else {
    algo::fast_sort(data);
  }
}