/**
 * @file 35.cc
 *
 * This problem was asked by Google.
 *
 * Given an array of strictly the characters 'R', 'G', and 'B', segregate the
 * values of the array so that all the Rs come first, the Gs come second, and
 * the Bs come last. You can only swap elements of the array. Do this in linear
 * time and in-place. For example, given the array ['G', 'B', 'R', 'R', 'B',
 * 'R', 'G'], it should become ['R', 'R', 'R', 'G', 'G', 'B', 'B'].
 */

#include <cstddef>
#include <exception>
#include <string>
#include <vector>

#include <gtest/gtest.h>

namespace {

template <typename V_t>
inline void swap(V_t& v, typename V_t::size_type i, typename V_t::size_type j)
{
  auto t = v[i];
  v[i] = v[j];
  v[j] = t;
}

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
      swap(rgb_seg, i, ii++);
  // move all the Bs to the end of the array
  ii = n - 1;
  for (diff_type i = n - 1; i >= 0; i--)
    if (rgb_seg[i] == 'B')
      swap(rgb_seg, i, ii--);
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
