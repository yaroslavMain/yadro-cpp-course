#pragma once
#include <cstddef>

template <typename...> struct Tuple {};

template <> struct Tuple<> {};

template <typename Head, typename... Tail>
struct Tuple<Head, Tail...> : Tuple<Tail...> {
  Head head_;

  Tuple() = default;
  explicit Tuple(Head h, Tail... t) : Tuple<Tail...>(t...), head_(h) {}
};

template <size_t I, typename... Args> struct TupleElement;

template <typename Head, typename... Tail>
struct TupleElement<0, Tuple<Head, Tail...>> {
  using type = Head;
  using tuple_type = Tuple<Head, Tail...>;
};

template <size_t I, typename Head, typename... Tail>
struct TupleElement<I, Tuple<Head, Tail...>>
    : TupleElement<I - 1, Tuple<Tail...>> {};

template <size_t I, typename... Args> auto &get(Tuple<Args...> &t) {
  using BaseType = typename TupleElement<I, Tuple<Args...>>::tuple_type;
  return static_cast<BaseType &>(t).head_;
}

template <size_t I, typename Tuple> void fill_tuple(Tuple &tuple) {}

template <size_t I, typename Tuple, typename First, typename... Rest>
void fill_tuple(Tuple &tuple, const First &first, const Rest &...rest) {
  get<I>(tuple) = first;
  fill_tuple<I + 1, Tuple, Rest...>(tuple, rest...);
}

template <typename... TTypes> Tuple<TTypes...> MakeTuple(TTypes... t) {
  Tuple<TTypes...> result{};
  fill_tuple<0>(result, t...);
  return result;
}
