/**
 * @file 62.cc
 * @author Derek Huang
 * @brief #62 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Facebook.
 *
 * There is an N by M matrix of zeroes. Given N and M, write a function to
 * count the number of ways of starting at the top-left corner and getting to
 * the bottom-right corner. You can only move right or down. For example, given
 * a 2 by 2 matrix, you should return 2, since there are two ways to get to the
 * bottom-right:
 *
 * Right, then down
 * Down, then right
 *
 * Given a 5 by 5 matrix, there are 70 ways to get to the bottom-right.
 */

#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

namespace {

/**
 * Return number of paths from the top left to bottom right of a grid.
 *
 * The paths consist of only down and right movements. For a M by N grid, the
 * time complexity is O(MN), while the space complexity is only O(2N), as we
 * are able to compute the number of paths to each cell in the grid row by row
 * starting from the first row and so do not need an entire O(MN) grid.
 *
 * @param n_rows Number of rows
 * @param n_cols Number of columns
 */
std::size_t bottom_right_paths(std::size_t n_rows, std::size_t n_cols)
{
  if (!n_rows || !n_cols)
    return 0;
  // paths_0 holds the path counts for the previously computed row, while
  // paths_1 holds the path counts for the currently computed row. paths_0 is
  // initially all 1s, as any cell in the top row only has 1 possible path.
  std::vector<std::size_t> paths_0(n_cols, 1);
  std::vector<std::size_t> paths_1(n_cols, 0);
  // first cell in the second row only has one possible path
  paths_1[0] = 1;
  // starting from the 2nd row and 2nd column, we can apply the recurrence
  // relation p(r, c) = p(r - 1, c) + p(r, c - 1), i.e. number of paths to
  // (r, c) is the sum of paths to (r - 1, c) and paths to (r, c - 1).
  for (decltype(n_rows) i = 1; i < n_rows; i++) {
    for (decltype(n_cols) j = 1; j < n_cols; j++)
      paths_1[j] = paths_0[j] + paths_1[j - 1];
    // swap the results so we can compute next row
    std::swap(paths_0, paths_1);
  }
  // due to the final swap, result is stored in paths_0
  return paths_0[n_cols - 1];
}

// convenience type aliases
using input_type = std::pair<std::size_t, std::size_t>;
using result_type = std::size_t;
using pair_type = std::pair<input_type, result_type>;

/**
 * Test fixture class for parametrized testing.
 */
class DailyTest62 : public ::testing::TestWithParam<pair_type> {};

INSTANTIATE_TEST_SUITE_P(
  SamplePairs,
  DailyTest62,
  ::testing::Values(pair_type{{2, 2}, 2}, pair_type{{5, 5}, 70})
);

/**
 * Test that `bottom_right_paths` works as expected.
 */
TEST_P(DailyTest62, ParamTest)
{
  const auto& [dims, n_paths] = GetParam();
  EXPECT_EQ(n_paths, bottom_right_paths(dims.first, dims.second));
}

}  // namespace
