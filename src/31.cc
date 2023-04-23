/**
 * @file 31.cc
 * @author Derek Huang
 * @brief #31 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Google.
 *
 * The edit distance between two strings refers to the minimum number of
 * character insertions, deletions, and substitutions required to change one
 * string to the other. For example, the edit distance between "kitten" and
 * "sitting" is three: substitute the "k" for "s", substitute the "e" for "i",
 * and append a "g". Given two strings, compute the edit distance between them.
 */

#include <algorithm>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <gtest/gtest.h>

#include "pddcp/enums.h"
#include "pddcp/utility.h"

namespace {

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
auto levenshtein_distance_r(
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
    return levenshtein_distance_r(tail_a, tail_b);
  // consider all cases of computing the distance with or without the first
  // characters of the two strings, i.e. (tail_a, b), (a, tail_b),
  // (tail_a, tail_b). add one to the result is because we remove one char.
  auto lev_a = levenshtein_distance_r(tail_a, b);
  auto lev_b = levenshtein_distance_r(a, tail_b);
  auto lev_ab = levenshtein_distance_r(tail_a, tail_b);
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
inline auto levenshtein_distance_r(
  const std::basic_string<CharT, Traits, Alloc>& a,
  const std::basic_string<CharT, Traits, Alloc>& b)
{
  using view_type = std::basic_string_view<CharT, Traits>;
  return levenshtein_distance_r(view_type{a.data()}, view_type{b.data()});
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
auto levenshtein_distance_dp(
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
      pre_ab = levenshtein_distance_dp(orig_a, orig_b, a_i + 1, b_i + 1, cache);
    return *pre_ab;
  }
  // references to the (a_i + 1, b_i) and (a_i, b_i + 1) cache results
  auto& pre_a = cache.at((a_i + 1) * orig_b.size() + b_i);
  auto& pre_b = cache.at(a_i * orig_b.size() + (b_i + 1));
  // now we consider all the cases of computing the distance with or without
  // the first chars of the two strings, updating the cache as necessary
  if (!pre_a)
    pre_a = levenshtein_distance_dp(orig_a, orig_b, a_i + 1, b_i, cache);
  if (!pre_b)
    pre_b = levenshtein_distance_dp(orig_a, orig_b, a_i, b_i + 1, cache);
  if (!pre_ab)
    pre_ab = levenshtein_distance_dp(orig_a, orig_b, a_i + 1, b_i + 1, cache);
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
inline auto levenshtein_distance_dp(
  const std::basic_string_view<CharT, Traits>& a,
  const std::basic_string_view<CharT, Traits>& b)
{
  std::vector<std::optional<std::size_t>> cache((a.size() + 1) * (b.size() + 1));
  return detail::levenshtein_distance_dp(a, b, 0, 0, cache);
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
inline auto levenshtein_distance_dp(
  const std::basic_string<CharT, Traits, Alloc>& a,
  const std::basic_string<CharT, Traits, Alloc>& b)
{
  using view_type = std::basic_string_view<CharT, Traits>;
  return levenshtein_distance_dp(view_type{a.data()}, view_type{b.data()});
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
    return levenshtein_distance_r(a, b);
  return levenshtein_distance_dp(a, b);
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

/**
 * Base test class template.
 *
 * @tparam InputType `pddcp::indexed_type<I, T>`, `T` a `basic_string`
 */
template <typename InputType>
class DailyTest31 : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_HELPER_TYPES(InputType);
};

// input types used in the specializations + TYPED_TEST_SUITE macro
using InputType1 = pddcp::indexed_type<0, std::string>;
using InputType2 = pddcp::indexed_type<1, std::wstring>;
using InputType3 = pddcp::indexed_type<2, std::string>;

/**
 * Specialization for the sample input/output pair.
 */
template <>
class DailyTest31<InputType1> : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_HELPER_TYPES(InputType1);
protected:
  static inline const element_type first_{"kitten"};
  static inline const element_type second_{"sitting"};
  static inline const std::size_t distance_ = 3;
};

/**
 * Specialization for the first custom input/output pair.
 */
template <>
class DailyTest31<InputType2> : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_HELPER_TYPES(InputType2);
protected:
  static inline const element_type first_{L"burger"};
  static inline const element_type second_{L"yeehawmania"};
  static inline const std::size_t distance_ = 11;
};

/**
 * Specialization for the second custom input/output pair.
 */
template <>
class DailyTest31<InputType3> : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_HELPER_TYPES(InputType3);
protected:
  static inline const element_type first_{"balloon"};
  static inline const element_type second_{"aloof"};
  static inline const std::size_t distance_ = 3;
};

using DailyTest31Types = ::testing::Types<InputType1, InputType2, InputType3>;
TYPED_TEST_SUITE(DailyTest31, DailyTest31Types);

/**
 * Check that the Levenshtein distance naive recursive implementation works.
 */
TYPED_TEST(DailyTest31, RecursiveTest)
{
  EXPECT_EQ(
    TestFixture::distance_,
    levenshtein_distance(
      TestFixture::first_,
      TestFixture::second_,
      // pddcp::execution::sync is ignored
      pddcp::execution::recursive | pddcp::execution::sync
    )
  );
}

/**
 * Check that the Levenshtein distance dynamic programming implementation works.
 */
TYPED_TEST(DailyTest31, DynamicTest)
{
  EXPECT_EQ(
    TestFixture::distance_,
    levenshtein_distance(
      TestFixture::first_,
      TestFixture::second_,
      // pddcp::execution::async is ignored
      pddcp::execution::dynamic | pddcp::execution::async
    )
  );
}

}  // namespace
