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

#include <cstdint>
#include <string>

#include <gtest/gtest.h>

#include "pddcp/enums.h"
#include "pddcp/string.h"
#include "pddcp/utility.h"

namespace {

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
    pddcp::levenshtein_distance(
      TestFixture::first_, TestFixture::second_, pddcp::execution::recursive
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
    pddcp::levenshtein_distance(
      TestFixture::first_, TestFixture::second_, pddcp::execution::dynamic
    )
  );
}

}  // namespace
