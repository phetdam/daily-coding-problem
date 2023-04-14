/**
 * @file 167.cc
 * @author Derek Huang
 * @brief #167 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Airbnb.
 *
 * Given a list of words, find all pairs of unique indices such that the
 * concatenation of the two words is a palindrome. For example, given the list
 * ["code", "edoc", "da", "d"], return [(0, 1), (1, 0), (2, 3)].
 */

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "pddcp/string.h"
#include "pddcp/utility.h"

namespace {

/**
 * Convenience type alias for string vectors.
 *
 * @tparam CharT Char type
 * @tparam Traits Char traits, defaults to `std::char_traits<CharT>`
 * @tparam Alloc Allocator, defaults to `std::allocator<CharT>`
 */
template <
  typename CharT,
  typename Traits = std::char_traits<CharT>,
  typename Alloc = std::allocator<CharT>
>
using string_vector = std::vector<std::basic_string<CharT, Traits, Alloc>>;

/**
 * Return index pairs of the concatenated string pairs that form palindromes.
 *
 * @tparam CharT Char type
 * @tparam Traits Char traits
 * @tparam Alloc Allocator
 *
 * @param value Strings to check for string pairs that form palindromes
 */
template <typename CharT, typename Traits, typename Alloc>
auto palindrome_indices(const string_vector<CharT, Traits, Alloc>& values)
{
  using size_type = typename std::decay_t<decltype(values)>::size_type;
  // vector of the palindrome indices
  std::vector<std::pair<size_type, size_type>> indices;
  if (values.empty())
    return indices;
  // double for loop. note we need to ensure i != j
  for (size_type i = 0; i < values.size(); i++)
    for (size_type j = 0; j < values.size(); j++)
      if (i != j && pddcp::is_palindrome(values[i] + values[j]))
        indices.emplace_back(i, j);
  return indices;
}

/**
 * Helper macro for `DailyTest167` type aliases.
 *
 * @param type `pddcp::indexed_type<I, T>` to get type members for
 */
#define DAILY_TEST_167_HELPER_TYPES(type) \
  using indexed_type = type; \
  using element_type = typename indexed_type::element_type; \
  using size_type = typename element_type::size_type; \
  using index_vector_type = std::vector<std::pair<size_type, size_type>>

/**
 * Base test class template.
 *
 * @tparam IndexedType `pddcp::indexed_type<I, T>` with a
 *  `string_vector<C, T, A>` as the `element_type`
 */
template <typename IndexedType>
class DailyTest167 : public ::testing::Test {
public:
  DAILY_TEST_167_HELPER_TYPES(IndexedType);
};

// input types used in the test fixture specializations. index types allows for
// unique wrapper types that have the same element_type member.
using InputType1 = pddcp::indexed_type<0, string_vector<char>>;
using InputType2 = pddcp::indexed_type<1, string_vector<char>>;

/**
 * Specialization for the sample input/output pair.
 */
template <>
class DailyTest167<InputType1> : public ::testing::Test {
public:
  DAILY_TEST_167_HELPER_TYPES(InputType1);
protected:
  static inline const element_type input_{"code", "edoc", "da", "d"};
  static inline const index_vector_type output_{{0, 1}, {1, 0}, {2, 3}};
};

/**
 * Specialization for a custom sample input/output pair.
 *
 * This input is designed to test the handling of cases where permuting the
 * concatenation either forms the same palindrome or does not form a palindrome.
 *
 * For example, "aaa" + "baaa" is a palindrome, but "baaa" + "aaa" is not. Same
 * with "ch" + "ahc" compared to "ahc" + "ch".
 */
template <>
class DailyTest167<InputType2> : public ::testing::Test {
public:
  DAILY_TEST_167_HELPER_TYPES(InputType2);
protected:
  static inline const element_type input_{"aaa", "aaa", "baaa", "ch", "ahc"};
  static inline const index_vector_type output_{
    {0, 1}, {0, 2}, {1, 0}, {1, 2}, {3, 4}
  };
};

using DailyTest167Types = ::testing::Types<InputType1, InputType2>;
TYPED_TEST_SUITE(DailyTest167, DailyTest167Types);

/**
 * Test that `palindrome_indices` works as expected.
 */
TYPED_TEST(DailyTest167, TypedTest)
{
  EXPECT_EQ(TestFixture::output_, palindrome_indices(TestFixture::input_));
}

}  // namespace
