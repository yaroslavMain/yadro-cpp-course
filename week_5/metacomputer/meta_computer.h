#pragma once

#include "name.h"
#include "ops.h"

template <typename Tokens, typename = void> struct calc_expr;
template <typename Tokens, typename = void> struct call_term;

// Helpers
template <typename T, typename = void> struct get_out {};
template <typename T> struct get_out<T, std::void_t<typename T::out>> {
  using type = typename T::out;
};
template <typename T> struct pop_close_s;
template <typename... Tail> struct pop_close_s<list<close_s, Tail...>> {
  using type = list<Tail...>;
};

// 3rd level: factor
template <typename Tokens, typename = void> struct calc_factor {};
template <typename T, T v, typename... Tail>
struct calc_factor<list<num<T, v>, Tail...>> {
  using out = result_pack<num<T, v>, list<Tail...>>;
};
template <typename T, T v, typename... Tail>
struct calc_factor<list<op_neg, num<T, v>, Tail...>> {
  using out = result_pack<num<T, -v>, list<Tail...>>;
};
template <typename... Tail>
struct calc_factor<list<open_s, Tail...>,
                   std::void_t<typename calc_expr<list<Tail...>>::out>> {
  using inner = typename calc_expr<list<Tail...>>::out;
  using out = result_pack<typename inner::result,
                          typename pop_close_s<typename inner::tail>::type>;
};

// 2nd level: term rest
template <typename Left, typename Tail, typename = void> struct calc_term_rest {
  using out = result_pack<Left, Tail>;
};
template <typename T, T v, typename... Tail>
struct calc_term_rest<num<T, v>, list<op_mul, Tail...>,
                      std::void_t<typename calc_factor<list<Tail...>>::out>> {
  using next = typename calc_factor<list<Tail...>>::out;
  using cur = num<decltype(v * next::result::value), v * next::result::value>;
  using out = typename calc_term_rest<cur, typename next::tail>::out;
};

template <typename T, T v, typename... Tail>
struct calc_term_rest<
    num<T, v>, list<op_div, Tail...>,
    std::enable_if_t<calc_factor<list<Tail...>>::out::result::value != 0>> {
  using next = typename calc_factor<list<Tail...>>::out;
  using cur = num<decltype(v / next::result::value), v / next::result::value>;
  using out = typename calc_term_rest<cur, typename next::tail>::out;
};

// term
template <typename Tokens, typename> struct call_term {};
template <typename Tokens>
struct call_term<Tokens, std::void_t<typename calc_factor<Tokens>::out>> {
  using first = typename calc_factor<Tokens>::out;

  using out = typename calc_term_rest<typename first::result,
                                      typename first::tail>::out;
};

// 1st level: expression rest
template <typename Left, typename Tail, typename = void> struct calc_expr_rest {
  using out = result_pack<Left, Tail>;
};
template <typename T, T v, typename... Tail>
struct calc_expr_rest<num<T, v>, list<op_add, Tail...>,
                      std::void_t<typename call_term<list<Tail...>>::out>> {
  using next = typename call_term<list<Tail...>>::out;

  using cur = num<decltype(v + next::result::value), v + next::result::value>;

  using out = typename calc_expr_rest<cur, typename next::tail>::out;
};

template <typename T, T v, typename... Tail>
struct calc_expr_rest<num<T, v>, list<op_sub, Tail...>,
                      std::void_t<typename call_term<list<Tail...>>::out>> {
  using next = typename call_term<list<Tail...>>::out;

  using cur = num<decltype(v - next::result::value), v - next::result::value>;

  using out = typename calc_expr_rest<cur, typename next::tail>::out;
};

// expressions
template <typename Tokens, typename> struct calc_expr {};

template <typename Tokens>
struct calc_expr<Tokens, std::void_t<typename call_term<Tokens>::out>> {
  using first = typename call_term<Tokens>::out;

  using out = typename calc_expr_rest<typename first::result,
                                      typename first::tail>::out;
};

// wrapper
template <typename... Tokens> struct expr {
  static constexpr auto eval() {
    using attempt = calc_expr<list<Tokens...>>;
    static_assert(std::is_same_v<typename attempt::out::tail, list<>>,
                  "Invalid expression");

    return attempt::out::result::value;
  }

  friend std::string to_string(expr) {
    std::string parts[] = {name_of(Tokens{})...};

    std::string result;
    for (size_t i = 0; i < sizeof...(Tokens); ++i) {
      if (i > 0)
        result += " ";

      result += parts[i];
    }
    return result;
  }
};