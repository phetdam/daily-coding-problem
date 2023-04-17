/**
 * @file 80.cc
 * @author Derek Huang
 * @brief #80 from Daily Coding Problem
 * @copyright MIT License
 *
 * Good morning! Here's your coding interview problem for today.
 *
 * This problem was asked by Google.
 *
 * Given the root of a binary tree, return a deepest node. For example, in the
 * following tree, return d.
 *
 *     a
 *    / \
 *   b   c
 *  /
 * d
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "pddcp/binary_tree.h"
#include "pddcp/utility.h"

namespace {

// input types used in the specializations + TYPED_TEST_SUITE macro
using InputType1 = pddcp::indexed_type<0, pddcp::binary_tree<char>>;
using InputType2 = pddcp::indexed_type<1, pddcp::binary_tree<unsigned int>>;
using InputType3 = pddcp::indexed_type<2, pddcp::binary_tree<int>>;

/**
 * Base test class template.
 *
 * Create specializations to use `TYPED_TEST` like a parametrized test.
 *
 * @tparam IndexedType `pddcp::indexed_type<I, T>` for `pddcp::binary_tree<T>`
 */
template <typename InputType>
class DailyTest80 : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_HELPER_TYPES(InputType);
};

/**
 * Specialization for the sample input/output pair.
 */
template <>
class DailyTest80<InputType1> : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_HELPER_TYPES(InputType1);
protected:
  static inline const element_type input_{
    'a',
    new element_type{'b', new element_type{'d'}, nullptr},
    new element_type{'c'}
  };
  static inline constexpr typename element_type::value_type output_ = 'd';
};

/**
 * Specialization for first custom input/output pair.
 *
 * Input:
 *
 *       7
 *     /   \
 *    4     9
 *   / \   / \
 *  3   6 8   11
 *     /       \
 *    5         13
 */
template <>
class DailyTest80<InputType2> : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_HELPER_TYPES(InputType2);
protected:
  static inline const element_type input_{
    7,
    new element_type{
      4,
      new element_type{3},
      new element_type{6, new element_type{5}, nullptr}
    },
    new element_type{
      9,
      new element_type{8},
      new element_type{11, nullptr, new element_type{13}}
    }
  };
  static inline constexpr typename element_type::value_type output_ = 5;
};

/**
 * Specialization for second custom input/output pair.
 *
 * Input:
 *
 *     -3
 *     / \
 *    4   5
 *   /   / \
 * -9   1   3
 *         /
 *        8
 */
template <>
class DailyTest80<InputType3> : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_HELPER_TYPES(InputType3);
protected:
  static inline const element_type input_{
    -3,
    new element_type{4, new element_type{-9}, nullptr},
    new element_type{
      5,
      new element_type{1},
      new element_type{3, new element_type{8}, nullptr}
    }
  };
  static inline constexpr typename element_type::value_type output_ = 8;
};

using DailyTest80Types = ::testing::Types<InputType1, InputType2, InputType3>;
TYPED_TEST_SUITE(DailyTest80, DailyTest80Types);

/**
 * Test that `deepest_node` works as expected.
 */
TYPED_TEST(DailyTest80, TypedTest)
{
  // guard against nullptr being returned
  auto deepest = pddcp::deepest_node(TestFixture::input_);
  ASSERT_THAT(deepest, ::testing::NotNull());
  // then we can compare the values
  EXPECT_EQ(TestFixture::output_, deepest->value());
}

}  // namespace
