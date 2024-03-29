/**
 * @file prefix_tree.h
 * @author Derek Huang
 * @brief C++ prefix tree + trie implementation
 * @copyright MIT License
 */

#ifndef PDDCP_PREFIX_TREE_H_
#define PDDCP_PREFIX_TREE_H_

#include <algorithm>
#include <memory>
#include <set>
#include <unordered_map>
#include <vector>

namespace pddcp {

/**
 * Template class for a prefix tree, better known as a trie.
 *
 * We need `trie` as a namespace name, so we use prefix tree here.
 *
 * `prefix_tree` is iterable, with `begin` and `end` iterators that allow
 * iteration through its underlying `unordered_map`.
 *
 * @tparam T type
 */
template <typename T>
class prefix_tree {
public:
  using value_type = T;
  using self_type = prefix_tree<T>;
  using lookup_map_type = std::unordered_map<T, std::unique_ptr<self_type>>;
  using iterator = typename lookup_map_type::iterator;
  using const_iterator = typename lookup_map_type::const_iterator;

  /**
   * Ctor.
   *
   * @param value Node value
   */
  prefix_tree(T value = {}) : value_{value}, lookup_{} {}

  /**
   * Return const reference to node value.
   */
  const auto& value() const { return value_; }

  /**
   * `true` if lookup map contains the prefix, `false` otherwise.
   *
   * @param key Search key
   */
  bool contains(const T& key) const
  {
// in C++20 we can use the contains() method
#if __cplusplus >= 202002L
    return lookup_.contains(key);
#else
    return lookup_.find(key) != lookup_.cend();
#endif  // __cplusplus < 202002L
  }

  /**
   * Return reference to node pointer associated with key.
   *
   * Wrapper over `operator[]` of the unordered map.
   *
   * @param key Search key
   */
  auto& at(const T& key) { return lookup_[key]; }
  auto& operator[](const T& key) { return at(key); }

  /**
   * Return const reference to node pointer associated with key if possible.
   *
   * @param key Search key
   */
  const auto& at(const T& key) const { return lookup_.at(key); }
  const auto& operator[](const T& key) const { return at(key); }

  /**
   * Return iterator to beginning of lookup map.
   */
  auto begin() noexcept { return lookup_.begin(); }
  auto begin() const noexcept { return lookup_.begin(); }

  /**
   * Return iterator to end of lookup map.
   */
  auto end() noexcept { return lookup_.end(); }
  auto end() const noexcept { return lookup_.end(); }

private:
  T value_;
  lookup_map_type lookup_;
};

namespace trie {

/**
 * Create a trie from a *Container* of strings.
 *
 * @tparam StringContainer *Container* with a `basic_string` specialization
 *
 * @param string_container *Container* of strings
 */
template <typename StringContainer>
auto from(const StringContainer& string_container)
{
  using StringType = typename StringContainer::value_type;
  using TrieType = prefix_tree<StringType>;
  TrieType root;
  auto temp = &root;
  // for each char for each string
  for (const auto& s : string_container) {
    for (const auto& c : s) {
      StringType prefix{c};
      // build the trie node by node if it doesn't exist
      if (!temp->contains(prefix)) {
        temp->at(prefix) = std::make_unique<TrieType>(prefix);
      }
      // update to continue our insertion
      temp = temp->at(prefix).get();
    }
    // reset temp
    temp = &root;
  }
  return root;
}

/**
 * Return `true` if the trie contains the prefix, `false` otherwise.
 *
 * @tparam StringType `basic_string` specialization
 *
 * @param root Trie root
 * @param prefix Prefix to search for in trie
 */
template <typename StringType>
auto contains(prefix_tree<StringType>* root, const StringType& prefix)
{
  // pointer to node with last character in prefix. if prefix is empty, then
  // sub_root will just be returned equal to root
  decltype(root) sub_root = root;
  // traverse tree depth until all characters are matched
  for (const auto& c : prefix) {
    StringType sub_prefix{c};
    if (sub_root->contains(sub_prefix))
      sub_root = sub_root->at(sub_prefix).get();
    // don't return early to allow auto deduction
    else {
      sub_root = nullptr;
      break;
    }
  }
  return sub_root;
}

/**
 * Return a vector of the values inserted into the trie using `from()`.
 *
 * Each element in the vector is the result of concatenating all the values
 * encountered in a path from the root to a leaf in the trie.
 *
 * If `root` is the address of a default-constructed `prefix_tree` or `nullptr`
 * then the returned vector is empty. Uses recursive DFS to retrieve values.
 *
 * @tparam StringType `basic_string` specialization
 *
 * @param root Trie root
 */
template <typename StringType>
auto to_vector(prefix_tree<StringType>* root)
{
  std::vector<StringType> values;
  // just return empty if root is nullptr
  if (!root)
    return values;
  // work recursively for each child, getting values stored in each path
  for (const auto& [key, child] : *root) {
    auto child_values = to_vector(child.get());
    // if child_values is empty, then that has no children. make child_values
    // just contain the empty string so we can prepend key properly
    if (child_values.empty())
      child_values = {{}};
    // need to prepend current search key to the child suffixes
    std::for_each(
      child_values.begin(),
      child_values.end(),
      // Clang errors as cannot implicitly capture structured binding in C++17.
      // this is allowed by GCC, but technically is not standard compliant.
#if __cplusplus >= 202002L
      []
#else
      [key = key]
#endif
      (auto& frag) { frag = key + frag; }
    );
    values.insert(values.cend(), child_values.cbegin(), child_values.cend());
  }
  return values;
}

/**
 * Return a set containing the values inserted into the trie using `from()`.
 *
 * Creates a `std::set` from the vector returned by `to_vector()`.
 *
 * @tparam StringType `basic_string` specialization
 *
 * @param root Trie root
 */
template <typename StringType>
inline auto to_set(prefix_tree<StringType>* root)
{
  auto value_vector = to_vector(root);
  return std::set<StringType>{value_vector.cbegin(), value_vector.cend()};
}

/**
 * Return autocomplete set available given a trie and a prefix.
 *
 * @tparam StringType `basic_string` specialization
 *
 * @param root Trie root
 * @param prefix Prefix to autocomplete
 * @returns `std::set<StringType>` of possible autocompletes
 */
template <typename StringType>
auto autocomplete(prefix_tree<StringType>* root, const StringType& prefix)
{
  using result_type = std::set<StringType>;
  // check if prefix exists in the trie. if not, then no autocomplete
  auto search_root = contains(root, prefix);
  if (!search_root)
    return result_type{};
  // otherwise, traverse depth-first to find all the value suffixes before
  // prepending the search prefix to them to get the full value
  auto child_values = to_vector(search_root);
  std::for_each(
    child_values.begin(),
    child_values.end(),
    [&](auto& frag) { frag = prefix + frag; }
  );
  return result_type{child_values.cbegin(), child_values.cend()};
}

}  // namespace trie

}  // namespace pddcp

#endif  // PDDCP_PREFIX_TREE_H_
