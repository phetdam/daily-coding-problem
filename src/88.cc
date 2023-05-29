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

#include <cstdint>
#include <stdexcept>
#include <type_traits>

#include <gtest/gtest.h>

#include "pddcp/utility.h"

namespace {

/**
 * Perform integer division of `a` by `b` for signed types.
 *
 * @tparam T Signed integral type
 *
 * @param a Numerator
 * @param b Denominator
 */
template <typename T, std::enable_if_t<std::is_signed_v<T>, bool> = true>
auto divide(T a, T b)
{
  if (!b)
    throw std::runtime_error{"denominator b is zero"};
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

/**
 * Perform integer division of `a` by `b` for unsigned types.
 *
 * @tparam T Unsigned integral type
 *
 * @param a Numerator
 * @param b Denominator
 */
template <typename T, std::enable_if_t<std::is_unsigned_v<T>, bool> = true>
auto divide(T a, T b)
{
  if (!b)
    throw std::runtime_error{"denominator b is zero"};
  T quot = 0;
  while (a >= b) {
    a -= b;
    quot++;
  }
  return quot;
}

/**
 * Base test class template.
 *
 * @tparam IndexedType `pddcp::indexed_type<I, T>`, integral `element_type`
 */
template <typename IndexedType>
class DailyTest88 : public ::testing::Test {};

// input types used in the specializations + TYPED_TEST_SUITE macro
using InputType1 = pddcp::indexed_type<0, unsigned int>;
using InputType2 = pddcp::indexed_type<1, unsigned int>;
using InputType3 = pddcp::indexed_type<2, int>;
using InputType4 = pddcp::indexed_type<3, long>;
using InputType5 = pddcp::indexed_type<4, long>;
using InputType6 = pddcp::indexed_type<5, std::size_t>;
using InputType7 = pddcp::indexed_type<6, std::int32_t>;
using InputType8 = pddcp::indexed_type<7, short>;

/**
 * Helper macro defining each `DailyTest88` specialization.
 *
 * Each specialization has `a_`, `b_`, and `res_` members for the numerator,
 * denominator, and expected result, respectively.
 *
 * @param type `pddcp::indexed_type<I, T>` specialiation
 * @param a Input numerator
 * @param b Input denominator
 * @param res Expected result
 */
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

/**
 * Specialization for first input/output pair.
 *
 * Tests using unsigned inputs.
 */
DAILY_TEST_88(InputType1, 9, 3, 3);

/**
 * Specialization for the second input/output pair.
 *
 * Tests using unsigned inputs, expecting correct truncation.
 */
DAILY_TEST_88(InputType2, 10, 3, 3);

/**
 * Specialization for the third input/output pair.
 *
 * Tests using signed inputs.
 */
DAILY_TEST_88(InputType3, 14, 2, 7);

/**
 * Specialization for the fourth input/output pair.
 *
 * Tests using signed inputs, expecting correct truncation.
 */
DAILY_TEST_88(InputType4, 15, 4, 3);

/**
 * Specialization for the fifth input/output pair.
 *
 * Tests using signed inputs, negative denominator, expecting truncation.
 */
DAILY_TEST_88(InputType5, 18, -4, -4);

/**
 * Specialization for the sixth input/output pair.
 *
 * Tests using unsigned inputs where numerator is zero.
 */
DAILY_TEST_88(InputType6, 0, 4, 0);

/**
 * Specialization for the seventh input/output pair.
 *
 * Tests using signed inputs, negative numerator, expecting truncation.
 */
DAILY_TEST_88(InputType7, -1, 10, 0);

/**
 * Specialization for the eigth input/output pair.
 *
 * Tests using signed inputs, negative numerator, expecting truncation. Here
 * the signed type used is `short`, to check if any warnings are raised.
 */
DAILY_TEST_88(InputType8, -19, 4, -4);

using DailyTest88Types = ::testing::Types<
  InputType1,
  InputType2,
  InputType3,
  InputType4,
  InputType5,
  InputType6,
  InputType7,
  InputType8
>;
TYPED_TEST_SUITE(DailyTest88, DailyTest88Types);

/**
 * Test that `divide` works as expected.
 */
TYPED_TEST(DailyTest88, TypedTest)
{
  EXPECT_EQ(TestFixture::res_, divide(TestFixture::a_, TestFixture::b_));
}

}  // namespace
