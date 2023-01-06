/**
 * @file matrix_test.cc
 * @author Derek Huang
 * @brief matrix.h unit tests
 * @copyright MIT License
*/

#include "pddcp/matrix.h"

#include <algorithm>
#include <cstdint>
#include <vector>

#include <gtest/gtest.h>

namespace {

/**
 * Test fixture template for testing matrix methods.
 *
 * @tparam T matrix `value_type`
 */
template <typename T>
class MatrixTest : public ::testing::Test {
protected:
  template <std::size_t rows, std::size_t cols>
  using matrix_type = pddcp::matrix<rows, cols, T>;

  static inline const matrix_type<4, 5> matrix_default_;
  static inline const matrix_type<2, 2> matrix_square_{{1, 2}, {3, 4}};
  static inline const matrix_type<5, 1> matrix_vector_{1, 2, 3, 4, 5};
};

using MatrixTestType = ::testing::Types<double, int, unsigned long>;
TYPED_TEST_SUITE(MatrixTest, MatrixTestType);

/**
 * Test that matrix value getting works as expected.
 */
TYPED_TEST(MatrixTest, IndexReadTest)
{
  EXPECT_EQ(0, this->matrix_default_(0, 3));
  EXPECT_EQ(3, this->matrix_square_(1, 0));
  EXPECT_EQ(2, this->matrix_vector_(1, 0));
}

}  // namespace
