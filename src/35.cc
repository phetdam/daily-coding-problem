/**
 * @file 35.cc
 * @author Derek Huang
 * @brief #35 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Google.
 *
 * Given an array of strictly the characters 'R', 'G', and 'B', segregate the
 * values of the array so that all the Rs come first, the Gs come second, and
 * the Bs come last. You can only swap elements of the array. Do this in linear
 * time and in-place. For example, given the array ['G', 'B', 'R', 'R', 'B',
 * 'R', 'G'], it should become ['R', 'R', 'R', 'G', 'G', 'B', 'B'].
 */

#include <algorithm>
#include <cstddef>
#include <string>
#include <type_traits>
#include <vector>

#include <gtest/gtest.h>

namespace {

/**
 * Segregate a *SequenceContainer* of `char` RGB into R, G, B ordered sections.
 *
 * This is done in linear time by noting that all we need to do is first make
 * one linear pass to put all Rs in the front and then another linear pass in
 * reverse to put all the Bs in the back. By default, Gs go into middle.
 *
 * @tparam V_t *SequenceContainer* with `value_type = char`
 *
 * @param rgb *SequenceContainer* of RGB `char` to perform segregation on.
 */
template <typename V_t>
V_t rgb_segregate(const V_t& rgb)
{
  static_assert(std::is_same_v<char, typename V_t::value_type>);
  // type aliases
  using size_type = typename V_t::size_type;
  using diff_type = typename V_t::difference_type;
  // array size + indices to start inserting + temp value + return value
  size_type n = rgb.size();
  diff_type ii = 0;
  V_t rgb_seg = rgb;
  // move all the Rs to the beginning of the array
  for (size_type i = 0; i < n; i++)
    if (rgb_seg[i] == 'R')
      std::swap(rgb_seg[i], rgb_seg[ii++]);
  // move all the Bs to the end of the array
  ii = n - 1;
  for (diff_type i = n - 1; i >= 0; i--)
    if (rgb_seg[i] == 'B')
      std::swap(rgb_seg[i], rgb_seg[ii--]);
  return rgb_seg;
}

// convenience type aliases
using string_pair = std::pair<std::string, std::string>;

/**
 * Test fixture class for parametrized testing.
 *
 * More convenient to test using strings instead of vectors.
 */
class DailyTest35 : public ::testing::TestWithParam<string_pair> {};

INSTANTIATE_TEST_SUITE_P(
  SamplePairs,
  DailyTest35,
  ::testing::Values(string_pair{"GBRRBRG", "RRRGGBB"})
);

INSTANTIATE_TEST_SUITE_P(
  CustomPairs,
  DailyTest35,
  ::testing::Values(
    string_pair{"RGRGRGRBBBR", "RRRRRGGGBBB"},
    // partially sorted by character block
    string_pair{"RRRRRBBBBGGGG", "RRRRRGGGGBBBB"},
    string_pair{"RGRGRBGBGR", "RRRRGGGGBB"},
    // repeating triplets
    string_pair{"RGBRGBRGBRGBRGB", "RRRRRGGGGGBBBBB"},
    // patterned blocks
    string_pair{"RRRGGGBBBRRRRGGGGBB", "RRRRRRRGGGGGGGBBBBB"}
  )
);

/**
 * Test that `rgb_segregate` works correctly.
 */
TEST_P(DailyTest35, ParamTest)
{
  EXPECT_EQ(GetParam().second, rgb_segregate(GetParam().first));
}

}  // namespace
