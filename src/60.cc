/**
 * @file 60.cc
 * @author Derek Huang
 * @brief #60 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Facebook
 *
 * Given a multiset of integers, return whether it can be partitioned into two
 * subsets whose sums are the same. For example, given the multiset
 * {15, 5, 20, 10, 35, 15, 10}, it would return true, since we can split it up
 * into {15, 5, 10, 15, 10} and {20, 35}, which both add up to 55. Given the
 * multiset {15, 5, 20, 10, 35}, it would return false, since we can't split it
 * up into two subsets that add up to the same sum.
 */

#include <algorithm>
#include <iostream>
#include <numeric>
#include <type_traits>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "pddcp/algorithm.h"
#include "pddcp/utility.h"
#include "pddcp/warnings.h"

namespace {

/**
 * Returns `true` if values can be partitioned into two subsets of equal sum.
 *
 * Subset is used loosely, more precisely these are multisets of the same size.
 *
 * @tparam Container *Container* with integral `value_type`
 */
template <typename Container>
bool can_partition(const Container& values)
{
  // only allow containers with integral value_type
  using value_type = typename Container::value_type;
  using size_type = typename Container::size_type;
  static_assert(std::is_integral_v<value_type>);
  // first compute sum of values. if this is odd, cannot equally partition
  auto total_sum = std::accumulate(values.begin(), values.end(), value_type{});
  if (total_sum % 2)
    return false;
  // otherwise, compute the target sum (halved). here we take the
  // pseudo-polynomial time DP approach, where complexity is O(Cn), C equal to
  // the positive value sum P minus the negative value sum N in the container,
  // plus 1 for zero, n the number of container values. space is also O(Cn).
  auto target_sum = total_sum / 2;
  // sums of negative and positive values. negative sum is zero if the type is
  // unsigned; this is determined at compile time to reduce branching.
  auto neg_sum = pddcp::negative_sum(values);
  // positive sum is just total_sum if type is unsigned, no need to sum again.
  // due to the constexpr branch, total_sum may end up unused, which will
  // trigger a warning from Clang about an unused lambda capture.
PDDCP_GNU_WARNING_PUSH()
PDDCP_GNU_WARNING_DISABLE(unused-lambda-capture)
  auto pos_sum = [&total_sum, &values]
  {
    if constexpr (std::is_unsigned_v<value_type>)
      return total_sum;
    else
      return pddcp::positive_sum(values);
  }();
PDDCP_GNU_WARNING_POP()
  // conceptually, we can imagine a state matrix, representing each column as
  // holding the range of subset sum values, i.e. we have |neg_sum| + pos_sum +
  // 1 rows (don't forget 0), values.size() columns for the subset elements.
PDDCP_GNU_WARNING_PUSH()
PDDCP_GNU_WARNING_DISABLE(unused-lambda-capture)
  const auto offset = [&neg_sum]
  {
    // again, must be zero if type is unsigned
    if constexpr (std::is_unsigned_v<value_type>)
      return value_type{};
    else
      return -neg_sum;
  }();
PDDCP_GNU_WARNING_POP()
  const auto n_range_values = offset + pos_sum + 1;
  const auto n_elems = values.size();
  // we can, however, model the state matrix as a flat vector such that the
  // state (v, s) is corresponds to index through this formula:
  //
  // (s - 1) * n_range_values + v + offset
  //
  // the below lambda reduces some typing for us. Clang warns that no lambda
  // capture is required for offset; this is due to choosing the true branch of
  // the constexpr if in the lambda invoked for offset.
PDDCP_GNU_WARNING_PUSH()
PDDCP_GNU_WARNING_DISABLE(unused-lambda-capture)
  auto state_index = [&n_range_values, &offset](value_type v, size_type s)
  {
    return (s - 1) * n_range_values + v + offset;
  };
PDDCP_GNU_WARNING_POP()
  // then, we simply enumerate all the possible states. each (v, s) pair should
  // be interpreted as "there exists a subset of the first s elements of the
  // container values that sums to v". only n_range_values * n_elems states.
  std::vector<bool> states(n_range_values * n_elems);
  // the first column will have only index offset and index values[0] + offset
  // set to true, as given 1 element the subset can sum to itself or zero.
  states[state_index(0, 1)] = true;
  states[state_index(values[0], 1)] = true;
  // now we set the other columns' elements to true as appropriate, i.e. at
  // index (v, s) we mark true if either (v, s - 1) is true or if
  // (v - values[s - 1], s - 1) is true. this corresponds to excluding or
  // including values[s - 1] from the subset sum. (v - values[s - 1], s - 1)
  // may index outside the state vector, so we have to check bounds first.
  for (size_type s = 2; s < n_elems + 1; s++)
    for (value_type v = neg_sum; v < pos_sum + 1; v++)
      if (
        states[state_index(v, s - 1)] ||
        (
          v >= values[s - 1] &&
          v < n_range_values + values[s - 1] &&
          states[state_index(v - values[s - 1], s - 1)]
        )
      )
        states[state_index(v, s)] = true;
  // having completed our loop, just report the target index state
  return states[state_index(target_sum, n_elems)];
}

/**
 * Base test class template.
 *
 * Create specializations to use `TYPED_TEST` like a parametrized test.
 *
 * @tparam IndexedType `pddcp::indexed_type<I, T>`, *Container* `element_type`
 */
template <typename IndexedType>
class DailyTest60 : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_CONTAINER_HELPER_TYPES(IndexedType);
};

// input types used in the specializations + TYPED_TEST_SUITE macro
using InputType1 = pddcp::indexed_type<0, std::vector<unsigned int>>;
using InputType2 = pddcp::indexed_type<1, std::vector<int>>;

/**
 * Specialization for the first given input/output pair.
 */
template <>
class DailyTest60<InputType1> : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_CONTAINER_HELPER_TYPES(InputType1);
protected:
  static inline const element_type values_{15, 5, 20, 10, 35, 15, 10};
  static inline constexpr bool res_ = true;
};

/**
 * Specializaton for the second given input/output pair.
 */
template <>
class DailyTest60<InputType2> : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_CONTAINER_HELPER_TYPES(InputType2);
protected:
  static inline const element_type values_{15, 5, 20, 10, 35};
  static inline constexpr bool res_ = false;
};

using DailyTest60Types = ::testing::Types<InputType1, InputType2>;
TYPED_TEST_SUITE(DailyTest60, DailyTest60Types);

/**
 * Check that `can_partition` works as expected.
 */
TYPED_TEST(DailyTest60, TypedTest)
{
  EXPECT_EQ(TestFixture::res_, can_partition(TestFixture::values_));
}

}  // namespace
