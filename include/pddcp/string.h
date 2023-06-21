/**
 * @file string.h
 * @author Derek Huang
 * @brief C++ header for string operations
 * @copyright MIT License
 */

#ifndef PDDCP_STRING_H_
#define PDDCP_STRING_H_

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "pddcp/enums.h"

namespace pddcp {

/**
 * Join a container of values into a string.
 *
 * @note This function does work properly when `CharT` is not `char`.
 *
 * @tparam Container *Container* with `value_type` compatible with `operator<<`
 * @tparam CharT Char type
 * @tparam Traits Char traits
 * @tparam Alloc Allocator
 *
 * @param values Values to join into string
 * @param delim Delimiter to use when joining values
 */
template <typename Container, typename CharT, typename Traits, typename Alloc>
auto string_join(
  const Container& values,
  const std::basic_string<CharT, Traits, Alloc>& delim)
{
  std::basic_stringstream<CharT, Traits, Alloc> stream;
  for (auto iter = values.begin(); iter != values.end(); iter++) {
    stream << *iter;
    if (std::distance(iter, values.end()) == 1)
      break;
    stream << delim;
  }
  return stream.str();
}

/**
 * Join a container of values into a string.
 *
 * @tparam Container *Container* with `value_type` compatible with `operator<<`
 * @tparam CharT Char type
 *
 * @param values Values to join into string
 * @param delim Delimiter to use when joining values
 */
template <typename Container, typename CharT>
inline auto string_join(const Container& values, const CharT* delim)
{
  return string_join(values, std::basic_string<CharT>{delim});
}

/**
 * Join a container of values into a `std::string` with no delimiter.
 *
 * @tparam Container *Container* with `value_type` compatible with `operator<<`
 *
 * @param values Values to join into string
 */
template <typename Container>
inline auto string_join(const Container& values)
{
  return string_join(values, "");
}

/**
 * Check that a string is a palindrome.
 *
 * @tparam CharT Char type
 * @tparam Traits Char traits
 * @tparam Alloc Allocator
 *
 * @param str String to check
 */
template <typename CharT, typename Traits, typename Alloc>
bool is_palindrome(const std::basic_string<CharT, Traits, Alloc>& str)
{
  // empty string is not considered palindrome
  if (str.empty())
    return false;
  // right and left endpoints of string
  auto right_i = str.size() - 1;
  decltype(right_i) left_i = 0;
  // move towards center to check
  while (left_i < right_i)
    if (str[left_i++] != str[right_i--])
      return false;
  return true;
}

/**
 * Return the Levenshtein distance between two string views.
 *
 * This is the naive recursive computation of the Levenshtein distance
 * implemented by following its recurrence relation. The complexity is
 * obviously O(3^N), which is exponential time complexity.
 *
 * Views are used to eliminate copying and are much more efficient that
 * operating on strings as each substring requires another memory allocation.
 *
 * @tparam CharT Char type
 * @tparam Traits Char traits type
 *
 * @param a First string view
 * @param b Second string view
 */
template <typename CharT, typename Traits>
auto rr_levenshtein_distance(
  const std::basic_string_view<CharT, Traits>& a,
  const std::basic_string_view<CharT, Traits>& b)
{
  // if either is empty, return size of other string
  if (b.empty())
    return a.size();
  if (a.empty())
    return b.size();
  // tails for first and second string
  auto tail_a = a.substr(1);
  auto tail_b = b.substr(1);
  // if first chars are equal, compute distance for the remainders (tails)
  if (a[0] == b[0])
    return rr_levenshtein_distance(tail_a, tail_b);
  // consider all cases of computing the distance with or without the first
  // characters of the two strings, i.e. (tail_a, b), (a, tail_b),
  // (tail_a, tail_b). add one to the result is because we remove one char.
  auto lev_a = rr_levenshtein_distance(tail_a, b);
  auto lev_b = rr_levenshtein_distance(a, tail_b);
  auto lev_ab = rr_levenshtein_distance(tail_a, tail_b);
  // of course, we would want to pick their minimum
  return 1 + std::min({lev_a, lev_b, lev_ab});
}

/**
 * Return the Levenshtein distance between two strings.
 *
 * This is the naive recursive computation of the Levenshtein distance
 * implemented by following its recurrence relation. The complexity is
 * obviously O(3^N), which is exponential time complexity.
 *
 * @tparam CharT Char type
 * @tparam Traits Char traits type
 * @tparam Alloc Allocator
 *
 * @param a First string
 * @param b Second string
 */
template <typename CharT, typename Traits, typename Alloc>
inline auto rr_levenshtein_distance(
  const std::basic_string<CharT, Traits, Alloc>& a,
  const std::basic_string<CharT, Traits, Alloc>& b)
{
  using view_type = std::basic_string_view<CharT, Traits>;
  return rr_levenshtein_distance(view_type{a.data()}, view_type{b.data()});
}

namespace detail {

/**
 * Return the Levenshtein distance between two string views.
 *
 * This is the bottom-up dynamic programming implementation with O(MN) time
 * and space complexity, of course a substantial improvement over the original
 * naive implementation, which has exponential time complexity.
 *
 * Views are used to eliminate copying and are much more efficient that
 * operating on strings as each substring requires another memory allocation.
 * We pass the original views the first call is invoked with to each recursive
 * call and update the `a_i`, `b_i` indices to track the sub-views.
 *
 * @tparam CharT Char type
 * @tparam Traits Char traits type
 *
 * @param orig_a First original string view passed to each recursive call
 * @param orig_b Second original string view passed to each recursive call
 * @param a_i Starting index of first sub-view considered
 * @param b_i Starting index of second sub-view considered
 * @param cache Flat vector holding the results of intemediate invocations. It
 *  must be of size `(orig_a.size() + 1) * (orig_b.size() + 1)`, where the
 *  extra +1 in the sizes is to hold results when any sub-view is empty.
 */
template <typename CharT, typename Traits>
auto dp_levenshtein_distance(
  const std::basic_string_view<CharT, Traits>& orig_a,
  const std::basic_string_view<CharT, Traits>& orig_b,
  std::size_t a_i,
  std::size_t b_i,
  std::vector<std::optional<std::size_t>>& cache)
{
  // form substrings starting from the a_i, b_i start indices. these are more
  // convenient to refer to in each recursive call
  auto a = orig_a.substr(a_i);
  auto b = orig_b.substr(b_i);
  // reference to optional (a_i, b_i) result
  auto& pre = cache.at(a_i * orig_b.size() + b_i);
  // if either a or b sub-views are empty, update cache and return
  if (b.empty()) {
    if (!pre) pre = a.size();
    return *pre;
  }
  if (a.empty()) {
    if (!pre) pre = b.size();
    return *pre;
  }
  // get reference to (a_i + 1, b_i + 1) cache result (also referred to later)
  auto& pre_ab = cache.at((a_i + 1) * orig_b.size() + (b_i + 1));
  // if first chars are equal, compute + update distance for the remainders
  if (a[0] == b[0]) {
    if (!pre_ab)
      pre_ab = dp_levenshtein_distance(orig_a, orig_b, a_i + 1, b_i + 1, cache);
    return *pre_ab;
  }
  // references to the (a_i + 1, b_i) and (a_i, b_i + 1) cache results
  auto& pre_a = cache.at((a_i + 1) * orig_b.size() + b_i);
  auto& pre_b = cache.at(a_i * orig_b.size() + (b_i + 1));
  // now we consider all the cases of computing the distance with or without
  // the first chars of the two strings, updating the cache as necessary
  if (!pre_a)
    pre_a = dp_levenshtein_distance(orig_a, orig_b, a_i + 1, b_i, cache);
  if (!pre_b)
    pre_b = dp_levenshtein_distance(orig_a, orig_b, a_i, b_i + 1, cache);
  if (!pre_ab)
    pre_ab = dp_levenshtein_distance(orig_a, orig_b, a_i + 1, b_i + 1, cache);
  // of course, pick the minimum, and +1 for the necessary insertion
  return 1 + std::min({*pre_a, *pre_b, *pre_ab});
}

}  // namespace detail

/**
 * Return the Levenshtein distance between two string views.
 *
 * This is the bottom-up dynamic programming implementation with O(MN) time
 * and space complexity, of course a substantial improvement over the original
 * naive implementation, which has exponential time complexity.
 *
 * @tparam CharT Char type
 * @tparam Traits Char traits type
 *
 * @param a First string view
 * @param b Second string view
 */
template <typename CharT, typename Traits>
inline auto dp_levenshtein_distance(
  const std::basic_string_view<CharT, Traits>& a,
  const std::basic_string_view<CharT, Traits>& b)
{
  std::vector<std::optional<std::size_t>> cache((a.size() + 1) * (b.size() + 1));
  return detail::dp_levenshtein_distance(a, b, 0, 0, cache);
}

/**
 * Return the Levenshtein distance between two string views.
 *
 * This is the bottom-up dynamic programming implementation with O(MN) time
 * and space complexity, of course a substantial improvement over the original
 * naive implementation, which has exponential time complexity.
 *
 * @tparam CharT Char type
 * @tparam Traits Char traits type
 * @tparam Alloc Allocator
 *
 * @param a First string
 * @param b Second string
 */
template <typename CharT, typename Traits, typename Alloc>
inline auto dp_levenshtein_distance(
  const std::basic_string<CharT, Traits, Alloc>& a,
  const std::basic_string<CharT, Traits, Alloc>& b)
{
  using view_type = std::basic_string_view<CharT, Traits>;
  return dp_levenshtein_distance(view_type{a.data()}, view_type{b.data()});
}

/**
 * Return the Levenshtein distance between two strings.
 *
 * @tparam CharT Char type
 * @tparam Traits Char traits type
 * @tparam Alloc Allocator
 *
 * @param a First string
 * @param b Second string
 * @param flags Execution flags, with `async` and `sync` ignored, to choose the
 *  implementation. By default, the dynamic programming implementation is used.
 */
template <typename CharT, typename Traits, typename Alloc>
inline auto levenshtein_distance(
  const std::basic_string<CharT, Traits, Alloc>& a,
  const std::basic_string<CharT, Traits, Alloc>& b,
  pddcp::execution flags)
{
  if (pddcp::flags_contain(flags, pddcp::execution::recursive))
    return rr_levenshtein_distance(a, b);
  return dp_levenshtein_distance(a, b);
}

/**
 * Return the Levenshtein distance between two strings.
 *
 * This calls the default dynamic programming implementation.
 *
 * @tparam CharT Char type
 * @tparam Traits Char traits type
 * @tparam Alloc Allocator
 *
 * @param a First string
 * @param b Second string
 */
template <typename CharT, typename Traits, typename Alloc>
inline auto levenshtein_distance(
  const std::basic_string<CharT, Traits, Alloc>& a,
  const std::basic_string<CharT, Traits, Alloc>& b)
{
  return levenshtein_distance(a, b, pddcp::execution::dynamic);
}

}  // namespace pddcp

#endif  // PDDCP_STRING_H_
