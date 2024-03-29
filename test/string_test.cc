/**
 * @file string_test.cc
 * @author Derek Huang
 * @brief string.h unit tests
 * @copyright MIT License
 */

#include "pddcp/string.h"

#include <deque>
#include <forward_list>
#include <list>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "pddcp/common.h"
#include "pddcp/enums.h"
#include "pddcp/type_traits.h"
#include "pddcp/utility.h"

namespace {

/**
 * Base test class template for `pddcp::string_join` testing.
 *
 * @tparam IndexedType `pddcp::indexed_type<I, T>` with `T` a *Container*.
 */
template <typename IndexedType>
class StringJoinTest : public ::testing::Test {};

/**
 * Helper macro to define `StringJoinTest` specializations.
 *
 * @param input_type `pddcp::indexed_type<I, T>` specialization
 * @param input_init `PDDCP_INIT_LIST(...)` of input values
 * @param expected String literal indicating the expected result
 */
#define PDDCP_STRING_JOIN_TEST(input_type, input_init, expected) \
  template <> \
  class StringJoinTest<input_type> : public ::testing::Test { \
  private: \
    using char_type = pddcp::literal_char_t<decltype(expected)>; \
  public: \
    PDDCP_INDEXED_TYPE_HELPER_TYPES(input_type); \
  protected: \
    static inline const element_type input_ input_init; \
    static inline const std::basic_string<char_type> expected_{expected}; \
    static inline const std::basic_string<char_type> delim_{}; \
  }

/**
 * Helper macro to define `StringJoinTest` specializations with delimiter.
 *
 * @param input_type `pddcp::indexed_type<I, T>` specialization
 * @param input_init `PDDCP_INIT_LIST(...)` of input values
 * @param expected String literal indicating the expected result
 * @param delim String literal delimiter, must be same type as `expected`
 */
#define PDDCP_STRING_JOIN_TEST_EX(input_type, input_init, expected, delim) \
  template <> \
  class StringJoinTest<input_type> : public ::testing::Test { \
  private: \
    using char_type = pddcp::literal_char_t<decltype(expected)>; \
    static_assert( \
      std::is_same_v<char_type, pddcp::literal_char_t<decltype(delim)>>, \
      "expected and delim must have the same character type" \
    ); \
  public: \
    PDDCP_INDEXED_TYPE_HELPER_TYPES(input_type); \
  protected: \
    static inline const element_type input_ input_init; \
    static inline const std::basic_string<char_type> expected_{expected}; \
    static inline const std::basic_string<char_type> delim_{delim}; \
  }

// input types used for StringJoinTest
using StringJoinType1 = pddcp::indexed_type<0, std::vector<unsigned int>>;
using StringJoinType2 = pddcp::indexed_type<1, std::vector<std::string>>;
using StringJoinType3 = pddcp::indexed_type<2, std::deque<double>>;
using StringJoinType4 = pddcp::indexed_type<3, std::vector<unsigned int>>;
using StringJoinType5 = pddcp::indexed_type<4, std::list<char>>;
using StringJoinType6 = pddcp::indexed_type<5, std::forward_list<int>>;

// specializations for StringJoinTest
// TODO: add test cases for different string literal types (doesn't work now)
PDDCP_STRING_JOIN_TEST(
  StringJoinType1,
  PDDCP_INIT_LIST(1, 4, 3, 5, 6, 6, 7, 1, 8, 9, 11, 134, 1),
  "1435667189111341"
);
PDDCP_STRING_JOIN_TEST(
  StringJoinType2,
  PDDCP_INIT_LIST("these", "strings", "are", "concatenated"),
  "thesestringsareconcatenated"
);
PDDCP_STRING_JOIN_TEST(
  StringJoinType3,
  PDDCP_INIT_LIST(1.3, 2.45, 9.1, 10.895, 15.67),
  "1.32.459.110.89515.67"
);
PDDCP_STRING_JOIN_TEST_EX(
  StringJoinType4,
  PDDCP_INIT_LIST(1, 4, 15, 67, 88),
  "1 wow 4 wow 15 wow 67 wow 88",
  " wow "
);
PDDCP_STRING_JOIN_TEST_EX(
  StringJoinType5,
  PDDCP_INIT_LIST('q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p'),
  "q w e r t y u i o p",
  " "
);
PDDCP_STRING_JOIN_TEST_EX(
  StringJoinType6,
  PDDCP_INIT_LIST(1, 5, 1, 2, 3, 1, 4, 12, 15, 61, 9, 100),
  "1, 5, 1, 2, 3, 1, 4, 12, 15, 61, 9, 100",
  ", "
);

using StringJoinTestTypes = ::testing::Types<
  StringJoinType1,
  StringJoinType2,
  StringJoinType3,
  StringJoinType4,
  StringJoinType5,
  StringJoinType6
>;
TYPED_TEST_SUITE(StringJoinTest, StringJoinTestTypes);

/**
 * Check that `pddcp::string_join` works as expected.
 */
TYPED_TEST(StringJoinTest, TypedTest)
{
  EXPECT_EQ(
    TestFixture::expected_,
    pddcp::string_join(TestFixture::input_, TestFixture::delim_)
  );
}

/**
 * Base test class template for `pddcp::is_palindrome` testing.
 *
 * @tparam IndexedType `pddcp::indexed_type<I, T>` with `T` an STL string type
 */
template <typename IndexedType>
class StringIsPalindromeTest : public ::testing::Test {};

/**
 * Helper macro to define `StringIsPalindromeTest` specializations.
 *
 * @param input_type `pddcp::indexed_type<I, T>` specialization
 * @param input String literal input
 * @param expected Boolean expected result
 */
#define PDDCP_STRING_IS_PALIDROME_TEST(input_type, input, expected) \
  template <> \
  class StringIsPalindromeTest<input_type> : public ::testing::Test { \
  public: \
    PDDCP_INDEXED_TYPE_HELPER_TYPES(input_type); \
  protected: \
    static inline const element_type input_{input}; \
    static inline constexpr bool expected_ = expected; \
  }

// input types used for StringIsPalindromeTest
using StringIsPalindromeType1 = pddcp::indexed_type<0, std::string>;
using StringIsPalindromeType2 = pddcp::indexed_type<1, std::wstring>;
using StringIsPalindromeType3 = pddcp::indexed_type<2, std::string>;
using StringIsPalindromeType4 = pddcp::indexed_type<3, std::u16string>;
using StringIsPalindromeType5 = pddcp::indexed_type<4, std::u32string>;
using StringIsPalindromeType6 = pddcp::indexed_type<5, std::wstring>;

// specializations for StringIsPalindromeTest
PDDCP_STRING_IS_PALIDROME_TEST(StringIsPalindromeType1, "totally", false);
PDDCP_STRING_IS_PALIDROME_TEST(StringIsPalindromeType2, L"abba", true);
PDDCP_STRING_IS_PALIDROME_TEST(StringIsPalindromeType3, "whatever", false);
PDDCP_STRING_IS_PALIDROME_TEST(StringIsPalindromeType4, u"aeiouoiea", true);
PDDCP_STRING_IS_PALIDROME_TEST(StringIsPalindromeType5, U"asdffdsa", true);
PDDCP_STRING_IS_PALIDROME_TEST(StringIsPalindromeType6, L"burger", false);

using StringIsPalindromeTestTypes = ::testing::Types<
  StringIsPalindromeType1,
  StringIsPalindromeType2,
  StringIsPalindromeType3,
  StringIsPalindromeType4,
  StringIsPalindromeType5,
  StringIsPalindromeType6
>;
TYPED_TEST_SUITE(StringIsPalindromeTest, StringIsPalindromeTestTypes);

/**
 * Check that `pddcp::is_palindrome` works as expected.
 */
TYPED_TEST(StringIsPalindromeTest, TypedTest)
{
  EXPECT_EQ(TestFixture::expected_, pddcp::is_palindrome(TestFixture::input_));
}

/**
 * Base test class for `pddcp::levenshtein_distance` testing.
 *
 * @tparam IndexedType `pddcp::indexed_type<I, T>` with `T` an STL string type
 */
template <typename IndexedType>
class StringLevenDistTest : public ::testing::Test {};

/**
 * Helper macro to define `StringLevenDistTest` specializations.
 *
 * @param input_type `pddcp::indexed_type<I, T>` specialization
 * @param first First string literal
 * @param second Second string literal
 * @param expected Expected Levenshtein distance
 * @param flags `pddcp::execution` flags, either `recursive` or `dynamic`
 */
#define PDDCP_STRING_LEVEN_DIST_TEST(input_type, first, second, expected, flags) \
  template <> \
  class StringLevenDistTest<input_type> : public ::testing::Test { \
    static_assert( \
      flags == pddcp::execution::recursive || \
      flags == pddcp::execution::dynamic, \
      "flags must be either pddcp::execution members recursive or dynamic" \
    ); \
  public: \
    PDDCP_INDEXED_TYPE_HELPER_TYPES(input_type); \
    using result_type = decltype( \
      pddcp::levenshtein_distance( \
        std::declval<element_type>(), std::declval<element_type>() \
      ) \
    ); \
  protected: \
    static inline const element_type first_{first}; \
    static inline const element_type second_{second}; \
    static inline constexpr result_type expected_ = expected; \
    static inline constexpr pddcp::execution flags_ = flags; \
  }

// input types used for StringLevenDistTest
using StringLevenDistType1 = pddcp::indexed_type<0, std::string>;
using StringLevenDistType2 = pddcp::indexed_type<1, std::string>;
using StringLevenDistType3 = pddcp::indexed_type<2, std::wstring>;
using StringLevenDistType4 = pddcp::indexed_type<3, std::basic_string<char16_t>>;
using StringLevenDistType5 = pddcp::indexed_type<4, std::basic_string<char32_t>>;

// specializations for StringLevenDistTest
PDDCP_STRING_LEVEN_DIST_TEST(
  StringLevenDistType1, "cheese", "chabse", 2, pddcp::execution::recursive
);
PDDCP_STRING_LEVEN_DIST_TEST(
  StringLevenDistType2, "burger", "borgar", 2, pddcp::execution::dynamic
);
PDDCP_STRING_LEVEN_DIST_TEST(
  StringLevenDistType3, L"hello", L"halp", 3, pddcp::execution::dynamic
);
PDDCP_STRING_LEVEN_DIST_TEST(
  StringLevenDistType4, u"fiduciary", u"feud city", 5, pddcp::execution::dynamic
);
PDDCP_STRING_LEVEN_DIST_TEST(
  StringLevenDistType5, U"lifetime", U"lifeline", 2, pddcp::execution::dynamic
);

using StringLevenDistTestTypes = ::testing::Types<
  StringLevenDistType1,
  StringLevenDistType2,
  StringLevenDistType3,
  StringLevenDistType4,
  StringLevenDistType5
>;
TYPED_TEST_SUITE(StringLevenDistTest, StringLevenDistTestTypes);

/**
 * Check that `pddcp::levenshtein_distance` works as expected.
 */
TYPED_TEST(StringLevenDistTest, TypedTest)
{
  EXPECT_EQ(
    TestFixture::expected_,
    pddcp::levenshtein_distance(
      TestFixture::first_, TestFixture::second_, TestFixture::flags_
    )
  );
}

}  // namespace
