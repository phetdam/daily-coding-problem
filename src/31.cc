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
#include <string>
#include <string_view>

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
  // if first chars are equal, compute distance for the remainders (tails)
  if (a[0] == b[0])
    return levenshtein_distance_r(a.substr(1), b.substr(1));
  // tails for first and second string
  // auto tail_a = a.substr(1);
  // auto tail_b = b.substr(1);
  decltype(a) tail_a{a.data() + 1};
  decltype(b) tail_b{b.data() + 1};
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
  return levenshtein_distance_r(
    std::basic_string_view<CharT, Traits>{a.data()},
    std::basic_string_view<CharT, Traits>{b.data()}
  );
}

/**
 * Return the Levenshtein distance between two strings.
 *
 * The implementation can be chosen by using the execution flags.
 *
 * @todo Currently, only the recursive implementation is available.
 *
 * @tparam CharT Char type
 * @tparam Traits Char traits type
 * @tparam Alloc Allocator
 *
 * @param a First string
 * @param b Second string
 * @param flags Execution flags, with `async` and `sync` ignored.
 */
template <typename CharT, typename Traits, typename Alloc>
inline auto levenshtein_distance(
  const std::basic_string<CharT, Traits, Alloc>& a,
  const std::basic_string<CharT, Traits, Alloc>& b,
  pddcp::execution flags)
{
  // unused for now
  (void) flags;
  // TODO: add DP implementation, which would be the default
  return levenshtein_distance_r(a, b);
}

/**
 * Return the Levenshtein distance between two strings.
 *
 * @todo Update default flags when DP solution is available.
 *
 * @tparam CharT Char type
 * @tparam Traits Char traits type
 * @tparam Alloc Allocator
 *
 * @param a First string
 * @param b Second string
 * @param flags Execution flags, with `async` and `sync` ignored.
 */
template <typename CharT, typename Traits, typename Alloc>
inline auto levenshtein_distance(
  const std::basic_string<CharT, Traits, Alloc>& a,
  const std::basic_string<CharT, Traits, Alloc>& b)
{
  return levenshtein_distance(a, b, pddcp::execution::recursive);
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
 * Check that the naive recursive Levenshtein distance implementation works.
 */
TYPED_TEST(DailyTest31, RecursiveTest)
{
  EXPECT_EQ(
    TestFixture::distance_,
    levenshtein_distance(
      TestFixture::first_,
      TestFixture::second_,
      pddcp::execution::recursive | pddcp::execution::sync
    )
  );
}

}  // namespace
