/**
 * @file 89.cc
 * @author Derek Huang
 * @brief #89 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by LinkedIn.
 *
 * Determine whether a tree is a valid binary search tree.
 *
 * A binary search tree is a tree with two children, left and right, and
 * satisfies the constraint that the key in the left child must be less than or
 * equal to the root and the key in the right child must be greater than or
 * equal to the root.
 */

#include <gtest/gtest.h>
#include <utility>
#include <vector>

#include "pddcp/binary_tree.h"

namespace {

// convenience type aliases
using binary_tree_type = pddcp::binary_tree<int>;
using input_type = binary_tree_type;
using result_type = bool;
using pair_type = std::pair<input_type, result_type>;

/**
 * Test fixture class.
 *
 * The binary tree class is not `constexpr` as unique pointers can't be copied,
 * so instead of using `TEST_P` we have to manually enumerate the test cases.
 */
class DailyTest89 : public ::testing::Test {
protected:
  // single node counts as a binary search tree
  static inline const pair_type case_1_{binary_tree_type{1}, true};
  /**
   *        1
   *      /   \
   *    -2     7
   *   /  \   / \
   * -7    0 5   11
   *            /  \
   *           9    13
   */
  static inline const pair_type case_2_{
    binary_tree_type{
      1,
      new binary_tree_type{
        -2, new binary_tree_type{-7}, new binary_tree_type{0}
      },
      new binary_tree_type{
        7,
        new binary_tree_type{5},
        new binary_tree_type{
          11, new binary_tree_type{9}, new binary_tree_type{13}
        }
      }
    },
    true
  };
  /**
   *      1
   *     / \
   *    2   3
   *   / \
   *  5   10
   */
  static inline const pair_type case_3_{
    binary_tree_type{
      1,
      new binary_tree_type{
        2, new binary_tree_type{5}, new binary_tree_type{10}
      },
      new binary_tree_type{3}
    },
    false
  };
};

}  // namespace

/**
 * Macro for defining a `CaseTest`.
 *
 * Defines a test to check that `pddcp::bst::check` works as expected.
 *
 * Trying to reduce some fingerwork since we have to manually parametrize, as
 * the `binary_tree<T>` is not `constexpr` due to use of `std::unique_ptr`.
 *
 * @param n integer case number
 */
#define DAILY_TEST_89_CASE_TEST(n) \
  TEST_F(DailyTest89, CaseTest ## n) \
  { \
    const auto& [root, truth] = case_ ## n ## _; \
    EXPECT_EQ(pddcp::bst::check(&root), truth); \
  }

namespace {

DAILY_TEST_89_CASE_TEST(1)
DAILY_TEST_89_CASE_TEST(2)
DAILY_TEST_89_CASE_TEST(3)

}  // namespace
