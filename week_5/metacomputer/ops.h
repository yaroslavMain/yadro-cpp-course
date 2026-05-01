#pragma once

template <typename T, T val> struct num {
  static constexpr T value = val;
};

// Ops
struct op_add {};
struct op_sub {};
struct op_mul {};
struct op_div {};
struct op_neg {};
struct open_s {};  // open_sign
struct close_s {}; // close_sign

template <typename...> struct list {};

template <typename Value, typename RemainingTokens> struct result_pack {
  using result = Value;
  using tail = RemainingTokens;
};