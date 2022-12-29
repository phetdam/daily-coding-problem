/**
 * @file 124.cc
 * @author Derek Huang
 * @brief #124 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Microsoft.
 *
 * You have n fair coins and you flip them all at the same time. Any that come
 * up tails you set aside. The ones that come up heads you flip again. How many
 * rounds do you expect to play before only one coin remains?
 *
 * Write a function that, given n, returns the number of rounds you'd expect to
 * play until one coin remains.
 */

#include <cmath>
#include <cstdint>
#include <random>
#include <type_traits>
#include <utility>

#include <gtest/gtest.h>

namespace {

/**
 * Return expected number of coin flip rounds.
 *
 * Given n fair coins, we expect half of them to land tails each round. If k is
 * such that 2 ^ k = n, k is the expected number of rounds we'd play until only
 * one coin remains. For n = 0, we obviously use n = 1 instead.
 *
 * @tparam UIntType An unsigned integral type
 *
 * @param n Number of rounds
 */
template <typename UIntType>
inline auto expected_rounds(UIntType n)
{
  static_assert(std::is_unsigned_v<UIntType>);
  return std::log2((!n) ? n + 1 : n);
}

// convenience type aliases
using result_type = double;
using input_type = unsigned int;
using pair_type = std::pair<input_type, result_type>;

/**
 * Test fixture class for parametrized testing.
 */
class DailyTest124 : public ::testing::TestWithParam<pair_type> {};

INSTANTIATE_TEST_SUITE_P(
  CustomPairs,
  DailyTest124,
  ::testing::Values(
    // edge cases should both evaluate to zero
    pair_type{0, 0.},
    pair_type{1, 0.},
    // even cases
    pair_type{2, 1.},
    pair_type{6, 2.584962500721156},
    pair_type{8, 3.},
    // odd cases
    pair_type{3, 1.584962500721156},
    pair_type{5, 2.321928094887362},
    pair_type{7, 2.807354922057604}
  )
);

/**
 * Test that `expected_rounds` works as expected.
 */
TEST_P(DailyTest124, FormulaParamTest)
{
  EXPECT_DOUBLE_EQ(GetParam().second, expected_rounds(GetParam().first));
}

}  // namespace
