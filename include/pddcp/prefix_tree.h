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
   * @param key Search key
   */
  auto& at(const T& key) { return lookup_.at(key); }
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
  TrieType* temp = &root;
  // for each char for each string
  for (const auto& s : string_container) {
    for (const auto& c : s) {
      StringType prefix{c};
      // build the trie node by node and update temp
      if (!temp->contains(prefix)) {
        temp->at(prefix) = std::make_unique<TrieType>(prefix);
        temp = temp->at(prefix).get();
      }
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
    if (sub_root->contains(c))
      sub_root = sub_root->at(c).get();
    // don't return early to allow auto deduction
    else {
      sub_root = nullptr;
      break;
    }
  }
  return sub_root;
}

/**
 * Return trie contents as a vector of values.
 *
 * Each element in the vector is the result of concatenating all the values
 * encountered in a path from the root to a leaf in the trie.
 *
 * @tparam StringType `basic_string` specialization
 *
 * @param root Trie root
 */
template <typename StringType>
auto contents(prefix_tree<StringType>* root)
{
  std::vector<StringType> values;
  if (!root)
    return values;
  // work recurisvely for each child
  for (const auto& [key, child] : *root) {
    auto child_values = contents(child.get());
    // need to prepend current search key to the child suffixes
    std::for_each(
      child_values.begin(),
      child_values.end(),
      [&](auto& frag) { frag = key + frag; }
    );
    values.insert(values.cend(), child_values.cbegin(), child_values.cend());
  }
  // need to prepend current node value to child suffixes
  std::for_each(
    values.begin(),
    values.end(),
    [&](auto& frag) { frag = root->value() + frag; }
  );
  return values;
}

/**
 * Return autocomplete available given a trie and a prefix.
 *
 * @tparam StringType `basic_string` specialization
 *
 * @param root Trie root
 * @param prefix Prefix to autocomplete
 * @returns `std::vector<StringType>` of possible autocompletes
 */
template <typename StringType>
auto autocomplete(prefix_tree<StringType>* root, const StringType& prefix)
{
  std::vector<StringType> values;
  // check if prefix exists in the trie. if not, then no autocomplete
  auto search_root = contains(root, prefix);
  if (!search_root)
    return values;
  // otherwise, traverse depth-first to find all the values
  return contents(search_root);
}

}  // namespace trie

}  // namespace pddcp

#endif  // PDDCP_PREFIX_TREE_H_
