/**
 * @file 151.cc
 * @author Derek Huang
 * @brief #151 from Daily Coding Problem
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
#include <iostream>
#include <type_traits>
#include <utility>

#include <gtest/gtest.h>

#include "pddcp/matrix.h"

namespace {

template <typename Matrix, typename IndexType, typename ValueType>
auto flood_fill(
  pddcp::matrix_base<Matrix>& mat,
  const std::pair<IndexType, IndexType>& index,
  ValueType fill_value)
{
  using traits_type = pddcp::matrix_traits<Matrix>;
  using value_type = typename traits_type::value_type;
  using index_pair_type = std::decay_t<decltype(index)>;
  static_assert(std::is_convertible_v<ValueType, value_type>);
  static_assert(std::is_unsigned_v<IndexType>);
  // can't fill any cells outside the grid
  std::size_t n_filled = 0;
  if (
    index.first >= traits_type::row_count ||
    index.second >= traits_type::col_count
  )
    return n_filled;
  // value at the current index that we want to replace
  const auto old_value = mat.at(index.first, index.second);
  // otherwise, perform breadth-first fill from the starting index
  std::deque<index_pair_type> index_queue{index};
  while (index_queue.size()) {
    // get row and column indices, perform the fill, update count, pop front
    auto [row, col] = index_queue.front();
    mat.at(row, col) = fill_value;
    n_filled++;
    index_queue.pop_front();
    // add valid neighbors to the queue
    if (row > 0 && mat.at(row - 1, col) == old_value)
      index_queue.push_back({row - 1, col});
    if (row < traits_type::row_count - 1 && mat.at(row + 1, col) == old_value)
      index_queue.push_back({row + 1, col});
    if (col > 0 && mat.at(row, col - 1) == old_value)
      index_queue.push_back({row, col - 1});
    if (col < traits_type::col_count - 1 && mat.at(row, col + 1) == old_value)
      index_queue.push_back({row, col + 1});
  }
  return n_filled;
}

template <typename Matrix>
class DailyTest151 : public ::testing::Test {
public:
  using matrix_type = Matrix;
};

template <>
class DailyTest151<pddcp::dense_matrix<4, 3, char>> : public ::testing::Test {
public:
  using matrix_type = pddcp::dense_matrix<4, 3, char>;
  using size_type = typename matrix_type::size_type;
  using index_type = std::pair<size_type, size_type>;
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
  static inline constexpr char fill_value_ = 'G';
  static inline const index_type fill_point_{2, 2};
};

using DailyTest151Types = ::testing::Types<pddcp::dense_matrix<4, 3, char>>;
TYPED_TEST_SUITE(DailyTest151, DailyTest151Types);

TYPED_TEST(DailyTest151, TypedTest)
{
  flood_fill(
    this->input_matrix_, TestFixture::fill_point_, TestFixture::fill_value_
  );
  EXPECT_EQ(TestFixture::output_matrix_, this->input_matrix_);
}

}  // namespace
