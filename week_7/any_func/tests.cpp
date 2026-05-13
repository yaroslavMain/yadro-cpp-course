#include "any.h"
#include <gtest/gtest.h>

std::string print_hello(const std::string &name) {
  return "Dear " + name + ", hello from Yaroslav T.";
}

TEST(AnyFuncTests, BasicLambda) {
  // First
  const AnyFunc<int(int)> f1 = [](const int el) { return el * el; };
  ASSERT_EQ(f1(0), 0);
  ASSERT_EQ(f1(1), 1);
  ASSERT_EQ(f1(2), 4);
  ASSERT_EQ(f1(-3), 9);

  // Second
  const AnyFunc<int(int, int)> f2 = [](const int el1, const int el2) {
    return el1 - el2;
  };
  ASSERT_EQ(f2(0, 0), 0);
  ASSERT_EQ(f2(1, 1), 0);
  ASSERT_EQ(f2(2, 1), 1);
  ASSERT_EQ(f2(-3, 4), -7);
}

TEST(AnyFuncTests, BasicStringFunc) {
  AnyFunc<std::string(const std::string &)> f1 = print_hello;
  ASSERT_EQ(print_hello("Ivan"), "Dear Ivan, hello from Yaroslav T.");
  ASSERT_EQ(print_hello("Test"), "Dear Test, hello from Yaroslav T.");
}

TEST(AnyFuncTests, BadFunc) {
  const AnyFunc<int(int)> f1;
  EXPECT_THROW(f1(0), std::bad_function_call);
}

TEST(AnyFuncTests, Copy) {
  const AnyFunc<int(int)> f1 = [](const int x) { return x + 1; };
  const AnyFunc<int(int)> f2 = f1;
  ASSERT_EQ(f2(0), f1(0));
  ASSERT_EQ(f2(10), f1(10));

  const AnyFunc f3(f2);
  ASSERT_EQ(f3(14), f2(14));
}

TEST(AnyFuncTests, Move) {
  AnyFunc<int(int)> f1 = [](const int x) { return x + 1; };
  const AnyFunc<int(int)> f2 = std::move(f1);

  ASSERT_EQ(f2(10), 11);
  ASSERT_EQ(f2(15), 16);
  ASSERT_THROW(f1(10), std::exception);
}
