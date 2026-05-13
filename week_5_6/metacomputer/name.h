#pragma once
#include "ops.h"
#include <string>

template <typename T, T v> std::string name_of(num<T, v>) {
  return std::to_string(v);
}

inline std::string name_of(op_add) { return "+"; }
inline std::string name_of(op_sub) { return "-"; }
inline std::string name_of(op_mul) { return "*"; }
inline std::string name_of(op_div) { return "/"; }
inline std::string name_of(op_neg) { return "-"; }
inline std::string name_of(open_s) { return "("; }
inline std::string name_of(close_s) { return ")"; }
