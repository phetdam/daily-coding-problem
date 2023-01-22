/**
 * @file matrix_test.cc
 * @author Derek Huang
 * @brief matrix.h unit tests
 * @copyright MIT License
*/

#include "pddcp/matrix.h"

#include <algorithm>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

namespace {

using matrix_shape = std::pair<std::size_t, std::size_t>;

/**
 * Helper macro for "unpacking" a `matrix_shape`.
 *
 * @param shape `matrix_shape` to unpack
 */
#define PDDCP_MATRIX_SHAPE_UNPACK(shape) shape.first, shape.second

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

  /**
   * Ctor.
   *
   * The matrices are non-static members so we can mutate them as needed.
   */
  MatrixTest()
    : mat_default_{},
      mat_square_{{1, 2}, {3, 4}},
      mat_vector_{1, 2, 3, 4, 5}
  {}

  // dimensions and values
  static inline constexpr matrix_shape shape_default_{4, 5};
  static inline constexpr matrix_shape shape_square_{2, 2};
  static inline constexpr matrix_shape shape_vector_{5, 1};

  // our matrices
  matrix_type<PDDCP_MATRIX_SHAPE_UNPACK(shape_default_)> mat_default_;
  matrix_type<PDDCP_MATRIX_SHAPE_UNPACK(shape_square_)> mat_square_;
  matrix_type<PDDCP_MATRIX_SHAPE_UNPACK(shape_vector_)> mat_vector_;
};

using MatrixTestType = ::testing::Types<float, int, unsigned long>;
TYPED_TEST_SUITE(MatrixTest, MatrixTestType);

/**
 * Test that matrix value getting works as expected.
 */
TYPED_TEST(MatrixTest, IndexReadTest)
{
  EXPECT_EQ(0, this->mat_default_(0, this->mat_default_.n_cols() - 1));
  EXPECT_EQ(3, this->mat_square_(this->mat_square_.n_rows() - 1, 0));
  EXPECT_EQ(2, this->mat_vector_(1, 0));
}

/**
 * Helper macro for updating and checking the set value of a matrix.
 *
 * @param mat matrix object
 * @param value value
 * @param row row index
 * @param col col index
 */
#define PDDCP_MATRIX_EXPECT_SET_EQ(mat, value, row, col) \
  mat(row, col) = value; \
  EXPECT_EQ(value, mat(row, col))

/**
 * Test that matrix value setting works as expected.
 */
TYPED_TEST(MatrixTest, IndexWriteTest)
{
  PDDCP_MATRIX_EXPECT_SET_EQ(
    this->mat_default_, 100, 0, this->mat_default_.n_cols() - 1
  );
  PDDCP_MATRIX_EXPECT_SET_EQ(
    this->mat_square_, 190, 0, this->mat_square_.n_cols() - 1
  );
  PDDCP_MATRIX_EXPECT_SET_EQ(
    this->mat_vector_, 981, this->mat_vector_.n_rows() - 1, 0
  );
}

/**
 * Test that matrix `operator+` overloads are selected as expected.
 *
 * Macros use extra parentheses to keep macro from interpreting template
 * arguments as separate macro arguments.
 */
TYPED_TEST(MatrixTest, PlusOverloadTest)
{
  // matrix + matrix
  EXPECT_TRUE(
    (
      std::is_same_v<
        TypeParam,
        typename decltype(this->mat_square_ + this->mat_square_)::value_type
      >
    )
  );
  // matrix + value_type scalar
  EXPECT_TRUE(
    (
      std::is_same_v<
        TypeParam,
        typename decltype(this->mat_default_ + TypeParam{5})::value_type
      >
    )
  );
  // value_type scalar + matrix
  EXPECT_TRUE(
    (
      std::is_same_v<
        TypeParam,
        typename decltype(TypeParam{100} + this->mat_vector_)::value_type
      >
    )
  );
}

/**
 * Test that matrix binary `operator-` overloads are selected as expected.
 */
TYPED_TEST(MatrixTest, MinusOverloadTest)
{
  // matrix - matrix
  EXPECT_TRUE(
    (
      std::is_same_v<
        TypeParam,
        typename decltype(this->mat_square_ - this->mat_square_)::value_type
      >
    )
  );
  // matrix - value_type scalar
  EXPECT_TRUE(
    (
      std::is_same_v<
        TypeParam,
        typename decltype(this->mat_default_ - TypeParam{14})::value_type
      >
    )
  );
  // value_type scalar - matrix
  EXPECT_TRUE(
    (
      std::is_same_v<
        TypeParam,
        typename decltype(TypeParam{100} - this->mat_vector_)::value_type
      >
    )
  );
}

/**
 * Test that matrix unary `operator-` works as expected.
 *
 * Nothing is done for unsigned types, as we would get a compile error. Also
 * indirectly tests `operator+`, `operator==`, as we check a + (-a) + a == a.
 */
TYPED_TEST(MatrixTest, NegationTest)
{
  if constexpr (std::is_signed_v<TypeParam>) {
    // 0 == -0
    EXPECT_EQ(this->mat_default_, -this->mat_default_);
    // a + (-a) + a == a
    EXPECT_EQ(
      this->mat_square_,
      this->mat_square_ + (-this->mat_square_) + this->mat_square_
    );
  }
  else
    GTEST_SKIP() << "skipping negation of unsigned type";
}

/**
 * Test that binary matrix `operator+`, `operator-` work as expected.
 *
 * Indirectly also tests `operator==` for comparison. Tests matrix/matrix,
 * scalar/matrix, matrix/scalar operations together.
 */
TYPED_TEST(MatrixTest, PlusMinusTest)
{
  // 0 == 0 + 0
  EXPECT_EQ(this->mat_default_, this->mat_default_ + this->mat_default_);
  // a == a + a - a, where we are not using unary operator-
  EXPECT_EQ(
    this->mat_square_,
    this->mat_square_ + this->mat_square_ - this->mat_square_
  );
  // b == 1 + b - 1
  EXPECT_EQ(this->mat_vector_, 1 + this->mat_vector_ - 1);
}

/**
 * Temporary test for the `dense_matrix` template implemented using CRTP.
 *
 * CRTP is also formally known as "F-bounded quantification".
 */
TYPED_TEST(MatrixTest, FBoundTest)
{
  // note: right now only dense_matrix is implemented
  pddcp::dense_matrix<4, 5, TypeParam> cheese;
  static_assert(
    std::is_same_v<TypeParam, typename decltype(cheese)::value_type>,
    "cheese value_type must equal TypeParam"
  );
  EXPECT_EQ(cheese.col_count, cheese.n_cols());
  EXPECT_EQ(cheese(0, 3), cheese.at(0, 3));
  EXPECT_EQ(cheese, cheese);
  // skip this test for unsigned type (can't use unary operator-)
  if constexpr (std::is_signed_v<TypeParam>)
    EXPECT_EQ(cheese, cheese + (-cheese) + cheese);

}

}  // namespace
