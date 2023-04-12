/**
 * @file 151.cc
 * @author Derek Huang
 * @brief #151 from Daily Coding Problem
 * @copyright MIT License
 *
 * Given a 2-D matrix representing an image, a location of a pixel in the
 * screen and a color C, replace the color of the given pixel and all adjacent
 * same colored pixels with C. For example, given the following matrix, and
 * location pixel of (2, 2), and 'G' for green:
 *
 * B B W
 * W W W
 * W W W
 * B B B
 *
 * Becomes
 *
 * B B G
 * G G G
 * G G G
 * B B B
 */

#include <cstdint>
#include <deque>
#include <type_traits>
#include <utility>

#include <gtest/gtest.h>

#include "pddcp/matrix.h"

namespace {

/**
 * Flood-fill a matrix with a value starting from a given index.
 *
 * @tparam Matrix `pddcp::matrix_base<T>` CRTP subclass
 * @tparam ValueType Type convertiable to `pddcp::matrix_traits<T>::value_type`
 *
 * @param mat Matrix to flood fill
 * @param index Index to start filling at
 * @param fill_value Value to fill with
 * @returns Number of filled cells
 */
template <typename Matrix, typename ValueType>
auto flood_fill(
  pddcp::matrix_base<Matrix>& mat,
  const typename pddcp::matrix_traits<Matrix>::index_type index,
  ValueType fill_value)
{
  // type aliases + check that conversion is allowed
  using traits_type = pddcp::matrix_traits<Matrix>;
  using value_type = typename traits_type::value_type;
  using index_type = std::decay_t<decltype(index)>;
  static_assert(std::is_convertible_v<ValueType, value_type>);
  // can't fill any cells outside the grid
  std::size_t n_filled = 0;
  if (
    index.first >= traits_type::row_count ||
    index.second >= traits_type::col_count
  )
    return n_filled;
  // value at the current index that we want to replace
  const auto old_value = mat(index);
  // otherwise, perform breadth-first fill from the starting index
  std::deque<index_type> index_queue{index};
  while (index_queue.size()) {
    // get row and column indices, perform the fill, update count, pop front
    auto [row, col] = index_queue.front();
    mat(row, col) = fill_value;
    n_filled++;
    index_queue.pop_front();
    // add valid neighbors to the queue
    if (row > 0 && mat(row - 1, col) == old_value)
      index_queue.push_back({row - 1, col});
    if (row < traits_type::row_count - 1 && mat(row + 1, col) == old_value)
      index_queue.push_back({row + 1, col});
    if (col > 0 && mat(row, col - 1) == old_value)
      index_queue.push_back({row, col - 1});
    if (col < traits_type::col_count - 1 && mat(row, col + 1) == old_value)
      index_queue.push_back({row, col + 1});
  }
  return n_filled;
}

/**
 * Base class template.
 *
 * Create specializations to use `TYPED_TEST` like a parametrized test.
 *
 * @tparam Matrix `pddcp::matrix_base<T>` CRTP subclass
 */
template <typename Matrix>
class DailyTest151 : public ::testing::Test {
public:
  PDDCP_MATRIX_HELPER_TYPES(Matrix);
};

// matrix types used in the specializations + TYPED_TEST_SUITE macro
using MatrixType1 = pddcp::dense_matrix<4, 3, char>;
using MatrixType2 = pddcp::sparse_matrix<3, 4, unsigned int>;

/**
 * Specialization for the sample input/output pair.
 */
template <>
class DailyTest151<MatrixType1> : public ::testing::Test {
public:
  PDDCP_MATRIX_HELPER_TYPES(MatrixType1);

protected:
  DailyTest151()
    : input_matrix_{
      {'B', 'B', 'W'},
      {'W', 'W', 'W'},
      {'W', 'W', 'W'},
      {'B', 'B', 'B'}}
  {}

  matrix_type input_matrix_;

  static inline const matrix_type output_matrix_{
    {'B', 'B', 'G'},
    {'G', 'G', 'G'},
    {'G', 'G', 'G'},
    {'B', 'B', 'B'}
  };
  static inline constexpr value_type fill_value_ = 'G';
  static inline const index_type fill_point_{2, 2};
};

/**
 * Specialization for the custom input/output pair.
 *
 * The initial sparse matrix looks like
 *
 * 0 1 0 1
 * 0 2 0 0
 * 1 0 0 0
 *
 * After filling with 5 at (1, 2), the resulting sparse matrix is
 *
 * 0 1 5 1
 * 0 2 5 5
 * 1 5 5 5
 */
template <>
class DailyTest151<MatrixType2> : public ::testing::Test {
public:
  PDDCP_MATRIX_HELPER_TYPES(MatrixType2);

protected:
  DailyTest151()
    : input_matrix_{{{0, 1}, 1}, {{0, 3}, 1}, {{1, 1}, 2}, {{2, 0}, 1}}
  {}

  matrix_type input_matrix_;

  static inline const matrix_type output_matrix_{
    {{0, 1}, 1}, {{0, 3}, 1}, {{0, 2}, 5},
    {{1, 1}, 2}, {{1, 2}, 5}, {{1, 3}, 5},
    {{2, 0}, 1}, {{2, 1}, 5}, {{2, 2}, 5}, {{2, 3}, 5}
  };
  static inline constexpr value_type fill_value_ = 5;
  static inline const index_type fill_point_{1, 2};
};

using DailyTest151Types = ::testing::Types<MatrixType1, MatrixType2>;
TYPED_TEST_SUITE(DailyTest151, DailyTest151Types);

/**
 * Test that `flood_fill` works as expected.
 */
TYPED_TEST(DailyTest151, TypedTest)
{
  flood_fill(
    this->input_matrix_, TestFixture::fill_point_, TestFixture::fill_value_
  );
  EXPECT_EQ(TestFixture::output_matrix_, this->input_matrix_);
}

}  // namespace
