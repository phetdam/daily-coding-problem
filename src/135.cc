/**
 * @file 135.cc
 * @author Derek Huang
 * @brief #135 from Daily Coding Problem
 * @copyright MIT License
 *
 * This question was asked by Apple.
 *
 * Given a binary tree, find a minimum path sum from root to a leaf.
 *
 * For example, the minimum path in this tree is [10, 5, 1, -1], which has sum
 * 15.
 *
 *    10
 *   /  \
 *  5    5
 *   \    \
 *    2    1
 *        /
 *      -1
 */

#include <utility>

#include <gtest/gtest.h>

#include "pddcp/binary_tree.h"

namespace {

// convenience type aliases
using binary_tree_type = pddcp::binary_tree<int>;
using input_type = binary_tree_type;
using result_type = typename input_type::value_type;
using pair_type = std::pair<input_type, result_type>;

/**
 * Test fixture.
 *
 * We can't use `TEST_P` because `binary_tree<T>` is not copyable since it uses
 * `std::unique_ptr<T>` to manage child node memory.
 */
class DailyTest135 : public ::testing::Test {
protected:
  /**
   *    10
   *   /  \
   *  5    5
   *   \    \
   *    2    1
   *        /
   *      -1
   *
   * min cost: 15
   */
  static inline const pair_type case_1_{
    binary_tree_type{
      10,
      new binary_tree_type{5, nullptr, new binary_tree_type{2}},
      new binary_tree_type{
        5,
        nullptr,
        new binary_tree_type{1, new binary_tree_type{-1}, nullptr}
      }
    },
    15
  };
  /**
   *      4
   *     / \
   *   -3   1
   *   /   /
   * -1  -9
   *
   * min cost: -4
   */
  static inline const pair_type case_2_{
    binary_tree_type{
      4,
      new binary_tree_type{-3, new binary_tree_type{-1}, nullptr},
      new binary_tree_type{1, new binary_tree_type{-9}, nullptr}
    },
    -4
  };
  /**
   *     15
   *    /  \
   *  -5    2
   *  / \    \
   * 3  -1   -3
   *         / \
   *       -9  -2
   *           /
   *         -8
   *
   * min cost: 4
   */
  static inline const pair_type case_3_{
    binary_tree_type{
      15,
      new binary_tree_type{-5, new binary_tree_type{3}, new binary_tree_type{-1}},
      new binary_tree_type{
        2,
        nullptr,
        new binary_tree_type{
          -3,
          new binary_tree_type{-9},
          new binary_tree_type{-2, new binary_tree_type{-8}, nullptr}
        }
      }
    },
    4
  };
};

/**
 * Macro for defining a `CaseTest`.
 *
 * Defines a test to check that `pddcp::bt::min_path` works as expected.
 *
 * Trying to reduce some fingerwork since we have to manually parametrize, as
 * the `binary_tree<T>` is not copyable due to use of `std::unique_ptr`.
 *
 * @param n integer case number
 */
#define DAILY_TEST_135_CASE_TEST(n) \
  TEST_F(DailyTest135, CaseTest ## n) \
  { \
    const auto& [root, cost] = case_ ## n ## _; \
    EXPECT_EQ(cost, pddcp::bt::min_path(&root)); \
  }

DAILY_TEST_135_CASE_TEST(1)
DAILY_TEST_135_CASE_TEST(2)
DAILY_TEST_135_CASE_TEST(3)

}  // namespace
