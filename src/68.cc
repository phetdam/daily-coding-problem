/**
 * @file 68.cc
 * @author Derek Huang
 * @brief #68 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Google.
 *
 * On our special chessboard, two bishops attack each other if they share the
 * same diagonal. This includes bishops that have another bishop located
 * between them, i.e. bishops can attack through pieces. You are given N
 * bishops, represented as (row, column) tuples on a M by M chessboard. Write a
 * function to count the number of pairs of bishops that attack each other. The
 * ordering of the pair doesn't matter: (1, 2) is considered the same as (2, 1).
 *
 * For example, given M = 5 and the list of bishops:
 *
 * (0, 0)
 * (1, 2)
 * (2, 2)
 * (4, 0)
 *
 * The board would look like this:
 * [b 0 0 0 0]
 * [0 0 b 0 0]
 * [0 0 b 0 0]
 * [0 0 0 0 0]
 * [b 0 0 0 0]
 *
 * You should return 2, since bishops 1 and 3 attack each other, as well as
 * bishops 3 and 4.
 */

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <tuple>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "pddcp/type_traits.h"

namespace {

/**
 * Return the number of bishop pairs that are mutually attacking each other.
 *
 * Bishops attack along diagonals and can attack through other bishops. This
 * function does not take a board dimension; the board dimension can be easily
 * extended to encompass the bishop furthest from the origin.
 *
 * @tparam InputIt *LegacyInputIterator* with pair-like value type
 *
 * @param first Iterator to first bishop coordinate
 * @param last Iterator one past last bishop coordinate
 */
template <typename InputIt>
auto bishop_attacks(InputIt first, InputIt last)
{
  // check that the iterator type is a pair-like type with homogenous types
  using value_type = typename std::iterator_traits<InputIt>::value_type;
  static_assert(pddcp::is_homogenous_pair_v<value_type>);
  // iterate over all N * (N - 1) / 2 pairs to count attacking pairs
  std::size_t n_attacking = 0;
  for (auto outer = first; outer < last; outer++) {
    auto inner = outer;
    for (++inner; inner < last; inner++) {
      // use structured binding to reference elements
      const auto& [outer_0, outer_1] = *outer;
      const auto& [inner_0, inner_1] = *inner;
      // avoid using absolute value in case types are unsigned
      auto diff_0 = (outer_0 < inner_0) ? inner_0 - outer_0 : outer_0 - inner_0;
      auto diff_1 = (outer_1 < inner_1) ? inner_1 - outer_1 : outer_1 - inner_1;
      // bishops attack diagonals through pieces, i.e. diff_0 == diff_1
      if (diff_0 == diff_1)
        n_attacking++;
    }
  }
  return n_attacking;
}

/**
 * Return the number of bishop pairs that are mutually attacking each other.
 *
 * @tparam Container *Container* with pair-like `value_type`
 *
 * @param locs Container of bishop coordinates
 */
template <typename Container>
inline auto bishop_attacks(const Container& locs)
{
  return bishop_attacks(locs.begin(), locs.end());
}

// convenience type aliases
using input_type = std::vector<std::pair<unsigned int, unsigned int>>;
using result_type = std::size_t;
using pair_type = std::pair<input_type, result_type>;

/**
 * Test fixture class for parametrized testing.
 */
class DailyTest68 : public ::testing::TestWithParam<pair_type> {};

INSTANTIATE_TEST_SUITE_P(
  SamplePairs,
  DailyTest68,
  ::testing::Values(pair_type{{{0, 0}, {1, 2}, {2, 2}, {4, 0}}, 2})
);

/**
 * Test that `bishop_attacks` works correctly.
 */
TEST_P(DailyTest68, ParamTest)
{
  EXPECT_EQ(GetParam().second, bishop_attacks(GetParam().first));
}

}  // namespace
