/**
 * @file binary_tree_test.cc
 * @author Derek Huang
 * @brief binary_tree.h unit tests
 * @copyright MIT License
 */

#include "pddcp/binary_tree.h"

#include <vector>

#include <gtest/gtest.h>

namespace {

/**
 * Test fixture for binary tree tests.
 */
class BinaryTreeTest : public ::testing::Test {
protected:
  using value_type = int;
  using binary_tree_type = pddcp::binary_tree<value_type>;

  /**
   * Ctor.
   *
   * Initializes a fresh binary [search] tree rooted at `root_` for each test.
   * The binary [search] tree, when drawn out, looks like this:
   *
   *      7
   *     / \
   *    4   17
   *       /  \
   *     11    19
   */
  BinaryTreeTest()
    : root_{
      7,
      new binary_tree_type{4},
      new binary_tree_type{17, new binary_tree_type{11}, new binary_tree_type{19}}
    }
  {}

  binary_tree_type root_;

  // static binary trees for testing the pddcp::bst::check function. g_root_1_
  // is a binary search tree but g_root_2_ and g_root_3_ are not.
  static inline const binary_tree_type g_root_1_{10};
  static inline const binary_tree_type g_root_2_{
    10, new binary_tree_type{19}, new binary_tree_type{15}
  };
  static inline const binary_tree_type g_root_3_{
    10,
    new binary_tree_type{19},
    new binary_tree_type{20, new binary_tree_type{15}, new binary_tree_type{21}}
  };
  // values that can be inserted into root_ for testing insert
  static inline const std::vector<value_type> extra_values_{1, 5, 12, 20, 15};
};

/**
 * Test that `set_value` works as expected.
 */
TEST_F(BinaryTreeTest, SetValueTest)
{
  const auto old_root_value = root_.value();
  EXPECT_EQ(old_root_value, root_.set_value(8));
  EXPECT_EQ(8, root_.value());
}

/**
 * Test that `set_left` works as expected.
 */
TEST_F(BinaryTreeTest, SetLeftTest)
{
  // replace left child and validate it
  auto old_left = root_.set_left(new binary_tree_type{5});
  EXPECT_EQ(4, old_left->value());
  EXPECT_EQ(nullptr, old_left->left());
  EXPECT_EQ(nullptr, old_left->right());
  // check new left child is as expected
  EXPECT_EQ(5, root_.left_value());
}

/**
 * Test that `set_right` works as expected.
 */
TEST_F(BinaryTreeTest, SetRightTest)
{
  // replace right subtree and validate it
  auto old_right = root_.set_right(new binary_tree_type{18});
  // validate old right child
  EXPECT_EQ(17, old_right->value());
  EXPECT_EQ(11, old_right->left_value());
  EXPECT_EQ(19, old_right->right_value());
  // check new right child is as expected
  EXPECT_EQ(18, root_.right_value());
}

/**
 * Test that `set_left_value` works as expected.
 *
 * Also tests node creation when it is called off the root's left child.
 */
TEST_F(BinaryTreeTest, SetLeftValueTest)
{
  // set left child value
  const auto old_left_value = root_.left_value();
  EXPECT_EQ(old_left_value, *root_.set_left_value(5));
  EXPECT_EQ(5, root_.left_value());
  // create and set left child of left child
  EXPECT_FALSE(root_.left()->set_left_value(3));
  EXPECT_EQ(3, root_.left()->left_value());
}

/**
 * Test that `set_right_value` works as expected.
 *
 * Also tests node creation when it is called off the root's left child.
 */
TEST_F(BinaryTreeTest, SetRightValueTest)
{
  // set right child value
  const auto old_right_value = root_.right_value();
  EXPECT_EQ(old_right_value, *root_.set_right_value(18));
  EXPECT_EQ(18, root_.right_value());
  // create and set right child of left child
  EXPECT_FALSE(root_.left()->set_right_value(6));
  EXPECT_EQ(6, root_.left()->right_value());
}

/**
 * Test that the single `insert` overload works as expected.
 */
TEST_F(BinaryTreeTest, InsertSingleTest)
{
  // 5 should be inserted as right child of left child
  EXPECT_EQ(5, pddcp::bst::insert(&root_, 5)->left()->right_value());
  // 18 should be inserted as left child of right child's right child
  EXPECT_EQ(18, pddcp::bst::insert(&root_, 18)->right()->right()->left_value());
}

/**
 * Test that the multiple `insert` overload works as expected.
 */
TEST_F(BinaryTreeTest, InsertMultipleTest)
{
  // 5 and 18 should be inserted in same places as in InsertSingleTest
  pddcp::bst::insert(&root_, {5, 18});
  EXPECT_EQ(5, root_.left()->right_value());
  EXPECT_EQ(18, root_.right()->right()->left_value());
}

/**
 * Test that the binary search tree `check` function works as expected.
 */
TEST_F(BinaryTreeTest, CheckTest)
{
  // root_ and g_root_1_ are roots for binary search trees
  EXPECT_TRUE(pddcp::bst::check(&root_));
  EXPECT_TRUE(pddcp::bst::check(&g_root_1_));
  // but g_root_2_ and g_root_3_ are not roots for binary search trees
  EXPECT_FALSE(pddcp::bst::check(&g_root_2_));
  EXPECT_FALSE(pddcp::bst::check(&g_root_3_));
}

/**
 * Test that the multiple `insert` overload produces binary search trees.
 *
 * This is indirectly tested in InsertSingleTest and InsertMultipleTest, but we
 * use a separate test case since `extra_values_` has a bit more values.
 */
TEST_F(BinaryTreeTest, CheckInsertMultipleTest)
{
  EXPECT_TRUE(pddcp::bst::check(pddcp::bst::insert(&root_, extra_values_)));
}

}  // namespace
