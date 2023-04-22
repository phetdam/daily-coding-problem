/**
 * @file enums_test.cc
 * @author Derek Huang
 * @brief enums.h unit tests
 * @copyright MIT License
 */

#include "pddcp/enums.h"

#include <type_traits>

#include <gtest/gtest.h>

#include "pddcp/type_traits.h"

namespace {

/**
 * Base test fixture class template for testing.
 *
 * @tparam EnumType Enum [class] type
 */
template <typename EnumType>
class EnumsTest : public ::testing::Test {
public:
  PDDCP_ENUM_CLASS_HELPER_TYPES(EnumType);
};

/**
 * Specialization for the `pddcp::execution` enum.
 */
template <>
class EnumsTest<pddcp::execution> : public ::testing::Test {
public:
  PDDCP_ENUM_CLASS_HELPER_TYPES(pddcp::execution);
protected:
  // target flag to test for, flags that contain and don't contain the target
  static inline constexpr enum_type target_flag_ = pddcp::execution::async;
  static inline constexpr enum_type true_flags_{
    target_flag_ | pddcp::execution::recursive
  };
  static inline constexpr enum_type false_flags_{
    pddcp::execution::dynamic | pddcp::execution::sync
  };
};

enum test_enum {
  test_enum_first,
  test_enum_second,
  test_enum_count
};

using EnumsTestTypes = ::testing::Types<pddcp::execution, test_enum>;
TYPED_TEST_SUITE(EnumsTest, EnumsTestTypes);

/**
 * Test that `flags_contain` works as expected for a bitmask enum.
 *
 * `target_flag_` is the enum value whose membership we want to test for.
 * `true_flags_` are a bitwise combination that contain `target_flag_` while
 * `false_flags_` are a bitwise combination that do not contain `target_flag_`.
 */
TYPED_TEST(EnumsTest, FlagMaskTest)
{
  if constexpr (pddcp::is_bitmask_enum_v<TypeParam>) {
    EXPECT_TRUE(
      pddcp::flags_contain(TestFixture::true_flags_, TestFixture::target_flag_)
    );
    EXPECT_FALSE(
      pddcp::flags_contain(TestFixture::false_flags_, TestFixture::target_flag_)
    );
  }
  else
    GTEST_SKIP();
}

/**
 * Test that a bitmask enum XORs correctly.
 *
 * Note that `false_flags_` does not contain `target_flag_`.
 */
TYPED_TEST(EnumsTest, XorTest)
{
  if constexpr (pddcp::is_bitmask_enum_v<TypeParam>) {
    EXPECT_EQ(
      static_cast<TypeParam>(0),
      TestFixture::target_flag_ ^ TestFixture::target_flag_
    );
    EXPECT_EQ(
      TestFixture::false_flags_ | TestFixture::target_flag_,
      TestFixture::false_flags_ ^ TestFixture::target_flag_
    );
  }
  else
    GTEST_SKIP();
}

}  // namespace
