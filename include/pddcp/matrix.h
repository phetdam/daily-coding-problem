/**
 * @file matrix.h
 * @author Derek Huang
 * @brief C++ flat 2D matrix implementation
 * @copyright MIT License
 */

#ifndef PDDCP_MATRIX_H_
#define PDDCP_MATRIX_H_

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <initializer_list>
#include <limits>
#include <type_traits>
#include <vector>

namespace pddcp {

/**
 * Typical parameters for matrix class templates.
 */
#define PDDCP_MATRIX_TEMPLATE_PARAMS \
  std::size_t n_rows_, std::size_t n_cols_, typename T

/**
 * Macro for the traits members shared by the `matrix` subtypes.
 */
#define PDDCP_MATRIX_TRAITS_MEMBERS \
  using value_type = T; \
  using size_type = std::size_t; \
  static inline constexpr auto row_count = n_rows_; \
  static inline constexpr auto col_count = n_cols_

/**
 * Macro for the members pulled from the `matrix` base by the subtypes.
 *
 * @param template_name Name of the matrix class template
 */
#define PDDCP_MATRIX_BASE_MEMBERS(template_name) \
  using typename matrix_base<template_name<n_rows_, n_cols_, T>>::value_type; \
  using typename matrix_base<template_name<n_rows_, n_cols_, T>>::size_type; \
  using matrix_base<template_name<n_rows_, n_cols_, T>>::row_count; \
  using matrix_base<template_name<n_rows_, n_cols_, T>>::col_count

/**
 * Declaration for the `matrix` traits template.
 */
template <typename MatrixI>
struct matrix_traits;

/**
 * Forward declaration and type traits specialization for `dense_matrix`.
 */
template <PDDCP_MATRIX_TEMPLATE_PARAMS>
struct dense_matrix;

template <PDDCP_MATRIX_TEMPLATE_PARAMS>
struct matrix_traits<dense_matrix<n_rows_, n_cols_, T>> {
  PDDCP_MATRIX_TRAITS_MEMBERS;
};

/**
 * Forward declaration and type traits specialization for `sparse_matrix`.
 */
template <PDDCP_MATRIX_TEMPLATE_PARAMS>
struct sparse_matrix;

template <PDDCP_MATRIX_TEMPLATE_PARAMS>
struct matrix_traits<sparse_matrix<n_rows_, n_cols_, T>> {
  PDDCP_MATRIX_TRAITS_MEMBERS;
};

// forward decl for derived type
template <PDDCP_MATRIX_TEMPLATE_PARAMS>
struct derived_type;

// traits type partial specialization for derived_type
template <PDDCP_MATRIX_TEMPLATE_PARAMS>
struct matrix_traits<derived_type<n_rows_, n_cols_, T>> {
  PDDCP_MATRIX_TRAITS_MEMBERS;
};

/**
 * Matrix base class template.
 *
 * Uses CRTP to implement static polymorphism, removing the need for virtual
 * functions and their associated lookup overhead at runtime.
 *
 * Partially fulfills some *Container* requirements.
 *
 * @tparam Derived Matrix class
 */
template <typename Derived>
class matrix_base {
public:
  using value_type = typename matrix_traits<Derived>::value_type;
  using size_type = typename matrix_traits<Derived>::size_type;

  static_assert(
    std::is_arithmetic_v<value_type>, "only arithmetic types allowed"
  );

  // members for the row and column counts
  static inline constexpr auto row_count = matrix_traits<Derived>::row_count;
  static inline constexpr auto col_count = matrix_traits<Derived>::col_count;

  static_assert(row_count, "row_count must be positive");
  static_assert(col_count, "col_count must be positive");

  /**
   * Return number of matrix rows.
   */
  constexpr auto n_rows() const noexcept { return row_count; }

  /**
   * Return number of matrix columns.
   */
  constexpr auto n_cols() const noexcept { return col_count; }

  /**
   * Return number of elements in underlying value vector.
   */
  constexpr auto size() const
  {
    return static_cast<const Derived*>(this)->size();
  }

  /**
   * Return const reference to underlying value container.
   */
  decltype(auto) values() const
  {
    return static_cast<const Derived*>(this)->values();
  }

  /**
   * Return reference to the (i, j) matrix value.
   *
   * @param row row index
   * @param col col index
   */
  decltype(auto) at(size_type row, size_type col)
  {
    return static_cast<Derived*>(this)->at(row, col);
  }

  /**
   * Return const reference to the (i, j) matrix value.
   *
   * @param row row index
   * @param col col index
   */
  decltype(auto) at(size_type row, size_type col) const
  {
    return static_cast<const Derived*>(this)->at(row, col);
  }

  /**
   * Return reference to the (i, j) matrix value.
   *
   * @param row row index
   * @param col col index
   */
  decltype(auto) operator()(size_type row, size_type col)
  {
    return static_cast<Derived*>(this)->at(row, col);
  }

  /**
   * Return const reference to the (i, j) matrix value.
   *
   * @param row row index
   * @param col col index
   */
  decltype(auto) operator()(size_type row, size_type col) const
  {
    return static_cast<const Derived*>(this)->at(row, col);
  }
};

/**
 * Dense fixed-size 2D matrix implementation using flat data storage.
 *
 * @tparam n_rows_ number of rows
 * @tparam n_cols_ number of columns
 * @tparam T value type
 */
template <PDDCP_MATRIX_TEMPLATE_PARAMS>
class dense_matrix : public matrix_base<dense_matrix<n_rows_, n_cols_, T>> {
public:
  PDDCP_MATRIX_BASE_MEMBERS(dense_matrix);
  static constexpr bool is_sparse = false;

  /**
   * Default ctor.
   *
   * Preallocates `row_count` * `col_count` default-initialized elements.
   */
// constexpr in C++20
#if __cplusplus >= 202002L
  constexpr dense_matrix() : values_(row_count * col_count) {}
#else
  dense_matrix() : values_(row_count * col_count) {}
#endif  // __cplusplus >= 202002L

  /**
   * Dense matrix initializer list ctor.
   *
   * @param init Nested initializer list representing a matrix
   */
  dense_matrix(const std::initializer_list<std::initializer_list<T>>& init)
  {
    assert(row_count == init.size() && "row count must equal row_count");
    // reserve required space and populate values
    values_.reserve(row_count * col_count);
    for (const auto& row : init) {
      assert(row.size() == col_count && "col count must equal col_count");
      for (const auto& value : row)
        values_.push_back(value);
    }
  }

  /**
   * Column vector initializer list ctor.
   *
   * Cannot be used when `col_count` is not 1.
   *
   * @param init Initializer list representing column vector values
   */
  dense_matrix(const std::initializer_list<T>& init)
  {
    assert(row_count == init.size() && "row count must equal row_count");
    values_.reserve(row_count);
    for (const auto& value : init)
      values_.push_back(value);
  }

  /**
   * Return number of elements in underlying value vector.
   */
  constexpr auto size() const { return row_count * col_count; }

  /**
   * Return const reference to underlying value vector.
   */
  const auto& values() const { return values_; }

  /**
   * Return reference to the (i, j) matrix value.
   *
   * @param row row index
   * @param col col index
   */
#if __cplusplus >= 202002L
  constexpr
#endif  // __cplusplus >= 202002L
  T& at(size_type row, size_type col)
  {
    return values_.at(row * col_count + col);
  }

  /**
   * Return const reference to the (i, j) matrix value.
   *
   * @param row row index
   * @param col col index
   */
#if __cplusplus >= 202002L
  constexpr
#else
  const
#endif  // __cplusplus >= 202002L
  T& at(size_type row, size_type col) const
  {
    return values_.at(row * col_count + col);
  }

private:
   std::vector<T> values_;
};

/**
 * Loop through matrix indices in row-major order.
 *
 * @param n_rows number of matrix rows
 * @param n_cols number of matrix cols
 */
#define PDDCP_MATRIX_ROW_MAJOR_LOOP(n_rows, n_cols) \
  for (std::size_t i = 0; i < n_rows; i++) \
    for (std::size_t j = 0; j < n_cols; j++)

/**
 * Loop through matrix indices in column-major order.
 *
 * @param n_rows number of matrix rows
 * @param n_cols number of matrix cols
 */
#define PDDCP_MATRIX_COL_MAJOR_LOOP(n_rows, n_cols) \
  for (std::size_t j = 0; j < n_cols; j++) \
    for (std::size_t i = 0; i < n_rows; i++)

/**
 * Macro used for matrix binary operators to check shapes and alias types.
 *
 * Defines the type aliases `T` for `typename matrix_a::value_type`, `U` for
 * `typename matrix_b::value_type`, `row_count` and `col_count` dimensions.
 *
 * Adds some always-true `static_assert` and `(void)` statements to suppress
 * unused local typedef and unused variable warnings.
 *
 * @param matrix_a Type of the first matrix
 * @param matrix_b Type of the second matrix
 */
#define PDDCP_MATRIX_CHECK_SIZE_TYPES(matrix_a, matrix_b) \
  static_assert(matrix_a::row_count == matrix_b::row_count, "row mismatch"); \
  static_assert(matrix_a::col_count == matrix_b::col_count, "col mismatch"); \
  using Ta = typename matrix_a::value_type; \
  using Tb = typename matrix_b::value_type; \
  static_assert(std::is_same_v<Ta, Ta> && std::is_same_v<Tb, Tb>); \
  constexpr auto row_count = matrix_a::row_count; \
  constexpr auto col_count = matrix_a::col_count; \
  (void) row_count; \
  (void) col_count

/**
 * Macro for scalar template type parameter for binary operator overloads.
 *
 * Uses SFINAE through `std::enable_if` to enable binary operator overloads
 * taking a matrix type and a scalar in the candidate set only when needed,
 * which serves to prevent ambiguous overloads.
 *
 * @param type Scalar type
 */
#define PDDCP_MATRIX_ENABLE_IF_ARITHMETIC(type) \
  type, \
  typename = std::enable_if_t<std::is_arithmetic_v<type>>

/**
 * Check if two matrices with same shape are exactly equal.
 *
 * Uses exact equality only for non-floating types, where for floating types
 * Knuth's formulas are used. See https://stackoverflow.com/a/253874/14227825.
 *
 * If types `T`, `U` are not the same, they are cast to `double`.
 *
 * @tparam InMatrixA First matrix type
 * @tparam InMatrixB Second matrix type
 *
 * @param a first matrix
 * @param b second matrix
 */
template <typename InMatrixA, typename InMatrixB>
bool operator==(const matrix_base<InMatrixA>& a, const matrix_base<InMatrixB>& b)
{
  PDDCP_MATRIX_CHECK_SIZE_TYPES(InMatrixA, InMatrixB);
  // comparison type used in floating point comparison. if types are different,
  // we'll just have to cast everything to double for comparison
  using eps_type = std::conditional_t<std::is_same_v<Ta, Tb>, Ta, double>;
  // perform elementwise comparison
  PDDCP_MATRIX_ROW_MAJOR_LOOP(InMatrixA::row_count, InMatrixA::col_count) {
    auto& v_a = a(i, j);
    auto& v_b = b(i, j);
    // only use float comparison for floating types
    if constexpr (std::is_floating_point_v<Ta> || std::is_floating_point_v<Tb>) {
      // constexpr epsilon (no runtime cost) for float comparison
      constexpr auto eps = std::min(
        static_cast<eps_type>(std::numeric_limits<Ta>::epsilon()),
        static_cast<eps_type>(std::numeric_limits<Tb>::epsilon())
      );
      if (
        std::fabs(static_cast<eps_type>(v_a) - static_cast<eps_type>(v_b)) >
        std::min(static_cast<eps_type>(v_a), static_cast<eps_type>(v_b)) * eps
      )
        return false;
    }
    else if (v_a != v_b)
      return false;
  }
  return true;
}

/**
 * Return sum of two non-boolean matrices as a dense matrix with same shape.
 *
 * This is typically called from one of the other `operator+` overloads.
 *
 * @tparam OutMatrix Output matrix type
 * @tparam InMatrixA First matrix type
 * @tparam InMatrixB Second matrix type
 *
 * @param a first matrix
 * @param b second matrix
 */
template <typename OutMatrix, typename InMatrixA, typename InMatrixB>
auto operator+(const matrix_base<InMatrixA>& a, const matrix_base<InMatrixB>& b)
{
  PDDCP_MATRIX_CHECK_SIZE_TYPES(InMatrixA, InMatrixB);
  static_assert(!std::is_same_v<Ta, bool>, "left matrix has bool value_type");
  static_assert(!std::is_same_v<Tb, bool>, "right matrix has bool value_type");
  OutMatrix c;
#ifdef _OPENMP
  #pragma omp parallel for collapse(2)
#endif  // _OPENMP
  PDDCP_MATRIX_ROW_MAJOR_LOOP(row_count, col_count)
    c(i, j) = a(i, j) + b(i, j);
  return c;
}

/**
 * Return sum of two non-boolean matrices as a matrix with same shape.
 *
 * Base case for matrix summation that returns a `dense_matrix` of doubles if
 * the value types differ, otherwise a `dense_matrix` of the shared type.
 *
 * @tparam InMatrixA First matrix type
 * @tparam InMatrixB Second matrix type
 *
 * @param a first matrix
 * @param b second matrix
 */
template <typename InMatrixA, typename InMatrixB>
inline auto operator+(
  const matrix_base<InMatrixA>& a, const matrix_base<InMatrixB>& b)
{
  PDDCP_MATRIX_CHECK_SIZE_TYPES(InMatrixA, InMatrixB);
  using OutMatrix = std::conditional_t<
    std::is_same_v<Ta, Tb>,
    dense_matrix<row_count, col_count, Ta>,
    dense_matrix<row_count, col_count, double>
  >;
  return operator+<OutMatrix, InMatrixA, InMatrixB>(a, b);
}

/**
 * Return sum of two non-boolean sparse matrices as a matrix with same shape.
 *
 * Special case for sparse matrices that returns a sparse matrix. The return
 * `value_type` is `double` if `a` and `b` have different `value_type` members.
 *
 * @tparam n_rows_ number of rows
 * @tparam n_cols_ number of columns
 * @tparam T value type
 * @tparam U value type
 *
 * @param a first sparse matrix
 * @param b second sparse matrix
 */
template <PDDCP_MATRIX_TEMPLATE_PARAMS, typename U>
inline auto operator+(
  const sparse_matrix<n_rows_, n_cols_, T>& a,
  const sparse_matrix<n_rows_, n_cols_, U>& b)
{
  using InMatrixA = std::decay_t<decltype(a)>;
  using InMatrixB = std::decay_t<decltype(b)>;
  using OutMatrix = std::conditional_t<
    std::is_same_v<T, U>, InMatrixA, sparse_matrix<n_rows_, n_cols_, double>
  >;
  return operator+<OutMatrix, InMatrixA, InMatrixB>(a, b);
}

/**
 * Return sum of a non-boolean matrix + scalar.
 *
 * This is typically called from one of the other `operator+` overloads.
 *
 * @tparam OutMatrix Output matrix type
 * @tparam InMatrix Input matrix type
 * @tparam T scalar type
 *
 * @param mat matrix
 * @param value scalar value
 */
template <
  typename OutMatrix,
  typename InMatrix,
  typename PDDCP_MATRIX_ENABLE_IF_ARITHMETIC(T)>
auto operator+(const matrix_base<InMatrix>& mat, T value)
{
  PDDCP_MATRIX_CHECK_SIZE_TYPES(InMatrix, OutMatrix);
  static_assert(!std::is_same_v<Ta, bool>, "matrix has bool value_type");
  static_assert(!std::is_same_v<T, bool>, "scalar has bool value_type");
  OutMatrix out;
#ifdef _OPENMP
  #pragma omp parallel for collapse(2)
#endif  // _OPENMP
  PDDCP_MATRIX_ROW_MAJOR_LOOP(row_count, col_count)
    out(i, j) = mat(i, j) + value;
  return out;
}

/**
 * Return sum of a non-boolean matrix + scalar.
 *
 * Base overload selected that returns a `dense_matrix` of `T` if the matrix
 * `value_type` is `T`, otherwise `dense_matrix` of doubles.
 *
 * @tparam InMatrix Input matrix type
 * @tparam T scalar type
 */
template <typename InMatrix, typename PDDCP_MATRIX_ENABLE_IF_ARITHMETIC(T)>
inline auto operator+(const matrix_base<InMatrix>& mat, T value)
{
  using OutMatrix = std::conditional_t<
    std::is_same_v<T, typename InMatrix::value_type>,
    dense_matrix<InMatrix::row_count, InMatrix::col_count, T>,
    dense_matrix<InMatrix::row_count, InMatrix::col_count, double>
  >;
  return operator+<OutMatrix, InMatrix, T>(mat, value);
}

/**
 * Return sum of a scalar + non-boolean matrix.
 *
 * Base overload selected that returns a `dense_matrix` of `T` if the matrix
 * `value_type` is `T`, otherwise `dense_matrix` of doubles.
 *
 * @tparam InMatrix Input matrix type
 * @tparam T scalar type
 */
template <typename InMatrix, typename PDDCP_MATRIX_ENABLE_IF_ARITHMETIC(T)>
inline auto operator+(T value, const matrix_base<InMatrix>& mat)
{
  return mat + value;
}

/**
 * Return sum of non-boolean sparse matrix + scalar.
 *
 * The returned `sparse_matrix` has `value_type` `T` if it is the same as `U`,
 * otherwise its `value_type` will be `double`.
 *
 * @tparam n_rows_ number of matrix rows
 * @tparam n_cols_ number of matrix cols
 * @tparam T sparse matrix `value_type`
 * @tparam U scalar type
 *
 * @param mat sparse matrix
 * @param value scalar value
 */
template <
  PDDCP_MATRIX_TEMPLATE_PARAMS,
  typename PDDCP_MATRIX_ENABLE_IF_ARITHMETIC(U)>
inline auto operator+(const sparse_matrix<n_rows_, n_cols_, T>& mat, U value)
{
  using InMatrix = std::decay_t<decltype(mat)>;
  using OutMatrix = std::conditional_t<
    std::is_same_v<T, U>, InMatrix, sparse_matrix<n_rows_, n_cols_, double>
  >;
  return operator+<OutMatrix, InMatrix, U>(mat, value);
}

/**
 * Return sum of scalar + non-boolean sparse matrix.
 *
 * The returned `sparse_matrix` has `value_type` `T` if it is the same as `U`,
 * otherwise its `value_type` will be `double`.
 *
 * @tparam n_rows_ number of matrix rows
 * @tparam n_cols_ number of matrix cols
 * @tparam T sparse matrix `value_type`
 * @tparam U scalar type
 *
 * @param mat sparse matrix
 * @param value scalar value
 */
template <
  PDDCP_MATRIX_TEMPLATE_PARAMS,
  typename PDDCP_MATRIX_ENABLE_IF_ARITHMETIC(U)>
inline auto operator+(U value, const sparse_matrix<n_rows_, n_cols_, T>& mat)
{
  return mat + value;
}

/**
 * Unary negation operator.
 *
 * @tparam MatrixI matrix class
 *
 * @param mat matrix
 */
template <typename MatrixI>
auto operator-(const matrix_base<MatrixI>& mat)
{
  using T = typename MatrixI::value_type;
  static_assert(std::is_signed_v<T>, "T must be a signed type");
  MatrixI out;
#ifdef _OPENMP
  #pragma omp parallel for collapse(2)
#endif  // _OPENMP
  PDDCP_MATRIX_ROW_MAJOR_LOOP(MatrixI::row_count, MatrixI::col_count)
    out(i, j) = -mat(i, j);
  return out;
}

/**
 * Dense fixed-size 2D matrix implementation using flat data storage.
 *
 * Partially fulfills some *Container* requirements.
 *
 * @tparam n_rows_ number of rows
 * @tparam n_cols_ number of columns
 * @tparam T value type
 */
template <std::size_t n_rows_, std::size_t n_cols_, typename T = double>
class matrix {
public:
  static_assert(std::is_arithmetic_v<T>, "only arithmetic types allowed");

  using value_type = T;
  using size_type = std::size_t;

  static_assert(n_rows_, "n_rows must be positive");
  static_assert(n_cols_, "n_cols must be positive");

  // members for the row and column counts
  static inline constexpr auto row_count = n_rows_;
  static inline constexpr auto col_count = n_cols_;

  /**
   * Default ctor.
   *
   * Preallocates `row_count` * `col_count` default-initialized elements.
   */
// constexpr in C++20
#if __cplusplus >= 202002L
  constexpr matrix() : values_(row_count * col_count) {}
#else
  matrix() : values_(row_count * col_count) {}
#endif  // __cplusplus >= 202002L

  /**
   * Matrix initializer list ctor.
   *
   * @param init Nested initializer list representing a matrix
   */
  matrix(const std::initializer_list<std::initializer_list<T>>& init)
  {
    assert(row_count == init.size() && "row count must equal row_count");
    // reserve required space and populate values
    values_.reserve(row_count * col_count);
    for (const auto& row : init) {
      assert(row.size() == col_count && "col count must equal col_count");
      for (const auto& value : row)
        values_.push_back(value);
    }
  }

  /**
   * Column vector initializer list ctor.
   *
   * Cannot be used when `col_count` is not 1.
   *
   * @param init Initializer list representing column vector values
   */
  matrix(const std::initializer_list<T>& init)
  {
    assert(row_count == init.size() && "row count must equal row_count");
    values_.reserve(row_count);
    for (const auto& value : init)
      values_.push_back(value);
  }

  /**
   * Return const reference to underlying value vector.
   */
  const auto& values() const { return values_; }

  /**
   * Return number of matrix rows.
   */
  constexpr auto n_rows() const noexcept { return row_count; }

  /**
   * Return number of matrix columns.
   */
  constexpr auto n_cols() const noexcept { return col_count; }

  /**
   * Return number of elements in underlying value vector.
   */
  constexpr auto size() const { return row_count * col_count; }

  /**
   * Return const reference to the (i, j) matrix value.
   *
   * @param row row index
   * @param col col index
   */
#if __cplusplus >= 202002L
  constexpr
#else
  const
#endif  // __cplusplus >= 202002L
  T& at(size_type row, size_type col) const
  {
    return values_.at(row * col_count + col);
  }

  /**
   * Return const reference to the (i, j) matrix value.
   *
   * @param row row index
   * @param col col index
   */
#if __cplusplus >= 202002L
  constexpr
#else
  const
#endif  // __cplusplus >= 202002L
  T& operator()(size_type row, size_type col) const { return at(row, col); }

  /**
   * Return reference to the (i, j) matrix value.
   *
   * @param row row index
   * @param col col index
   */
#if __cplusplus >= 202002L
  constexpr
#endif  // __cplusplus >= 202002L
  T& at(size_type row, size_type col)
  {
    return values_.at(row * col_count + col);
  }

  /**
   * Return reference to the (i, j) matrix value.
   *
   * @param row row index
   * @param col col index
   */
#if __cplusplus >= 202002L
  constexpr
#endif  // __cplusplus >= 202002L
  T& operator()(size_type row, size_type col) { return at(row, col); }

private:
   std::vector<T> values_;
};

/**
 * Check if two matrices are exactly equal.
 *
 * Uses exact equality only for non-floating types, where for floating types
 * Knuth's formulas are used. See https://stackoverflow.com/a/253874/14227825.
 *
 * If types `T`, `U` are not the same, they are cast to `double`.
 *
 * @tparam n_rows number of matrix rows
 * @tparam n_cols number of matrix cols
 * @tparam T first `value_type`
 * @tparam U second `value_type`
 */
template <std::size_t n_rows, std::size_t n_cols, typename T, typename U>
bool operator==(
  const matrix<n_rows, n_cols, T>& a, const matrix<n_rows, n_cols, U>& b)
{
  // compute constexpr epsilon (no runtime cost anyways) for float comparison
  PDDCP_MATRIX_ROW_MAJOR_LOOP(n_rows, n_cols) {
    auto& v_a = a(i, j);
    auto& v_b = b(i, j);
    // only use float comparison for floating types
    if constexpr (std::is_floating_point_v<T> || std::is_floating_point_v<U>) {
      // when types are same
      if constexpr (std::is_same_v<T, U>) {
        constexpr auto eps = std::numeric_limits<T>::epsilon();
        // negation of checking relative tolerance to smaller value
        if (std::fabs(v_a - v_b) > std::min(v_a, v_b) * eps)
          return false;
      }
      // when types differ -- cast everything to double instead
      else {
        constexpr auto eps = std::min(
          static_cast<double>(std::numeric_limits<T>::epsilon()),
          static_cast<double>(std::numeric_limits<U>::epsilon())
        );
        if (
          std::fabs(v_a - v_b) >
          std::min(static_cast<double>(v_a), static_cast<double>(v_b)) * eps
        )
          return false;
      }
    }
    else if (v_a != v_b)
      return false;
  }
  return true;
}

/**
 * Return sum of two non-boolean matrices as a matrix with same shape.
 *
 * @tparam n_rows number of matrix rows
 * @tparam n_cols number of matrix cols
 * @tparam T first `value_type`
 * @tparam U second `value_type`
 * @tparam V returned matrix `value_type`
 *
 * @param a first matrix
 * @param b second matrix
 */
template <
  std::size_t n_rows,
  std::size_t n_cols,
  typename T,
  typename U,
  typename V = double>
auto operator+(
  const matrix<n_rows, n_cols, T>& a, const matrix<n_rows, n_cols, U>& b)
{
  static_assert(!std::is_same_v<T, bool>, "left matrix has bool value_type");
  static_assert(!std::is_same_v<U, bool>, "right matrix has bool value_type");
  matrix<n_rows, n_cols, V> c;
#ifdef _OPENMP
  #pragma omp parallel for collapse(2)
#endif  // _OPENMP
  PDDCP_MATRIX_ROW_MAJOR_LOOP(n_rows, n_cols)
    c(i, j) = a(i, j) + b(i, j);
  return c;
}

/**
 * Return sum of two non-boolean matrices as a matrix with same shape.
 *
 * This overload is selected if matrix `value_type` and scalar type are same.
 *
 * @tparam n_rows number of matrix rows
 * @tparam n_cols number of matrix cols
 * @tparam T input and return matrix `value_type`
 *
 * @param a first matrix
 * @param b second matrix
 */
template <std::size_t n_rows, std::size_t n_cols, typename T>
inline auto operator+(
  const matrix<n_rows, n_cols, T>& a, const matrix<n_rows, n_cols, T>& b)
{
  return operator+<n_rows, n_cols, T, T, T>(a, b);
}

/**
 * Return sum a non-boolean matrix + scalar.
 *
 * @tparam n_rows number of matrix rows
 * @tparam n_cols number of matrix cols
 * @tparam T matrix `value_type`
 * @tparam U scalar type
 * @tparam V returned matrix `value_type`
 *
 * @param mat matrix
 * @param value scalar value
 */
template <
  std::size_t n_rows,
  std::size_t n_cols,
  typename T,
  typename U,
  typename V = double>
auto operator+(const matrix<n_rows, n_cols, T>& mat, U value)
{
  static_assert(!std::is_same_v<T, bool>, "matrix has bool value_type");
  static_assert(!std::is_same_v<U, bool>, "scalar has bool value_type");
  matrix<n_rows, n_cols, V> out;
#ifdef _OPENMP
  #pragma omp parallel for collapse(2)
#endif  // _OPENMP
  PDDCP_MATRIX_ROW_MAJOR_LOOP(n_rows, n_cols)
    out(i, j) = mat(i, j) + value;
  return out;
}

/**
 * Return sum a non-boolean matrix + scalar.
 *
 * This overload is selected if matrix `value_type` and scalar type are same.
 *
 * @tparam n_rows number of matrix rows
 * @tparam n_cols number of matrix cols
 * @tparam T input and return matrix `value_type`
 *
 * @param mat matrix
 * @param value scalar value
 */
template <std::size_t n_rows, std::size_t n_cols, typename T>
inline auto operator+(const matrix<n_rows, n_cols, T>& mat, T value)
{
  return operator+<n_rows, n_cols, T, T, T>(mat, value);
}

/**
 * Return sum of scalar + non-boolean matrix.
 *
 * @tparam n_rows number of matrix rows
 * @tparam n_cols number of matrix cols
 * @tparam T scalar type
 * @tparam U matrix `value_type`
 *
 * @param value scalar value
 * @param mat matrix
 */
template <std::size_t n_rows, std::size_t n_cols, typename T, typename U>
inline auto operator+(T value, const matrix<n_rows, n_cols, U>& mat)
{
  return mat + value;
}

/**
 * Unary negation operator.
 *
 * @tparam n_rows number of matrix rows
 * @tparam n_cols number of matrix cols
 * @tparam T matrix `value_type`
 *
 * @param mat matrix
 */
template <std::size_t n_rows, std::size_t n_cols, typename T>
auto operator-(const matrix<n_rows, n_cols, T>& mat)
{
  static_assert(std::is_signed_v<T>, "T must be a signed type");
  matrix<n_rows, n_cols, T> out;
#ifdef _OPENMP
  #pragma omp parallel for collapse(2)
#endif  // _OPENMP
  PDDCP_MATRIX_ROW_MAJOR_LOOP(n_rows, n_cols)
    out(i, j) = -mat(i, j);
  return out;
}

/**
 * Return difference of two non-boolean matrices as matrix with same shape.
 *
 * @tparam n_rows number of matrix rows
 * @tparam n_cols number of matrix cols
 * @tparam T first `value_type`
 * @tparam U second `value_type`
 * @tparam V returned matrix `value_type`
 *
 * @param a first matrix
 * @param b second matrix
 */
template <
  std::size_t n_rows,
  std::size_t n_cols,
  typename T,
  typename U,
  typename V = double>
auto operator-(
  const matrix<n_rows, n_cols, T>& a, const matrix<n_rows, n_cols, U>& b)
{
  static_assert(!std::is_same_v<T, bool>, "left matrix has bool value_type");
  static_assert(!std::is_same_v<U, bool>, "right matrix has bool value_type");
  matrix<n_rows, n_cols, V> c;
#ifdef _OPENMP
  #pragma omp parallel for collapse(2)
#endif  // _OPENMP
  PDDCP_MATRIX_ROW_MAJOR_LOOP(n_rows, n_cols)
    c(i, j) = a(i, j) - b(i, j);
  return c;
}

/**
 * Return difference of two non-boolean matrices as a matrix with same shape.
 *
 * This overload is selected if matrix `value_type` and scalar type are same.
 *
 * @tparam n_rows number of matrix rows
 * @tparam n_cols number of matrix cols
 * @tparam T input and return matrix `value_type`
 *
 * @param a first matrix
 * @param b second matrix
 */
template <std::size_t n_rows, std::size_t n_cols, typename T>
inline auto operator-(
  const matrix<n_rows, n_cols, T>& a, const matrix<n_rows, n_cols, T>& b)
{
  return operator-<n_rows, n_cols, T, T, T>(a, b);
}

/**
 * Return difference of non-boolean matrix and a scalar.
 *
 * @tparam n_rows number of matrix rows
 * @tparam n_cols number of matrix cols
 * @tparam T first `value_type`
 * @tparam U second `value_type`
 * @tparam V returned matrix `value_type`
 *
 * @param mat matrix
 * @param value scalar value
 */
template <
  std::size_t n_rows,
  std::size_t n_cols,
  typename T,
  typename U,
  typename V = double>
auto operator-(const matrix<n_rows, n_cols, T>& mat, U value)
{
  static_assert(!std::is_same_v<T, bool>, "matrix has bool value_type");
  static_assert(!std::is_same_v<U, bool>, "scalar has bool value_type");
  matrix<n_rows, n_cols, V> out;
#ifdef _OPENMP
  #pragma omp parallel for collapse(2)
#endif  // _OPENMP
  PDDCP_MATRIX_ROW_MAJOR_LOOP(n_rows, n_cols)
    out(i, j) = mat(i, j) - value;
  return out;
}

/**
 * Return difference of non-boolean matrix and a scalar.
 *
 * This overload is selected if matrix `value_type` and scalar type are same.
 *
 * @tparam n_rows number of matrix rows
 * @tparam n_cols number of matrix cols
 * @tparam T input and return matrix `value_type`
 *
 * @param mat  matrix
 * @param value second matrix
 */
template <std::size_t n_rows, std::size_t n_cols, typename T>
inline auto operator-(const matrix<n_rows, n_cols, T>& mat, T value)
{
  return operator-<n_rows, n_cols, T, T, T>(mat, value);
}

/**
 * Return difference of a scalar and a non-boolean matrix.
 *
 * Cannot be implemented in terms of unary `operator-` and binary `operator+`,
 * as otherwise that will fail if the matrix has an unsigned `value_type`.
 *
 * @tparam n_rows number of matrix rows
 * @tparam n_cols number of matrix cols
 * @tparam T first `value_type`
 * @tparam U second `value_type`
 * @tparam V returned matrix `value_type`
 *
 * @param value scalar value
 * @param mat matrix
 */
template <
  std::size_t n_rows,
  std::size_t n_cols,
  typename T,
  typename U,
  typename V = double>
auto operator-(T value, const matrix<n_rows, n_cols, U>& mat)
{
  static_assert(!std::is_same_v<T, bool>, "matrix has bool value_type");
  static_assert(!std::is_same_v<U, bool>, "scalar has bool value_type");
  matrix<n_rows, n_cols, V> out;
#ifdef _OPENMP
  #pragma omp parallel for collapse(2)
#endif  // _OPENMP
  PDDCP_MATRIX_ROW_MAJOR_LOOP(n_rows, n_cols)
    out(i, j) = value - mat(i, j);
  return out;
}

/**
 * Return difference of a scalar and a non-boolean matrix.
 *
 * This overload is selected if matrix `value_type` and scalar type are same.
 *
 * @tparam n_rows number of matrix rows
 * @tparam n_cols number of matrix cols
 * @tparam T input and returned matrix `value_type`
 *
 * @param value scalar value
 * @param mat matrix
 */
template <std::size_t n_rows, std::size_t n_cols, typename T>
inline auto operator-(T value, const matrix<n_rows, n_cols, T>& mat)
{
  return operator-<n_rows, n_cols, T, T, T>(value, mat);
}

/**
 * Elementwise AND operator for boolean matrices.
 *
 * @tparam n_rows number of matrix rows
 * @tparam n_cols number of matrix cols
 *
 * @param a first matrix
 * @param b second matrix
 */
template <std::size_t n_rows, std::size_t n_cols>
auto operator&(
  const matrix<n_rows, n_cols, bool>& a, const matrix<n_rows, n_cols, bool>& b)
{
  matrix<n_rows, n_cols, bool>& c;
#ifdef _OPENMP
  #pragma omp parallel for collapse(2)
#endif  // _OPENMP
  PDDCP_MATRIX_ROW_MAJOR_LOOP(n_rows, n_cols)
    c(i, j) = a(i, j) && b(i, j);
  return c;
}

/**
 * Elementwise OR operator for boolean matrices.
 *
 * @tparam n_rows number of matrix rows
 * @tparam n_cols number of matrix cols
 *
 * @param a first matrix
 * @param b second matrix
 */
template <std::size_t n_rows, std::size_t n_cols>
auto operator|(
  const matrix<n_rows, n_cols, bool>& a, const matrix<n_rows, n_cols, bool>& b)
{
  matrix<n_rows, n_cols, bool>& c;
#ifdef _OPENMP
  #pragma omp parallel for collapse(2)
#endif  // _OPENMP
  PDDCP_MATRIX_ROW_MAJOR_LOOP(n_rows, n_cols)
    c(i, j) = a(i, j) || b(i, j);
  return c;
}

}  // namespace pddcp

#endif  // PDDCP_MATRIX_H_
