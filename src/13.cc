/**
 * @file 13.cc
 * @author Derek Huang
 * @brief #13 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Amazon.
 *
 * Given an integer k and a string s, find the length of the longest substring
 * that contains at most k distinct characters. For example, given s = "abcba"
 * and k = 2, the longest substring with k distinct characters is "bcb".
 */

#include <algorithm>
#include <cstdint>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>

#include <gtest/gtest.h>

#include "pddcp/utility.h"

namespace {

template <typename CharT, typename Traits>
auto longest_k_distinct_substring(
  const std::basic_string_view<CharT, Traits>& str, std::size_t max_distinct)
{
  using size_type = typename std::decay_t<decltype(str)>::size_type;
  // only possible answer if max_distinct is zero
  if (!max_distinct)
    return size_type{};
  // if max_distinct >= string size, then string itself is valid
  if (max_distinct > str.size())
    return str.size();
  // starting and ending indices of the substring, actual string size
  size_type s_index = 0;
  size_type e_index = 0;
  const auto str_size = str.size();
  // max substring length, map holding unique character counts
  size_type max_len = 0;
  std::unordered_map<CharT, size_type> char_counts;
  // until we have exhausted the entire string
  while (e_index < str_size) {
    // get current e_index char count char occurrence
    auto cur_char = str[e_index];
    char_counts[cur_char]++;
    // if too many unique characters, we decrement the char count for the
    // s_index character and then increment s_index
    if (char_counts.size() > max_distinct) {
      char_counts[str[s_index]]--;
      // if the count for char at s_index is zero, need to delete key entry
      if (!char_counts[str[s_index]])
        char_counts.erase(str[s_index]);
      // contract substring
      s_index++;
    }
    // else ok to update max_len
    else
      max_len = std::max(max_len, e_index - s_index + 1);
    // continue expanding substring
    e_index++;
  }
  return max_len;
}

template <typename CharT, typename Traits, typename Alloc>
inline auto longest_k_distinct_substring(
  const std::basic_string<CharT, Traits, Alloc>& str, std::size_t max_distinct)
{
  return longest_k_distinct_substring(
    std::basic_string_view<CharT, Traits>{str.data()}, max_distinct
  );
}

template <typename IndexedType>
class DailyTest13 : public ::testing::Test {};

using InputType1 = pddcp::indexed_type<0, std::string>;
using InputType2 = pddcp::indexed_type<1, std::wstring>;
using InputType3 = pddcp::indexed_type<2, std::basic_string<char16_t>>;

#define DAILY_TEST_13(input_type, input, max_distinct, res) \
  template <> \
  class DailyTest13<input_type> : public ::testing::Test { \
  public: \
    PDDCP_INDEXED_TYPE_HELPER_TYPES(input_type); \
  protected: \
    static inline const element_type input_{input}; \
    static inline constexpr std::size_t max_distinct_ = max_distinct; \
    static inline constexpr std::size_t res_ = res; \
  }

DAILY_TEST_13(InputType1, "abcba", 2, 3);
DAILY_TEST_13(InputType2, L"abcdaklqw", 4, 5);
DAILY_TEST_13(InputType3, u"aninterestingstring", 5, 7);

using DailyTest13Types = ::testing::Types<InputType1, InputType2, InputType3>;
TYPED_TEST_SUITE(DailyTest13, DailyTest13Types);

TYPED_TEST(DailyTest13, TypedTest)
{
  EXPECT_EQ(
    TestFixture::res_,
    longest_k_distinct_substring(TestFixture::input_, TestFixture::max_distinct_)
  );
}

}  // namespace
