/**
 * @file binary_tree.h
 * @author Derek Huang
 * @brief C++ binary tree implementation
 * @copyright MIT License
 */

#ifndef PDDCP_BINARY_TREE_H_
#define PDDCP_BINARY_TREE_H_

#include <algorithm>
#include <deque>
#include <initializer_list>
#include <memory>
#include <optional>
#include <ostream>
#include <type_traits>
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
  binary_tree(T value) : binary_tree{value, nullptr, nullptr} {}

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

  /**
   * Create binary tree node with children.
   *
   * @param value node value
   * @param left Owned left child to take ownership of
   * @param right Owned right child to take ownership of
   */
  binary_tree(
    T value,
    std::unique_ptr<self_type>& left,
    std::unique_ptr<self_type>& right)
    : binary_tree{value, left.release(), right.release()}
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

// TODO: consider removing tree namespace, minimum path cost is something
// generic that could be applied to graphs, non-binary trees, etc.
namespace tree {

/**
 * Returns the minimum cost of travelling from root to leaf in a binary tree.
 *
 * @tparam T value type
 *
 * @param root Binary tree root
 */
template <typename T>
auto min_path(const binary_tree<T>* root)
{
  // assume no cost for nullptr
  if (!root)
    return T{};
  // left and right subtree cost can be zero if the subtree is missing
  auto left_cost = min_path(root->left());
  auto right_cost = min_path(root->right());
  // in which case, we return cost using the other subtree's possibly 0 cost
  if (!root->left())
    return root->value() + right_cost;
  if (!root->right())
    return root->value() + left_cost;
  // otherwise, we choose the lower subtree cost
  return root->value() + std::min(left_cost, right_cost);
}

/**
 * Returns the minimum cost of travelling from root to leaf in a binary tree.
 *
 * @tparam T value type
 *
 * @param root Binary tree root
 */
template <typename T>
inline auto min_path(const binary_tree<T>& root)
{
  return min_path(&root);
}

}  // namespace tree

/**
 * Perform a breadth-first search on the binary tree.
 *
 * @tparam T value type
 * @tparam Sink *Callable* that takes a `T` or `const T&`
 *
 * @param root Binary tree root
 * @param sink *Callable* used to handle each retrieved value
 * @returns Number of elements found in the tree
 */
template <typename T, typename Sink>
auto bfs(const binary_tree<T>* root, Sink sink)
{
  // queue for nodes + counter for number of elements in tree
  std::deque<const binary_tree<T>*> node_queue;
  std::size_t n_nodes = 0;
  // if there's no tree, there are no results
  if (!root)
    return n_nodes;
  // otherwise, proceed in a typical breadth-first search
  node_queue.push_back(root);
  while (node_queue.size()) {
    // place value of front node in the sink + increment node count
    auto node = node_queue.front();
    // values.emplace_back(node.value());
    sink(node->value());
    n_nodes++;
    // add left and right child if they exist
    if (node->left())
      node_queue.push_back(node->left());
    if (node->right())
      node_queue.push_back(node->right());
    // remove remaining front reference in queue
    node_queue.pop_front();
  }
  return n_nodes;
}

/**
 * Perform a breadth-first search on the binary tree.
 *
 * @tparam T value type
 * @tparam Sink *Callable* that takes a `T` or `const T&`
 *
 * @param root Binary tree root
 * @param sink *Callable* used to handle each retrieved value
 * @returns Number of elements found in the tree
 */
template <typename T, typename Sink>
inline auto bfs(const binary_tree<T>& root, Sink sink)
{
  return bfs(&root, sink);
}

/**
 * Return values from a breadth-first search in a container.
 *
 * @tparam Container *Container* with `T` as `value_type`
 * @tparam T value type
 *
 * @param root Binary tree root
 */
template <typename Container, typename T>
inline auto bfs(const binary_tree<T>* root)
{
  static_assert(std::is_same_v<T, typename Container::value_type>);
  Container values;
  bfs(root, [&values](const T& v) { values.emplace_back(v); });
  return values;
}

/**
 * Return values from a breadth-first search in a container.
 *
 * @tparam Container *Container* with `T` as `value_type`
 * @tparam T value type
 *
 * @param root Binary tree root
 */
template <typename Container, typename T>
inline auto bfs(const binary_tree<T>& root)
{
  return bfs<Container>(&root);
}

/**
 * Return values from a breadth-first search in a `std::vector<T>`.
 *
 * @tparam T value type
 *
 * @param root Binary tree root
 */
template <typename T>
inline auto bfs(const binary_tree<T>* root)
{
  return bfs<std::vector<T>>(root);
}

/**
 * Return values from a breadth-first search in a `std::vector<T>`.
 *
 * @tparam T value type
 *
 * @param root Binary tree root
 */
template <typename T>
inline auto bfs(const binary_tree<T>& root)
{
  return bfs(&root);
}

/**
 * Write values from a breadth-first search to a stream, one value per line.
 *
 * @tparam Char T Char type
 * @tparam Traits Char traits
 * @tparam T value type
 *
 * @param stream Output stream
 * @param root Binary tree root
 */
template <typename CharT, typename Traits, typename T>
inline auto bfs(
  std::basic_ostream<CharT, Traits>& stream,
  const binary_tree<T>* root)
{
  return bfs(
    root,
    [&stream](const T& v)
    {
      stream << v;
      stream.put(stream.widen('\n'));
    }
  );
}

/**
 * Write values from a breadth-first search to a stream, one value per line.
 *
 * @tparam Char T Char type
 * @tparam Traits Char traits
 * @tparam T value type
 *
 * @param stream Output stream
 * @param root Binary tree root
 */
template <typename CharT, typename Traits, typename T>
inline auto bfs(
  std::basic_ostream<CharT, Traits>& stream, const binary_tree<T>& root)
{
  return bfs(stream, &root);
}

namespace bst {

/**
 * Insert a single value into the binary search tree.
 *
 * Duplicate values are always inserted as right children.
 *
 * @tparam T value type
 *
 * @param root Binary tree root
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
 * @tparam ContainerType *Container* with `T` as `value_type`
 *
 * @param root Binary tree root
 * @param values *Container* of `T` values to insert
 * @returns `this` to allow method chaining
 */
template <typename T, typename ContainerType>
inline auto insert(binary_tree<T>* root, const ContainerType& values)
{
  static_assert(std::is_same_v<T, typename ContainerType::value_type>);
  for (const auto& value : values)
    insert(root, value);
  return root;
}

/**
 * Insert multiple values into the binary search tree.
 *
 * Duplicate values are always inserted as right children.
 *
 * @tparam T value type
 *
 * @param root Binary tree root
 * @param values initializer list of `T` values to insert
 * @returns `this` to allow method chaining
 */
template <typename T>
inline auto insert(binary_tree<T>* root, const std::initializer_list<T>& values)
{
  return insert(root, std::vector<T>{values});
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
