/**
 * @file 76.cc
 * @author Derek Huang
 * @brief #76 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Google.
 *
 * You are given an N by M 2D matrix of lowercase letters. Determine the
 * minimum number of columns that can be removed to ensure that each row is
 * ordered from top to bottom lexicographically. That is, the letter at each
 * column is lexicographically later as you go down each row. It does not
 * matter whether each row itself is ordered lexicographically.
 *
 * For example, given the following table:
 *
 * cba
 * daf
 * ghi
 *
 * This is not ordered because of the a in the center. We can remove the second
 * column to make it ordered:
 *
 * ca
 * df
 * gi
 *
 * So your function should return 1, since we only needed to remove 1 column.
 * As another example, given the following table:
 *
 * abcdef
 *
 * Your function should return 0, since the rows are already ordered (there's
 * only one row). As another example, given the following table:
 *
 * zyx
 * wvu
 * tsr
 *
 * Your function should return 3, since we would need to remove all the columns
 * to order it.
 */

#include <cstdint>
#include <type_traits>

#include <gtest/gtest.h>

#include "pddcp/matrix.h"
#include "pddcp/utility.h"

namespace {

/**
 * Return number of columns to remove to ensure sorted matrix columns.
 *
 * In other words, returns the number of columns whose values are not in
 * ascending order as one goes down the column. Simple O(N) scan suffices.
 *
 * @tparam Matrix `pddcp::matrix_base<T>` CRTP subclass
 */
template <typename Matrix>
auto min_column_removals(const pddcp::matrix_base<Matrix>& mat)
{
  // type aliases + check that value_type is integral
  using traits_type = pddcp::matrix_traits<Matrix>;
  using size_type = typename traits_type::size_type;
  using value_type = typename traits_type::value_type;
  static_assert(std::is_integral_v<value_type>);
  // simply count number of columns that aren't sorted
  std::size_t n_unsorted = 0;
  for (size_type c_i = 0; c_i < traits_type::col_count; c_i++) {
    // current column value
    auto cur_value = mat(0, c_i);
    for (size_type r_i = 1; r_i < traits_type::row_count; r_i++) {
      // sorted columns have increasing values as we go down
      if (cur_value >= mat(r_i, c_i)) {
        n_unsorted++;
        break;
      }
    }
  }
  return n_unsorted;
}

/**
 * Base test class template.
 *
 * Create specializations to use `TYPED_TEST` like a parametrized test.
 *
 * @tparam IndexedType `pddcp::indexed_type<I, T>` with a
 *  `pddcp::matrix_base<T>` CRTP subclass as the `element_type`
 */
template <typename IndexedType>
class DailyTest76 : public ::testing::Test {
public:
  PDDCP_MATRIX_HELPER_TYPES(typename IndexedType::element_type);
};

// inputs types used in the specializations + TYPED_TEST_SUITE macro. note how
// we use indexed_type here to create a unique input type.
using InputType1 = pddcp::indexed_type<0, pddcp::dense_matrix<3, 3, char>>;
using InputType2 = pddcp::indexed_type<2, pddcp::dense_matrix<1, 6, char>>;
using InputType3 = pddcp::indexed_type<3, typename InputType1::element_type>;

/**
 * `DailyTest76` specialization for the first sample input/output pair.
 *
 * Input matrix:
 *
 * cba
 * daf
 * ghi
 */
template <>
class DailyTest76<InputType1> : public ::testing::Test {
public:
  PDDCP_MATRIX_HELPER_TYPES(typename InputType1::element_type);

protected:
  DailyTest76() : input_{{'c', 'b', 'a'}, {'d', 'a', 'f'}, {'g', 'h', 'i'}} {};
  matrix_type input_;
  static inline constexpr std::size_t output_ = 1;
};

/**
 * `DailyTest76` specialization for the second sample input/output pair.
 *
 * Input matrix:
 *
 * abcdef
 */
template <>
class DailyTest76<InputType2> : public ::testing::Test {
public:
  PDDCP_MATRIX_HELPER_TYPES(typename InputType2::element_type);

protected:
  DailyTest76() : input_{{'a', 'b', 'c', 'd', 'e', 'f'}} {};
  matrix_type input_;
  static inline constexpr std::size_t output_ = 0;
};

/**
 * `DailyTest76` specialization for the third sample input/output pair.
 *
 * Input matrix:
 *
 * zyx
 * wvu
 * tsr
 */
template <>
class DailyTest76<InputType3> : public ::testing::Test {
public:
  PDDCP_MATRIX_HELPER_TYPES(typename InputType3::element_type);

protected:
  DailyTest76() : input_{{'z', 'y', 'x'}, {'w', 'v', 'u'}, {'t', 's', 'r'}} {};
  matrix_type input_;
  static inline constexpr std::size_t output_ = 3;
};

using DailyTest76Types = ::testing::Types<InputType1, InputType2, InputType3>;
TYPED_TEST_SUITE(DailyTest76, DailyTest76Types);

/**
 * Test that `min_column_removals` works as expected.
 */
TYPED_TEST(DailyTest76, TypedTest)
{
  EXPECT_EQ(TestFixture::output_, min_column_removals(this->input_));
}

}  // namespace
