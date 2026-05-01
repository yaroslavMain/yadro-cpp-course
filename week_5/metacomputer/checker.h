#pragma once

#include "meta_computer.h"

template <typename T, typename = void> struct has_result : std::false_type {};

template <typename T>
struct has_result<T, std::void_t<typename T::out::result>> : std::true_type {};

template <typename... Tokens> struct is_valid_expression {
  using test = calc_expr<list<Tokens...>>;

  static constexpr bool value = []() {
    if constexpr (has_result<test>::value) {
      using tail = typename test::out::tail;
      return std::is_same_v<tail, list<>>;
    } else {
      return false;
    }
  }();
};

template <typename... Tokens>
inline constexpr bool is_valid_expression_v =
    is_valid_expression<Tokens...>::value;

template <typename... Tokens>
struct is_valid_expression<expr<Tokens...>> : is_valid_expression<Tokens...> {};
