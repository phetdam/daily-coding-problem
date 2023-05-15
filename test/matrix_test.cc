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
 * Traits classes that defines matrix types based on shape and enum value.
 */
/**
 * Matrix impl type enum for traits classes.
 */
enum class matrix_impl_type { dense, sparse };

/**
 * Matrix impl traits class defaulting to the `dense_matrix`.
 *
 * Provides a `matrix_type` template and the associated `value_type`.
 */
template <matrix_impl_type, typename T>
struct matrix_impl_traits {
  template <std::size_t n_rows, std::size_t n_cols>
  using matrix_type = pddcp::dense_matrix<n_rows, n_cols, T>;
  using value_type = T;
};

/**
 * Matrix impl traits class for the `sparse_matrix`.
 */
template <typename T>
struct matrix_impl_traits<matrix_impl_type::sparse, T> {
  template <std::size_t n_rows, std::size_t n_cols>
  using matrix_type = pddcp::sparse_matrix<n_rows, n_cols, T>;
  using value_type = T;
};

/**
 * Macros for the values used to initialize the dense matrices.
 */
#define PDDCP_DENSE_MATRIX_VALUES_DEFAULT
#define PDDCP_DENSE_MATRIX_VALUES_SQUARE {1, 2}, {3, 4}
#define PDDCP_DENSE_MATRIX_VALUES_VECTOR 1, 2, 3, 4, 5

/**
 * Macros for the values used to initialize the sparse matrices.
 */
#define PDDCP_SPARSE_MATRIX_VALUES_DEFAULT
#define PDDCP_SPARSE_MATRIX_VALUES_SQUARE {{1, 0}, 3}, {{0, 1}, 10}
#define PDDCP_SPARSE_MATRIX_VALUES_VECTOR {{1, 0}, 2}, {{4, 0}, 88}

/**
 * Test class base that defines the test matrix shapes.
 */
class MatrixTestBase : public ::testing::Test {
protected:
  static inline constexpr matrix_shape shape_default_{4, 5};
  static inline constexpr matrix_shape shape_square_{2, 2};
  static inline constexpr matrix_shape shape_vector_{5, 1};
};

/**
 * Test fixture template for testing [dense] matrix methods.
 *
 * @tparam T matrix `value_type`
 */
template <typename Traits>
class MatrixTest : public MatrixTestBase {
protected:
  template <std::size_t n_rows, std::size_t n_cols>
  using matrix_type = typename Traits::template matrix_type<n_rows, n_cols>;
  using value_type = typename Traits::value_type;

  /**
   * Ctor.
   *
   * The matrices are non-static members so we can mutate them as needed.
   */
  MatrixTest()
    : mat_default_{PDDCP_DENSE_MATRIX_VALUES_DEFAULT},
      mat_square_{PDDCP_DENSE_MATRIX_VALUES_SQUARE},
      mat_vector_{PDDCP_DENSE_MATRIX_VALUES_VECTOR}
  {}

  // our matrices
  matrix_type<PDDCP_MATRIX_SHAPE_UNPACK(shape_default_)> mat_default_;
  matrix_type<PDDCP_MATRIX_SHAPE_UNPACK(shape_square_)> mat_square_;
  matrix_type<PDDCP_MATRIX_SHAPE_UNPACK(shape_vector_)> mat_vector_;
};

/**
 * Test fixture template specialization for testing sparse matrix methods.
 */
template <typename T>
class MatrixTest<matrix_impl_traits<matrix_impl_type::sparse, T>>
  : public MatrixTestBase {
protected:
  template <std::size_t n_rows, std::size_t n_cols>
  using matrix_type = pddcp::sparse_matrix<n_rows, n_cols, T>;
  using value_type = T;

  /**
   * Ctor.
   *
   * The matrices are non-static members so we can mutate them as needed.
   */
  MatrixTest()
    : mat_default_{PDDCP_SPARSE_MATRIX_VALUES_DEFAULT},
      mat_square_{PDDCP_SPARSE_MATRIX_VALUES_SQUARE},
      mat_vector_{PDDCP_SPARSE_MATRIX_VALUES_VECTOR}
  {}

  // our matrices
  matrix_type<PDDCP_MATRIX_SHAPE_UNPACK(shape_default_)> mat_default_;
  matrix_type<PDDCP_MATRIX_SHAPE_UNPACK(shape_square_)> mat_square_;
  matrix_type<PDDCP_MATRIX_SHAPE_UNPACK(shape_vector_)> mat_vector_;
};

using MatrixTestTypes = ::testing::Types<
  matrix_impl_traits<matrix_impl_type::dense, float>,
  matrix_impl_traits<matrix_impl_type::dense, int>,
  matrix_impl_traits<matrix_impl_type::dense, unsigned long>,
  matrix_impl_traits<matrix_impl_type::sparse, double>
>;
TYPED_TEST_SUITE(MatrixTest, MatrixTestTypes);

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
  using value_type = typename TestFixture::value_type;
  // matrix + matrix
  EXPECT_TRUE(
    (
      std::is_same_v<
        value_type,
        typename decltype(this->mat_square_ + this->mat_square_)::value_type
      >
    )
  );
  // matrix + value_type scalar
  EXPECT_TRUE(
    (
      std::is_same_v<
        value_type,
        typename decltype(this->mat_default_ + value_type{5})::value_type
      >
    )
  );
  // value_type scalar + matrix
  EXPECT_TRUE(
    (
      std::is_same_v<
        value_type,
        typename decltype(value_type{100} + this->mat_vector_)::value_type
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
  if constexpr (std::is_signed_v<typename TestFixture::value_type>) {
    // 0 == -0
    EXPECT_EQ(this->mat_default_, -this->mat_default_);
    // a + (-a) + a == a
    EXPECT_EQ(
      this->mat_square_,
      this->mat_square_ + (-this->mat_square_) + this->mat_square_
    );
  }
  else
    GTEST_SKIP();
}

/**
 * Test that matrix binary `operator-` overloads are selected as expected.
 */
TYPED_TEST(MatrixTest, MinusOverloadTest)
{
  using value_type = typename TestFixture::value_type;
  // matrix - matrix
  EXPECT_TRUE(
    (
      std::is_same_v<
        value_type,
        typename decltype(this->mat_square_ - this->mat_square_)::value_type
      >
    )
  );
  // matrix - value_type scalar
  EXPECT_TRUE(
    (
      std::is_same_v<
        value_type,
        typename decltype(this->mat_default_ - value_type{14})::value_type
      >
    )
  );
  // value_type scalar - matrix
  EXPECT_TRUE(
    (
      std::is_same_v<
        value_type,
        typename decltype(value_type{100} - this->mat_vector_)::value_type
      >
    )
  );
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

}  // namespace
