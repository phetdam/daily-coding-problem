/**
 * @file 107.cc
 * @author Derek Huang
 * @brief #107 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Microsoft.
 *
 * Print the nodes in a binary tree level-wise. For example, the following
 * should print 1, 2, 3, 4, 5.
 *
 *    1
 *   / \
 *  2   3
 *     / \
 *    4   5
 */

#include <sstream>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "pddcp/binary_tree.h"
#include "pddcp/string.h"

namespace {

// convenience type aliases
using binary_tree_type = pddcp::binary_tree<int>;
// we have a separate binary_tree_type and input_type because unique pointers
// are not copyable, so passing a binary_tree<T> into ::testing::Values, which
// takes parameters by copy, triggers a compile error.
using input_type = binary_tree_type*;
using result_type = std::vector<int>;
using pair_type = std::pair<input_type, result_type>;

/**
 * Test fixture class for parametrized testing.
 */
class DailyTest107 : public ::testing::TestWithParam<pair_type> {};

INSTANTIATE_TEST_SUITE_P(
  SamplePairs,
  DailyTest107,
  ::testing::Values(
    // not a memory leak; it is as if we just made some static binary tree
    pair_type{
      new binary_tree_type{
        1,
        new binary_tree_type{2},
        new binary_tree_type{3, new binary_tree_type{4}, new binary_tree_type{5}}
      },
      {1, 2, 3, 4, 5}
    }
  )
);

INSTANTIATE_TEST_SUITE_P(
  CustomPairs,
  DailyTest107,
  ::testing::Values(
    /**
     *     6
     *    / \
     *   1   9
     *  /   / \
     * 5   11  7
     */
    pair_type{
      new binary_tree_type{
        6,
        new binary_tree_type{1, new binary_tree_type{5}, nullptr},
        new binary_tree_type{9, new binary_tree_type{11}, new binary_tree_type{7}},
      },
      {6, 1, 9, 5, 11, 7}
    }
  )
);

/**
 * Test that `pddcp::bfs` overload returning a vector works as expected.
 */
TEST_P(DailyTest107, VectorParamTest)
{
  EXPECT_EQ(GetParam().second, pddcp::bfs(GetParam().first));
}

/**
 * Test that `pddcp::bfs` overload writing to a stream works as expected.
 */
TEST_P(DailyTest107, StreamParamTest)
{
  std::stringstream stream;
  pddcp::bfs(stream, GetParam().first);
  // don't forget trailing newline that is added by bfs call
  EXPECT_EQ(pddcp::string_join(GetParam().second, "\n") + "\n", stream.str());
}

}  // namespace
