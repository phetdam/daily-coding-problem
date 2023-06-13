/**
 * @file type_traits_test.h
 * @author Derek Huang
 * @brief type_traits.h type-parametrized test definition header
 * @copyright MIT License
 */

#ifndef PDDCP_TESTING_TYPE_TRAITS_TEST_H_
#define PDDCP_TESTING_TYPE_TRAITS_TEST_H_

#include <gtest/gtest.h>

#include "pddcp/testing/common.h"

PDDCP_TESTING_NAMESPACE_BEGIN

/**
 * Test fixture class template for testing type traits classes.
 *
 * @tparam InputType `std::pair<Traits, TruthType>` where `Traits` is the type
 *   resulting from applying a traits template to a type and `TruthType` is
 *  `std::true_type` or `std::false_type`. See examples for helper type
 *  template definitions that provide simpler use than manual repetition.
 *
 * @note The `Traits` type is expected to be a `std::bool_constant<B>` subclass
 *  or at least contain the static constexpr boolean `value` member.
 */
template <typename InputType>
class TypeTraitsTest : public ::testing::Test {};

// we use type-parametrized testing since we exceeded the 50-type limit for the
// types that ::testing::Types<...> can support for typed tests
TYPED_TEST_SUITE_P(TypeTraitsTest);

/**
 * Test that a traits class works as expected.
 */
TYPED_TEST_P(TypeTraitsTest, TraitsTest)
{
  EXPECT_EQ(TestFixture::expected_, TestFixture::actual_);
}

// unlike typed tests, need to individually register each test
REGISTER_TYPED_TEST_SUITE_P(TypeTraitsTest, TraitsTest);

/**
 * Macro for simplifying `TypeTraitsTest` specializations.
 *
 * @param traits_input `InputType` as described above
 */
#define PDDCP_TYPE_TRAITS_TEST_CLASS(traits_input) \
  template <> \
  class PDDCP_TESTING_NAMESPACE::TypeTraitsTest<traits_input> \
    : public ::testing::Test { \
  public: \
    using traits_type = typename traits_input::first_type; \
    using truth_type = typename traits_input::second_type; \
  protected: \
    static inline constexpr bool expected_ = truth_type::value; \
    static inline constexpr bool actual_ = traits_type::value; \
  }

PDDCP_TESTING_NAMESPACE_END

#endif  // PDDCP_TESTING_TYPE_TRAITS_TEST_H_
