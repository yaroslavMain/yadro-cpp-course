#include <gtest/gtest.h>

#include "checker.h"
#include "meta_computer.h"

TEST(MetaComputerTests, SimpleArithmetic) {
  using E1 = expr<num<int, 5>, op_sub, num<int, 3>>;
  static_assert(E1::eval() == 2);
  EXPECT_EQ(E1::eval(), 2);
  EXPECT_EQ(to_string(E1{}), "5 - 3");

  using E2 = expr<num<int, 4>, op_mul, num<int, 3>>;
  static_assert(E2::eval() == 12);
  EXPECT_EQ(E2::eval(), 12);
  EXPECT_EQ(to_string(E2{}), "4 * 3");

  using E3 = expr<num<int, 10>, op_div, num<int, 2>>;
  static_assert(E3::eval() == 5);
  EXPECT_EQ(E3::eval(), 5);
  EXPECT_EQ(to_string(E3{}), "10 / 2");
}

TEST(MetaComputerTests, Precedence) {
  using E1 = expr<num<int, 2>, op_add, num<int, 2>, op_mul, num<int, 2>>;
  static_assert(E1::eval() == 6);
  EXPECT_EQ(E1::eval(), 6);
  EXPECT_EQ(to_string(E1{}), "2 + 2 * 2");

  using E2 = expr<num<int, 10>, op_sub, num<int, 6>, op_div, num<int, 2>>;
  static_assert(E2::eval() == 7);
  EXPECT_EQ(E2::eval(), 7);
}

TEST(MetaComputerTests, Parentheses) {
  using E1 = expr<open_s, num<int, 2>, op_add, num<int, 2>, close_s, op_mul,
                  num<int, 2>>;
  static_assert(E1::eval() == 8);
  EXPECT_EQ(E1::eval(), 8);
  EXPECT_EQ(to_string(E1{}), "( 2 + 2 ) * 2");

  using E2 = expr<num<int, 20>, op_div, open_s, num<int, 2>, op_add,
                  num<int, 3>, close_s>;
  static_assert(E2::eval() == 4);
  EXPECT_EQ(E2::eval(), 4);
  EXPECT_EQ(to_string(E2{}), "20 / ( 2 + 3 )");
}

TEST(MetaComputerTests, UnaryNegation) {
  using E1 = expr<op_neg, num<int, 5>, op_add, num<int, 10>>;
  static_assert(E1::eval() == 5);
  EXPECT_EQ(E1::eval(), 5);

  using E2 = expr<num<int, 5>, op_mul, op_neg, num<int, 3>>;
  static_assert(E2::eval() == -15);
  EXPECT_EQ(E2::eval(), -15);
}

TEST(MetaComputerTests, ComplexExpression) {
  using E = expr<open_s, op_neg, num<int, 10>, op_add, num<int, 2>, close_s,
                 op_mul, open_s, num<int, 15>, op_div, num<int, 3>, close_s>;
  static_assert(E::eval() == -40);
  EXPECT_EQ(E::eval(), -40);
}

TEST(MetaComputerTests, ValidExpressions) {
  using E1 = expr<op_neg, num<int, 5>, op_add, num<int, 10>>;
  static_assert(is_valid_expression_v<E1> == 1);
  EXPECT_EQ(is_valid_expression_v<E1>, 1);

  using E2 = expr<op_neg, num<int, 5>, op_add>;
  static_assert(is_valid_expression_v<E2> == 0);
  EXPECT_EQ(is_valid_expression_v<E2>, 0);
}
