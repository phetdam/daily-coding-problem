/**
 * @file 161.cc
 * @author Derek Huang
 * @brief #161 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Facebook.
 *
 * Given a 32-bit integer, return the number with its bits reversed.
 *
 * For example, given the binary number 1111 0000 1111 0000 1111 0000 1111 0000,
 * return 0000 1111 0000 1111 0000 1111 0000 1111.
 */

#include <cstdint>
#include <type_traits>
#include <utility>

#include <gtest/gtest.h>

namespace {

/**
 * Return input with bits reversed.
 *
 * @tparam T Integral type
 *
 * @param value Value to have bits reversed
 */
template <typename T>
inline T bit_reverse(T value)
{
  static_assert(std::is_integral_v<T>);
  return ~value;
}

/**
 * Macro for reversing bits.
 *
 * @param x Integral value to have bits reversed
 */
#define BIT_REVERSE(x) ~(x)

// convenience type aliases
using input_type = std::uint32_t;
using result_type = input_type;
using pair_type = std::pair<input_type, result_type>;

/**
 * Test fixture class for parametrized testing.
 */
class DailyTest161 : public ::testing::TestWithParam<pair_type> {};

INSTANTIATE_TEST_SUITE_P(
  SamplePairs,
  DailyTest161,
  // writing using 0x instead of 0b is more concise
  ::testing::Values(pair_type{0xF0F0F0F0, 0x0F0F0F0F})
);

INSTANTIATE_TEST_SUITE_P(
  CustomPairs,
  DailyTest161,
  ::testing::Values(
    pair_type{0x0F02A6B2, 0xF0FD594D}, pair_type{0x023A4D38, 0xFDC5B2C7}
  )
);

/**
 * Test that `bit_reverse` works as expected.
 */
TEST_P(DailyTest161, FunctionParamTest)
{
  EXPECT_EQ(GetParam().second, bit_reverse(GetParam().first));
}

/**
 * Test that `BIT_REVERSE` works as expected.
 */
TEST_P(DailyTest161, MacroParamTest)
{
  EXPECT_EQ(GetParam().second, BIT_REVERSE(GetParam().first));
}

}  // namespace
