/**
 * @file 49.cc
 * @author Derek Huang
 * @brief #49 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Amazon.
 *
 * Given an array of numbers, find the maximum sum of any contiguous subarray
 * of the array. For example, given the array [34, -50, 42, 14, -5, 86], the
 * maximum sum would be 137, since we would take elements 42, 14, -5, and 86.
 * Given the array [-5, -1, -8, -9], the maximum sum would be 0, since we would
 * not take any elements. Do this in O(N) time.
 */

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <type_traits>
#include <vector>

#include <gtest/gtest.h>

#include "pddcp/utility.h"

namespace {

/**
 * Return the maximum sum of any slice of the input container values.
 *
 * If there is no positive sum that is possible, zero is returned. This routine
 * is O(N), and works by tracking the max running sum, restarting if any
 * negative value causes the running sum to fall below zero.
 *
 * @tparam Container *Container* with integral `value_type`
 *
 * @param values Values to return max sum of any contiguous container slice
 */
template <typename Container>
auto max_contiguous_sum(const Container& values)
{
  // value type alias + check we can sum this type
  using value_type = typename Container::value_type;
  static_assert(
    std::is_integral_v<value_type>, "Container value_type must be integral"
  );
  // running sum + maximum sum
  value_type running_sum{};
  value_type max_sum{};
  // loop through container values. if all values are nonnegative, then we
  // would just sum all of them. it is worth keeping negative values in the
  // running sum, i.e. not restarting the running sum, as long as their
  // addition does not make the running sum negative. we restart, i.e. set
  // running sum to zero, if a negative value brings us below zero.
  for (const auto& value : values) {
    // running sum is restarted if goes below zero + update max sum
    running_sum = std::max(value_type{}, running_sum + value);
    max_sum = std::max(running_sum, max_sum);
  }
  return max_sum;
}

// input types used in the specializations + TYPED_TEST_SUITE macro. note that
// there were some issues using std::vector<short> or std::vector<std::int16_t>
// (likely type alias for short anyways) involving std::max overloads.
using InputType1 = pddcp::indexed_type<0, std::vector<int>>;
using InputType2 = pddcp::indexed_type<1, std::vector<std::int64_t>>;
using InputType3 = pddcp::indexed_type<2, std::vector<std::uint32_t>>;
using InputType4 = pddcp::indexed_type<3, std::vector<long>>;
using InputType5 = pddcp::indexed_type<4, std::vector<std::int32_t>>;

/**
 * Base test class template.
 *
 * Create specializations to use `TYPED_TEST` like a parametrized test.
 *
 * @tparam IndexedType `pddcp::indexed_type<I, T>`, *Container* `element_type`
 */
template <typename InputType>
class DailyTest49 : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_CONTAINER_HELPER_TYPES(InputType);
};

/**
 * Specialization for the first sample input/output pair.
 */
template <>
class DailyTest49<InputType1> : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_CONTAINER_HELPER_TYPES(InputType1);
protected:
  static inline const element_type input_{34, -50, 42, 14, -5, 86};
  static inline constexpr value_type output_ = 137;
};

/**
 * Specialization for the second sample input/output pair.
 */
template <>
class DailyTest49<InputType2> : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_CONTAINER_HELPER_TYPES(InputType2);
protected:
  static inline const element_type input_{-5, -1, -8, -9};
  static inline constexpr value_type output_ = 0;
};

/**
 * Specialization for the first custom input/output pair.
 *
 * Shows that when all values are nonnegative, the entire container is summed.
 */
template <>
class DailyTest49<InputType3> : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_CONTAINER_HELPER_TYPES(InputType3);
protected:
  static inline const element_type input_{1, 5, 10, 13, 7, 17, 9};
  static inline const value_type output_{
    std::accumulate(input_.begin(), input_.end(), value_type{})
  };
};

/**
 * Specialization for the second custom input/output pair.
 *
 * Here the running sum is never reset during execution.
 */
template <>
class DailyTest49<InputType4> : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_CONTAINER_HELPER_TYPES(InputType4);
protected:
  static inline const element_type input_{200, 300, -499, 2, 9};
  static inline constexpr value_type output_ = 500;
};

/**
 * Speclization for the third custom input/output pair.
 *
 * Here the running sum is never reset and the large negative value included in
 * the actual maximum sum of the container values, which is all the values.
 */
template <>
class DailyTest49<InputType5> : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_CONTAINER_HELPER_TYPES(InputType5);
protected:
  static inline const element_type input_{200, 300, -499, 900};
  static inline constexpr value_type output_ = 901;
};

using DailyTest49Types = ::testing::Types<
  InputType1, InputType2, InputType3, InputType4, InputType5
>;
TYPED_TEST_SUITE(DailyTest49, DailyTest49Types);

/**
 * Test that `max_contiguous_sum` works as expected.
 */
TYPED_TEST(DailyTest49, TypedTest)
{
  EXPECT_EQ(TestFixture::output_, max_contiguous_sum(TestFixture::input_));
}

}  // namespace
