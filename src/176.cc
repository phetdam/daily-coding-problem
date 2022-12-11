/**
 * @file 176.cc
 * @author Derek Huang
 * @brief #176 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Bloomberg.
 *
 * Determine whether there exists a one-to-one character mapping from one
 * string s1 to another s2.
 *
 * For example, given s1 = abc and s2 = bcd, return true since we can map a to
 * b, b to c, and c to d.
 *
 * Given s1 = foo and s2 = bar, return false since the o cannot map to two
 * characters.
 */

#include <cstddef>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

#include <gtest/gtest.h>

namespace {

/**
 * Determine if a bijective character map exists between two strings.
 *
 * Runs in O(N) time using O(N) space. To determine that the bijective map
 * exists, it is sufficient to find that both strings are nonempty and both
 * contain an equal number of unique characters.
 *
 * @tparam T character type
 *
 * @param a first string
 * @param b second string
 */
template <typename T>
bool bijective_map(const std::basic_string<T>& a, const std::basic_string<T>& b)
{
  if (!a.size() || !b.size())
    return false;
  std::unordered_map<T, std::size_t> map_a, map_b;
  for (const auto& c : a)
    map_a[c]++;
  for (const auto& c : b)
    map_b[c]++;
  if (map_a.size() == map_b.size())
    return true;
  return false;
}

// convenience type aliases
using result_type = bool;
using input_type = std::pair<std::string, std::string>;
using pair_type = std::pair<input_type, result_type>;

/**
 * Test fixture class for parametrized testing.
 */
class Daily176Test : public ::testing::TestWithParam<pair_type> {};

INSTANTIATE_TEST_SUITE_P(
  SamplePairs,
  Daily176Test,
  ::testing::Values(
    pair_type{{"abc", "bcd"}, true},
    pair_type{{"foo", "bar"}, false}
  )
);

INSTANTIATE_TEST_SUITE_P(
  CustomPairs,
  Daily176Test,
  ::testing::Values(
    // 3 unique chars per string, different lengths
    pair_type{{"sdsdsooooo", "ppappaaaajjjj"}, true},
    // only one duplicate char each, same length
    pair_type{{"sd9fuhs", "asdffgk"}, true},
    pair_type{{"sdsssffdpop", "asdf"}, false}
  )
);

/**
 * Test that `bijective_map` works correctly.
 */
TEST_P(Daily176Test, ParamTest)
{
  const auto& input = GetParam().first;
  const auto& output = GetParam().second;
  EXPECT_EQ(output, bijective_map(input.first, input.second));
}

}  // namespace
