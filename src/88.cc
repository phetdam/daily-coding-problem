/**
 * @file 88.cc
 * @author Derek Huang
 * @brief #88 from Daily Coding Problem
 * @copyright MIT License
 *
 * This question was asked by ContextLogic.
 *
 * Implement division of two positive integers without using the division,
 * multiplication, or modulus operators. Return the quotient as an integer,
 * ignoring the remainder.
 */

#include <type_traits>

#include <gtest/gtest.h>

#include "pddcp/utility.h"

namespace {

template <typename T, std::enable_if_t<std::is_signed_v<T>, bool> = true>
auto divide(T a, T b)
{
  // flags for signs of a, b
  bool a_neg = (a < 0);
  bool b_neg = (b < 0);
  // magnitudes of a, b
  T a_u = (a_neg) ? -a : a;
  T b_u = (b_neg) ? -b : b;
  // determine divisibility
  T quot = 0;
  while (a_u >= b_u) {
    a_u -= b_u;
    quot++;
  }
  // return quotient. will be negative if signs don't match
  return (a_neg && !b_neg || !a_neg && b_neg) ? -quot : quot;
}

template <typename T, std::enable_if_t<std::is_unsigned_v<T>, bool> = true>
auto divide(T a, T b)
{
  T quot = 0;
  while (a >= b) {
    a -= b;
    quot++;
  }
  return quot;
}

template <typename IndexedType>
class DailyTest88 : public ::testing::Test {};

using InputType1 = pddcp::indexed_type<0, unsigned int>;
using InputType2 = pddcp::indexed_type<1, unsigned int>;
using InputType3 = pddcp::indexed_type<2, int>;
using InputType4 = pddcp::indexed_type<3, long>;
using InputType5 = pddcp::indexed_type<4, long>;

#define DAILY_TEST_88(type, a, b, res) \
  template <> \
  class DailyTest88<type> : public ::testing::Test { \
  public: \
    PDDCP_INDEXED_TYPE_HELPER_TYPES(type); \
  protected: \
    static inline constexpr element_type a_ = a; \
    static inline constexpr element_type b_ = b; \
    static inline constexpr element_type res_ = res; \
  }

DAILY_TEST_88(InputType1, 9, 3, 3);
DAILY_TEST_88(InputType2, 10, 3, 3);
DAILY_TEST_88(InputType3, 14, 2, 7);
DAILY_TEST_88(InputType4, 15, 4, 3);
DAILY_TEST_88(InputType5, 18, -4, -4);

using DailyTest88Types = ::testing::Types<
  InputType1, InputType2, InputType3, InputType4, InputType5
>;
TYPED_TEST_SUITE(DailyTest88, DailyTest88Types);

TYPED_TEST(DailyTest88, TypedTest)
{
  EXPECT_EQ(TestFixture::res_, divide(TestFixture::a_, TestFixture::b_));
}

}  // namespace
