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
#include <tuple>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "pddcp/matrix.h"

namespace {

/**
 * Return minimum step count needed to reach the end from the start point.
 *
 * @tparam Matrix Matrix type with `row_count`, `col_count` attributes
 * @tparam size_type Unsigned integral type, default `Matrix::size_type`
 * @tparam index_type Pair-like type where both elements are `size_type`
 *
 * @param board Coordinate grid to traverse
 * @param start Start point on grid
 * @param end Destination point on grid
 */
template <
  typename Matrix,
  typename size_type = typename Matrix::size_type,
  typename index_type = std::pair<size_type, size_type>>
auto min_steps(
  const Matrix& board, const index_type& start, const index_type& end)
{
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
  // copy of the board we use to mark where we've visited so we don't loop
  auto mark_board = board;
  // queue holding pair of starting point and accumulated steps to that point
  std::queue<std::pair<index_type, decltype(n_steps)>> queue;
  queue.push({start, 0});
  // perform BFS from the start point
  while (queue.size()) {
    // point, steps to that point, row and column indices
    auto [point, steps] = queue.front();
    auto [row_i, col_i] = point;
    queue.pop();
    // mark as visited so we don't fall into cycles
    mark_board(row_i, col_i) = true;
    // if at the destination, update n_steps to smaller value if possible and
    // don't add any new points to the queue (continue)
    if (point == end) {
      n_steps = std::min(n_steps, steps);
      continue;
    }
    // otherwise, continue searching from non-diagonal adjacent tiles
    std::vector<index_type> offsets{{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    for (const auto& [row_offset, col_offset] : offsets) {
      auto new_row_i = row_i + row_offset;
      auto new_col_i = col_i + col_offset;
      // skip if we are going out of bounds or walking into a wall. since
      // size_type is unsigned, -1 is its max value, which is an illegal
      // index value anyways, so no issue with negative indices.
      if (
        new_row_i >= board.row_count ||
        new_col_i >= board.col_count ||
        mark_board(new_row_i, new_col_i)  // true is a wall
      )
        continue;
      // otherwise, continue our search
      queue.push({{new_row_i, new_col_i}, steps + 1});
    }
  }
  return n_steps;
}

// convenience type aliases. right now we keep 4 x 4 board dimensions. note
// that the orders of the type parameters is reversed; this is so that the
// input is easier for human eyes to read when examining test parameters.
using board_type = pddcp::sparse_matrix<4, 4, bool>;
// input type is tuple of start index, end index, vector of [index_type, bool]
using input_type = std::tuple<
  typename board_type::index_type,
  typename board_type::index_type,
  std::vector<typename board_type::storage_type::value_type>
>;
using result_type = decltype(min_steps(board_type{}, {}, {}));
// in other tests, we usually have input_type before result_type
using pair_type = std::pair<result_type, input_type>;

/**
 * Test fixture for parametrized testing.
 */
class DailyTest23 : public ::testing::TestWithParam<pair_type> {};

INSTANTIATE_TEST_SUITE_P(
  SamplePairs,
  DailyTest23,
  ::testing::Values(
    /**
     * F F F F  (3, 0) -> (0, 0) [7]
     * T T F T
     * F F F F
     * F F F F
     */
    pair_type{
      7,
      {
        {3, 0},
        {0, 0},
        {{{1, 0}, true}, {{1, 1}, true}, {{1, 3}, true}}
      }
    }
  )
);

INSTANTIATE_TEST_SUITE_P(
  CustomPairs,
  DailyTest23,
  ::testing::Values(
    /**
     * F F F F  (2, 2) -> (0, 3) [3]
     * F T F T
     * T F F T
     * F F T F
     */
    pair_type{
      3,
      {
        {2, 2},
        {0, 3},
        {
          {{2, 0}, true},
          {{1, 1}, true},
          {{3, 2}, true},
          {{1, 3}, true},
          {{2, 3}, true}
        }
      }
    },
    /**
     * F F F T  (0, 0) -> (3, 3) [6]
     * F T F F
     * F T F T
     * F F F F
     */
    pair_type{
      6,
      {
        {0, 0},
        {3, 3},
        {{{1, 1}, true}, {{2, 1}, true}, {{0, 3}, true}, {{2, 3}, true}}
      }
    }
  )
);

/**
 * Test that `min_steps` works correctly.
 */
TEST_P(DailyTest23, ParamTest)
{
  // stepts are the expected number of steps
  const auto& [steps, input] = GetParam();
  // start point, end point, and the [index, bool] pairs sent to the board
  const auto& [start, end, value_pairs] = input;
  // initialize our sparse_matrix board and check
  board_type board{value_pairs};
  EXPECT_EQ(steps, min_steps(board, start, end));
}

}  // namespace
