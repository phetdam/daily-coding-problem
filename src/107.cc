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
using tree_type = pddcp::binary_tree<int>;
// we have a separate tree_type and input_type because unique pointers are not
// copyable, so passing a binary_tree<T> into ::testing::Values, which takes
// parameters by copy, triggers a compile error. therefore, we have to create
// the trees in global scope and pass a const reference or pointer instead.
using input_type = const tree_type&;
using result_type = std::vector<int>;
using pair_type = std::pair<input_type, result_type>;

/**
 * Test fixture class for parametrized testing.
 */
class DailyTest107 : public ::testing::TestWithParam<pair_type> {};

/**
 *    1
 *   / \
 *  2   3
 *     / \
 *    4   5
 */
const tree_type sample_tree{
  1, new tree_type{2}, new tree_type{3, new tree_type{4}, new tree_type{5}}
};
INSTANTIATE_TEST_SUITE_P(
  SamplePairs,
  DailyTest107,
  ::testing::Values(
    pair_type{
      sample_tree,
      {1, 2, 3, 4, 5}
    }
  )
);

/**
 *     6
 *    / \
 *   1   9
 *  /   / \
 * 5   11  7
 */
const tree_type custom_tree{
  6,
  new tree_type{1, new tree_type{5}, nullptr},
  new tree_type{9, new tree_type{11}, new tree_type{7}}
};
INSTANTIATE_TEST_SUITE_P(
  CustomPairs,
  DailyTest107,
  ::testing::Values(
    pair_type{
      custom_tree,
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
