/**
 * @file 102.cc
 * @author Derek Huang
 * @brief #102 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Lyft.
 *
 * Given a list of integers and a number K, return which contiguous elements of
 * the list sum to K. For example, if the list is [1, 2, 3, 4, 5] and K is 9,
 * then it should return [2, 3, 4], since 2 + 3 + 4 = 9.
 */

#include <deque>
#include <type_traits>
#include <vector>

#include <gtest/gtest.h>

#include "pddcp/utility.h"

namespace {

/**
 * Return a contiguous slice of values that sum up to the given target.
 *
 * The input container must only contain nonnegative values for correct output.
 *
 * @tparam OutContainer *Container* used for the return type
 * @tparam T target sum type
 * @tparam InContainer *Container* with `value_type` convertible to `T`
 */
template <typename OutContainer, typename T, typename InContainer>
auto contiguous_sum(T target_sum, const InContainer& values)
{
  // check that input container type is summable and convertible to T
  using value_type = typename InContainer::value_type;
  static_assert(
    std::is_integral_v<value_type>, "InContainer type must be integral"
  );
  static_assert(
    std::is_convertible_v<value_type, T>,
    "InContainer type must be convertible to target_sum type"
  );
  // queue to store summation elements in + running summation as we iterate
  std::deque<value_type> queue;
  T running_sum{};
  // O(N) loop of pushing elements until we exceed the sum and popping until we
  // either meet the sum or run out of elements in the queue (failure)
  for (const auto& value : values) {
    // for each new value, check if are over sum and need to pop elements
    // first. this allows us to use the single range-for loop
    while (queue.size() && running_sum > target_sum) {
      running_sum -= queue.front();
      queue.pop_front();
    }
    // if we have perfect summation, copy values into OutContainer and return
    if (running_sum == target_sum)
      return OutContainer{queue.begin(), queue.end()};
    // otherwise, sum below target, so push into queue
    running_sum += value;
    queue.push_back(value);
  }
  // one final check in case last value meets target sum
  if (running_sum == target_sum)
    return OutContainer{queue.begin(), queue.end()};
  // failed to meet summation, so return empty OutContainer
  return OutContainer{};
}

/**
 * Return a contiguous vector of values that sum up to the given target.
 *
 * The input container must only contain nonnegative values for correct output.
 *
 * @tparam T target sum type
 * @tparam Container *Container* with `value_type` convertible to `T`
 */
template <typename T, typename Container>
inline auto contiguous_sum(T target_sum, const Container& values)
{
  using value_type = typename Container::value_type;
  return contiguous_sum<std::vector<value_type>>(target_sum, values);
}

/**
 * Base test class template.
 *
 * @tparam IndexedType `pddcp::indexed_type<I, T>`, *Container* `element_type`
 */
template <typename IndexedType>
class DailyTest102 : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_CONTAINER_HELPER_TYPES(IndexedType);
};

// input types used in the test fixture specializations. index types allows for
// unique wrapper types that have the same element_type member.
using InputType1 = pddcp::indexed_type<0, std::vector<unsigned int>>;
using InputType2 = pddcp::indexed_type<1, std::vector<unsigned short>>;
using InputType3 = pddcp::indexed_type<2, std::vector<long>>;

/**
 * Specialization for the sample input/output pair.
 */
template <>
class DailyTest102<InputType1> : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_CONTAINER_HELPER_TYPES(InputType1);
protected:
  static inline const element_type input_{1, 2, 3, 4, 5};
  static inline constexpr value_type target_ = 9;
  static inline const std::vector<value_type> output_{2, 3, 4};
};

/**
 * Specialization for a custom input/output pair with only unsigned values.
 *
 * `contiguous_sum` will end up exiting the range-for loop since the last value
 * in the input container is needed to meet the target value.
 */
template <>
class DailyTest102<InputType2> : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_CONTAINER_HELPER_TYPES(InputType2);
protected:
  static inline const element_type input_{3, 5, 1, 7, 10, 4};
  static inline constexpr value_type target_ = 14;
  static inline const std::vector<value_type> output_{10, 4};
};

/**
 * Specialization for a custom input/output pair with signed values.
 */
template <>
class DailyTest102<InputType3> : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_CONTAINER_HELPER_TYPES(InputType3);
protected:
  static inline const element_type input_{3, 1, 2, 12, 5, 6};
  static inline constexpr value_type target_ = 17;
  static inline const std::vector<value_type> output_{12, 5};
};


using DailyTest102Types = ::testing::Types<InputType1, InputType2, InputType3>;
TYPED_TEST_SUITE(DailyTest102, DailyTest102Types);

/**
 * Test that `contiguous_sum` works as expected.
 */
TYPED_TEST(DailyTest102, TypedTest)
{
  EXPECT_EQ(
    TestFixture::output_,
    contiguous_sum(TestFixture::target_, TestFixture::input_)
  );
}

}  // namespace
