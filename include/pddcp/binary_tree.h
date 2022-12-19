/**
 * @file binary_tree.h
 * @author Derek Huang
 * @brief C++ binary tree implementation
 * @copyright MIT License
 */

#ifndef PDDCP_BINARY_TREE_H_
#define PDDCP_BINARY_TREE_H_

#include <memory>
#include <optional>
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
  using self_type = binary_tree<T>;

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
   * @param left `self_type*` left child to take ownership of
   * @param right `self_type*` right child to take ownership of
   */
  binary_tree(T value, self_type* left, self_type* right)
    : value_(value), left_(left), right_(right)
  {}

  // getters, including convenience getters for left + right values
  const T& value() const noexcept { return value_; }
  self_type* left() const noexcept { return left_.get(); }
  const T& left_value() const { return left_->value_; }
  self_type* right() const noexcept { return right_.get(); }
  const T& right_value() const { return right_->value_; }

  /**
   * Update the node value.
   *
   * @param new_value `T` new node value
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
   * @param new_left `self_type*` to take ownership of as left subtree
   * @returns `std::unique_ptr<self_type>` owning old left subtree
   */
  auto set_left(self_type* new_left)
  {
    std::unique_ptr<self_type> old_left{left_.release()};
    left_.reset(new_left);
    return old_left;
  }

  /**
   * Update the value of the left child.
   *
   * If there is no left child, it will be created.
   *
   * @param new_value `T` new left node value
   * @returns `std::optional<T>` containing previous left node value. If there
   *  was no previous left node, then the `std::optional<T>` is empty.
   */
  std::optional<T> set_left_value(T new_value)
  {
    if (!left_.get()) {
      left_.reset(new binary_tree{new_value});
      return {};
    }
    return left_->set_value(new_value);
  }

  /**
   * Update the right subtree by taking ownership of a node pointer.
   *
   * @param new_right `self_type*` to take ownership of as right subtree
   * @returns `std::unique_ptr<self_type>` owning old right subtree
   */
  auto set_right(self_type* new_right)
  {
    std::unique_ptr<self_type> old_right{right_.release()};
    right_.reset(new_right);
    return old_right;
  }

    /**
   * Update the value of the right child.
   *
   * If there is no right child, it will be created.
   *
   * @param new_value `T` new right node value
   * @returns `std::optional<T>` containing previous right node value. If there
   *  was no previous right node, then the `std::optional<T>` is empty.
   */
  std::optional<T> set_right_value(T new_value)
  {
    if (!right_.get()) {
      right_.reset(new binary_tree{new_value});
      return {};
    }
    return right_->set_value(new_value);
  }

private:
  T value_;
  std::unique_ptr<self_type> left_;
  std::unique_ptr<self_type> right_;
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
  // note: for both left and right inserts, we could use single-line if-else,
  // but we want to allow auto deduction and so have explicit returns
  // left insert
  if (value < root->value()) {
    if (!root->left()) {
      root->set_left(new binary_tree(value));
      return root;
    }
    insert(root->left(), value);
    return root;
  }
  // right insert
  if (!root->right()) {
    root->set_right(new binary_tree(value));
    return root;
  }
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
auto check(const binary_tree<T>* root)
{
  // false on NULL
  if (!root)
    return false;
  // check left and right values
  if (
    root->left() && root->left_value() >= root->value() ||
    root->right() && root->right_value() < root->value()
  )
    return false;
  // check recursively, although if subtree is NULL, treat as true
  auto left_ok = (root->left()) ? check(root->left()) : true;
  // if left is not ok, we can return false earlier before evaluting right
  if (!left_ok)
    return false;
  // otherwise, left is ok, only need to check right
  auto right_ok = (root->right()) ? check(root->right()) : true;
  if (!right_ok)
    return false;
  return true;
}

}  // namespace bst

}  // namespace pddcp

#endif  // PDDCP_BINARY_TREE_H_
