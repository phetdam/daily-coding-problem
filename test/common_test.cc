/**
 * @file common_test.cc
 * @author Derek Huang
 * @brief common.h unit tests
 * @copyright MIT License
 */

#include "pddcp/common.h"

#include <gtest/gtest.h>

namespace {

/**
 * Test fixture class.
 */
class CommonTest : public ::testing::Test {};

/**
 * Test that `PDDCP_STRINGIFY` and `PDDCP_STRINGIFY_I` work as expected.
 */
TEST_F(CommonTest, StringifyTest)
{
  // expected value for testing PDDCP_STRINGIFY
  static const char* expected = "my_string";
  // expected value for testing PDDCP_STRINGIFY_I on the MY_STRING macro
  static const char* expected_caps = "MY_STRING";
  // test macros. PDDCP_STRINGIFY_I won't expand the macro
#define MY_STRING my_string
  // need to use EXPECT_STREQ as EXPECT_EQ would just compare pointers
  EXPECT_STREQ(expected, PDDCP_STRINGIFY(my_string));
  EXPECT_STREQ(expected, PDDCP_STRINGIFY(MY_STRING));
  EXPECT_STREQ(expected, PDDCP_STRINGIFY_I(my_string));
  EXPECT_STREQ(expected_caps, PDDCP_STRINGIFY_I(MY_STRING));
#undef MY_STRING
}

/**
 * Test that `PDDCP_CONCAT` and `PDDCP_CONCAT_I` work as expected.
 *
 * Note the expected values are bound to names that themselves would result
 * from token pasting in order to avoid needing to use `PDDCP_STRINGIFY`.
 */
TEST_F(CommonTest, ConcatTest)
{
  // expected value for tokens PDDCP_CONCAT is applied to
  static const char* token1token2 = "token1token2";
  // expected value for macro tokens PDDCP_CONCAT_I is applied to
  static const char* TOKEN1TOKEN2 = "TOKEN1TOKEN2";
  // test macros. PDDCP_CONCAT_I won't expand the macros
#define TOKEN1 token1
#define TOKEN2 token2
  // ok to use EXPECT_EQ, we should have identical memory locations
  EXPECT_EQ(token1token2, PDDCP_CONCAT(token1, token2));
  EXPECT_EQ(token1token2, PDDCP_CONCAT(TOKEN1, TOKEN2));
  EXPECT_EQ(token1token2, PDDCP_CONCAT_I(token1, token2));
  EXPECT_EQ(TOKEN1TOKEN2, PDDCP_CONCAT_I(TOKEN1, TOKEN2));
#undef TOKEN1
#undef TOKEN2
}

/**
 * Test that `PDDCP_CPP20_ENABLE` works as expected.
 *
 * There's not really a good way to "fake" compilation levels.
 */
TEST_F(CommonTest, Cpp20Enable)
{
#if PDDCP_HAS_CPP20
  PDDCP_CPP20_ENABLE(SUCCEED());
#else
  PDDCP_CPP20_ENABLE(FAIL());
#endif  // !PDDCP_HAS_CPP20
}

/**
 * Test that `PDDCP_CPP20_CONDITIONAL` works as expected/
 *
 * Again, not really a good way to "fake" compilation levels.
 */
TEST_F(CommonTest, Cpp20Conditional)
{
#if PDDCP_HAS_CPP20
  PDDCP_CPP20_CONDITIONAL(SUCCEED(), FAIL());
#else
  PDDCP_CPP20_CONDITIONAL(FAIL(), SUCCEED());
#endif  // !PDDCP_HAS_CPP20
}

}  // namespace
