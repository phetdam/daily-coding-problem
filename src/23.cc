/**
 * @file 23.cc
 * @author Derek Huang
 * @brief #23 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Google.
 *
 * You are given an M by N matrix consisting of booleans that represents a
 * board. Each True boolean represents a wall. Each False boolean represents a
 * tile you can walk on.
 *
 * Given this matrix, a start coordinate, and an end coordinate, return the
 * minimum number of steps required to reach the end coordinate from the start.
 * If there is no possible path, then return null. You can move up, left, down,
 * and right. You cannot move through walls. You cannot wrap around the edges
 * of the board.
 *
 * For example, given the following board:
 *
 * [[f, f, f, f],
 *  [t, t, f, t],
 *  [f, f, f, f],
 *  [f, f, f, f]]
 *
 * and start = (3, 0) (bottom left) and end = (0, 0) (top left), the minimum
 * number of steps required to reach the end is 7, since we would need to go
 * through (1, 2) because there is a wall everywhere else on the second row.
 */

#include <algorithm>
#include <cstdint>
#include <limits>
#include <queue>
#include <utility>

#include <gtest/gtest.h>

#include "pddcp/matrix.h"

namespace {

/**
 * Return minimum step count needed to reach the end from the start point.
 *
 * @tparam Matrix Matrix type with `row_count`, `col_count` attributes
 *
 * @param board Coordinate grid to traverse
 * @param start Start point on grid
 * @param end Destination point on grid
 */
template <typename Matrix, typename size_type = typename Matrix::size_type>
auto min_steps(
  const Matrix& board,
  const std::pair<size_type, size_type>& start,
  const std::pair<size_type, size_type>& end)
{
  using index_type = std::pair<size_type, size_type>;
  using result_type = std::size_t;
  static_assert(
    std::is_same_v<bool, typename Matrix::value_type>,
    "Matrix value_type expected to be bool"
  );
  static_assert(std::is_unsigned_v<size_type>, "size_type must be unsigned");
  // no steps if we're already there
  if (start == end)
    return result_type{0};
  // number of steps from start to end
  auto n_steps = std::numeric_limits<result_type>::max();
  // queue holding pair of starting point and accumulated steps to that point
  std::queue<std::pair<index_type, decltype(n_steps)>> queue;
  queue.push({start, 0});
  // perform BFS from the start point
  while (queue.size()) {
    // point, steps to that point, row and column indices
    auto [point, steps] = queue.front();
    auto [row_i, col_i] = point;
    queue.pop();
    // if at the destination, update n_steps to smaller value if possible
    if (point == end)
      n_steps = std::min(n_steps, steps);
    // otherwise, continue searching from non-diagonal adjacent tiles
    for (const auto& row_offset : {-1, 1})
      for (const auto& col_offset : {-1, 1}) {
        auto new_row_i = row_i + row_offset;
        auto new_col_i = col_i + col_offset;
        // skip if we are going out of bounds. since size_type is unsigned, -1
        // is its max value, which is an illegal index value anyways.
        if (new_row_i >= board.row_count || new_col_i >= board.col_count)
          continue;
        queue.push({{new_row_i, new_col_i}, steps + 1});
      }
  }
  return n_steps;
}

// convenience type aliases
// using input_type = pddcp::sparse_matrix

// class DailyTest23 : public ::testing::TestWithParam<> {};

}  // namespace
