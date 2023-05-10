/**
 * @file 61.cc
 * @author Derek Huang
 * @brief #61 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Google.
 *
 * Implement integer exponentiation. That is, implement the pow(x, y) function,
 * where x and y are integers and returns x^y. Do this faster than the naive
 * method of repeated multiplication. For example, pow(2, 10) should return 1024.
 */

#include <tuple>
#include <type_traits>

#include <gtest/gtest.h>

#include "pddcp/utility.h"

namespace {

/**
 * Fast exponentiation for integral types.
 *
 * Given an integral base x and an integral exponent n, the naive method of
 * integral exponentiation is O(n), but the "fast" implementation reduces the
 * complexity to around O(log(n)), where log is base 2. The idea is that we
 * recursively compute the squares of square roots to get the answer.
 *
 * @tparam OutType Floating return type
 * @tparam BaseType Integral type
 * @tparam ExpType Integral type
 *
 * @param base Base value to exponentiate
 * @param exp Exponent
 */
template <typename OutType, typename BaseType, typename ExpType>
constexpr auto fast_pow(BaseType base, ExpType exp)
{
  // inputs must be integral, return float (to allow negative exponents)
  static_assert(std::is_integral_v<BaseType>, "BaseType must be integral");
  static_assert(std::is_integral_v<ExpType>, "ExpType must be integral");
  static_assert(std::is_floating_point_v<OutType>, "OutType must be floating");
  // actual return type must have all three types be promoted to float
  using value_type = std::common_type_t<BaseType, ExpType, OutType>;
  static_assert(
    std::is_same_v<value_type, OutType>,
    "Cannot promote BaseType, ExpType to OutType"
  );
  // base case
  if (exp == 0)
    return value_type{1};
  // compute the square root of the result (approx if odd exponent)
  const auto res_sqrt = fast_pow<OutType>(base, exp / 2);
  // if exponent is odd, we have to multiply base one more time. furthermore,
  // if the exponent is negative, we have to divide by base instead.
  // if exponent even, multiply squares
  if (exp % 2 == 0)
    return res_sqrt * res_sqrt;
  // if odd, also need to multiply by base if positive, divide if negative.
  // for unsigned exp, we can avoid the branching (never negative)
  if constexpr (std::is_unsigned_v<ExpType>)
    return res_sqrt * res_sqrt * base;
  else
    return (exp < 0) ? res_sqrt * res_sqrt / base : res_sqrt * res_sqrt * base;
}

/**
 * Fast exponentiation for integral types.
 *
 * @tparam BaseType Integral type
 * @tparam ExpType Integral type
 *
 * @param base Base value to exponentiate
 * @param exp Exponent
 */
template <typename BaseType, typename ExpType>
inline constexpr auto fast_pow(BaseType base, ExpType exp)
{
  return fast_pow<double>(base, exp);
}

/**
 * Base test class template.
 *
 * Create specializations to use `TYPED_TEST` like a parametrized test.
 *
 * @tparam IndexedType `pddcp::indexed_type<I, T>`, tuple-like `element_type`
 */
template <typename IndexedType>
class DailyTest61 : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_HELPER_TYPES(IndexedType);
};

// input types used in the specializations + TYPED_TEST_SUITE macro
using InputType1 = pddcp::indexed_type<0, std::tuple<unsigned, short, float>>;
using InputType2 = pddcp::indexed_type<1, std::tuple<int, long, double>>;
using InputType3 = pddcp::indexed_type<2, std::tuple<int, unsigned, float>>;
using InputType4 = pddcp::indexed_type<3, std::tuple<int, int, double>>;
using InputType5 = pddcp::indexed_type<4, std::tuple<short, long, double>>;
using InputType6 = pddcp::indexed_type<5, std::tuple<short, unsigned, float>>;

/**
 * Helper macro defining each `DailyTest61` specialization.
 *
 * Each specialization has `base_`, `exp_`, `res_` members for the base,
 * exponent, and expected result, respectively.
 *
 * @param type `pddcp::indexed_type<I, T>` specialization
 * @param base Base value to exponentiate
 * @param exp Exponent value
 * @param res Expected result
 */
#define DAILY_TEST_61(type, base, exp, res) \
  template <> \
  class DailyTest61<type> : public ::testing::Test { \
  protected: \
    static inline constexpr \
    std::tuple_element_t<0, typename type::element_type> base_ = base; \
    static inline constexpr \
    std::tuple_element_t<1, typename type::element_type> exp_ = exp; \
    static inline constexpr \
    std::tuple_element_t<2, typename type::element_type> res_ = res; \
  }

/**
 * Specialization for the given input/output pair.
 */
DAILY_TEST_61(InputType1, 2, 10, 1024);

/**
 * Specialization for the first custom input/output pair.
 *
 * Here we use a negative exponent in our input.
 */
DAILY_TEST_61(InputType2, 4, -3, 0.015625);

/**
 * Specialization for the second custom input/output pair.
 *
 * Here we use an unsigned exponent for the first time.
 */
DAILY_TEST_61(InputType3, 8, 3, 512);

/**
 * Specialization for the third custom input/output pair.
 *
 * Here we use a negative base in our input.
 */
DAILY_TEST_61(InputType4, -4, 3, -64);

/**
 * Specialization for the fourth custom input/output pair.
 *
 * Here we use both a negative base and a negative exponent.
 */
DAILY_TEST_61(InputType5, -6, -4, 0.0007716049382716049);

/**
 * Specialization for the fifth custom input/output pait.
 *
 * Here we use a negative base with an even positive exponent.
 */
DAILY_TEST_61(InputType6, -5, 8, 390625);

using DailyTest61Types = ::testing::Types<
  InputType1, InputType2, InputType3, InputType4, InputType5, InputType6
>;
TYPED_TEST_SUITE(DailyTest61, DailyTest61Types);

/**
 * Check that `fast_pow` works as expected.
 */
TYPED_TEST(DailyTest61, TypedTest)
{
  EXPECT_DOUBLE_EQ(
    TestFixture::res_, fast_pow(TestFixture::base_, TestFixture::exp_)
  );
}

}  // namespace
