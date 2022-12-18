/**
 * @file binary_tree.h
 * @author Derek Huang
 * @brief C++ binary tree implementation
 * @copyright MIT License
 */

#ifndef PDDCP_BINARY_TREE_H_
#define PDDCP_BINARY_TREE_H_

#include <memory>
#include <vector>

namespace pddcp {

/**
 * Simple binary tree class.
 *
 * @tparam T type
 */
template <typename T>
class binary_tree {
public:
  using value_type = T;

  /**
   * Create binary tree node without children.
   *
   * @param value node value
   */
  binary_tree(T value) : binary_tree(value, nullptr, nullptr) {}

  /**
   * Create binary tree node with children.
   *
   * The left and right children must not be owned by other smart pointers.
   *
   * @param value node value
   * @param left `binary_tree*` left child to take ownership of
   * @param right `binary_tree*` right child to take ownership of
   */
  binary_tree(T value, binary_tree* left, binary_tree* right)
    : value_(value), left_(left), right_(right)
  {}

  // getters, including convenience getters for left + right values
  const T& value() const noexcept { return value_; }
  binary_tree* left() const noexcept { return left_.get(); }
  const T& left_value() const { return left_->value_; }
  binary_tree* right() const noexcept { return right_.get(); }
  const T& right_value() const { return right_->value_; }

  /**
   * Update the node value.
   *
   * @param value `T` new node value
   * @returns the previous node value
   */
  auto set_value(T new_value)
  {
    auto old_value = value_;
    value_ = new_value;
    return old_value;
  }

  /**
   * Update the left subtree by taking ownership of a node pointer.
   *
   * @param new_left `binary_tree*` to take ownership of as left subtree
   * @returns `std::unique_ptr<binary_tree>` owning old left subtree
   */
  auto set_left(binary_tree* new_left)
  {
    auto old_left = std::make_unique<binary_tree>(left_.release());
    left_.reset(new_left);
    return old_left;
  }

  /**
   * Update the right subtree by taking ownership of a node pointer.
   *
   * @param new_right `binary_tree*` to take ownership of as right subtree
   * @returns `std::unique_ptr<binary_tree>` owning old right subtree
   */
  auto set_right(binary_tree* new_right)
  {
    auto old_right = std::make_unique<binary_tree>(right_.release());
    right_.reset(new_right);
    return old_right;
  }

private:
  T value_;
  std::unique_ptr<binary_tree> left_;
  std::unique_ptr<binary_tree> right_;
};

namespace bst {

/**
 * Insert a single value into the binary search tree.
 *
 * Duplicate values are always inserted as right children.
 *
 * @tparam T value type
 *
 * @param value value to insert
 * @returns `root` to allow method chaining
 */
template <typename T>
auto insert(binary_tree<T>* root, T value)
{
  // left insert
  if (value < root->value()) {
    if (!root->left())
      root->set_left(new binary_tree(value));
    else
      insert(root->left(), value);
    return root;
  }
  // right insert
  if (!root->right())
    root->set_right(new binary_tree(value));
  else
    insert(root->right(), value);
  return root;
}

/**
 * Insert multiple values into the binary search tree.
 *
 * Duplicate values are always inserted as right children.
 *
 * @tparam T value type
 *
 * @param values vector of `T` values to insert
 * @returns `this` to allow method chaining
 */
template <typename T>
inline auto insert(binary_tree<T>* root, const std::vector<T>& values)
{
  for (const auto& value : values)
    insert(root, value);
  return root;
}

/**
 * Verify that a tree is a binary search tree.
 *
 * @tparam T value type
 *
 * @param root root of binary tree to verify
 */
template <typename T>
bool check(const binary_tree<T>* root)
{
  // false on NULL
  if (!root)
    return false;
  // check left and right values
  if (
    root.left() && root.left_value() >= root.value() ||
    root.right() && root.right_value() < root.value()
  )
    return false;
  // check recursively, although if subtree is NULL, treat as true
  auto left_ok = (root.left()) ? is_bst(root.left()) : true;
  // if left is not ok, we can return false earlier before evaluting right
  if (!left_ok)
    return false;
  // otherwise, left is ok, only need to check right
  auto right_ok = (root.right()) ? is_bst(root.right()) : true;
  if (!right_ok)
    return false;
  return true;
}

}  // namespace bst

}  // namespace pddcp

#endif  // PDDCP_BINARY_TREE_H_
