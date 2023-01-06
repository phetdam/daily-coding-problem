/**
 * @file matrix.h
 * @author Derek Huang
 * @brief C++ flat 2D matrix implementation
 * @copyright MIT License
 */

#ifndef PDDCP_MATRIX_H_
#define PDDCP_MATRIX_H_

#include <cassert>
#include <cstdint>
#include <initializer_list>
#include <type_traits>
#include <vector>

namespace pddcp {

/**
 * Simple fixed-size 2D matrix implementation using flat data storage.
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

  /**
   * Matrix-matrix addition operator.
   *
   * Returned matrix has `T` for `value_type`.
   */
  auto operator+(const matrix<row_count, col_count, T>& other) const
  {
    static_assert(!std::is_same_v<T, bool>, "do not use + for bool matrices");
    matrix<row_count, col_count, T> res;
// use OpenMP if possible
#ifdef _OPENMP
    #pragma omp parallel for collapse(2)
#endif  // _OPENMP
    for (size_type i = 0; i < row_count; i++)
      for (size_type j = 0; j < col_count; j++)
        res(i, j) = this->at(i, j) + other(i, j);
    return res;
  }

  /**
   * Matrix-scalar addition operator.
   *
   * Returned matrix has `T` for `value_type`.
   */
  auto operator+(T value) const
  {
    matrix<row_count, col_count, T> res;
#ifdef _OPENMP
    #pragma omp parallel for collapse(2)
#endif  // _OPENMP
    for (size_type i = 0; i < row_count; i++)
      for (size_type j = 0; j < col_count; j++)
        res(i, j) = this->at(i, j) + value;
    return res;
  }

  /**
   * Matrix-matrix subtraction operator.
   *
   * Returned matrix has `T` for `value_type`.
   */
  auto operator-(const matrix<row_count, col_count, T>& other) const
  {
    static_assert(!std::is_same_v<T, bool>, "do not use - for bool matrices");
    matrix<row_count, col_count, T> res;
#ifdef _OPENMP
    #pragma omp parallel for collapse(2)
#endif  // _OPENMP
    for (size_type i = 0; i < row_count; i++)
      for (size_type j = 0; j < col_count; j++)
        res(i, j) = this->at(i, j) - other(i, j);
    return res;
  }

  /**
   * Matrix-scalar subtraction operator.
   *
   * Returned matrix has `T` for `value_type`.
   */
  auto operator-(T value) const
  {
    matrix<row_count, col_count, T> res;
#ifdef _OPENMP
    #pragma omp parallel for collapse(2)
#endif  // _OPENMP
    for (size_type i = 0; i < row_count; i++)
      for (size_type j = 0; j < col_count; j++)
        res(i, j) = this->at(i, j) - value;
    return res;
  }

  /**
   * Matrix-matrix elementwise and operator for bools.
   */

private:
   std::vector<T> values_;
};

/**
 * Return sum of two non-boolean matrices as a `double` matrix.
 *
 * @tparam n_rows number of matrix rows
 * @tparam n_cols number of matrix cols
 * @tparam T first `value_type`
 * @tparam U second `value_type`
 *
 * @param a first matrix
 * @param b second matrix
 */
template <std::size_t n_rows, std::size_t n_cols, typename T, typename U>
auto operator+(
  const matrix<n_rows, n_cols, T>& a, const matrix<n_rows, n_cols, U>& b)
{
  static_assert(!std::is_same_v<T, bool>, "left matrix has bool value_type");
  static_assert(!std::is_same_v<U, bool>, "right matrix has bool value_type");
  matrix<n_rows, n_cols, double> c;
#ifdef _OPENMP
  #pragma omp parallel for collapse(2)
#endif  // _OPENMP
  for (std::size_t i = 0; i < n_rows; i++)
    for (std::size_t j = 0; j < n_cols; j++)
      c(i, j) = a(i, j) + b(i, j);
  return c;
}

/**
 * Return sum a non-boolean matrix + scalar as a `double` matrix.
 *
 * @tparam n_rows number of matrix rows
 * @tparam n_cols number of matrix cols
 * @tparam T matrix `value_type`
 * @tparam U scalar type
 *
 * @param mat matrix
 * @param value scalar value
 */
template <std::size_t n_rows, std::size_t n_cols, typename T, typename U>
auto operator+(const matrix<n_rows, n_cols, T>& mat, U value)
{
  static_assert(!std::is_same_v<T, bool>, "matrix has bool value_type");
  static_assert(!std::is_same_v<U, bool>, "scalar has bool value_type");
  matrix<n_rows, n_cols, double> out;
#ifdef _OPENMP
  #pragma omp parallel for collapse(2)
#endif  // _OPENMP
  for (std::size_t i = 0; i < n_rows; i++)
    for (std::size_t j = 0; j < n_cols; j++)
      out(i, j) = mat(i, j) + value;
  return out;
}

/**
 * Return sum of non-boolean scalar + matrix as a `double` matrix.
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
inline auto operator+(T value, matrix<n_rows, n_cols, U>& mat)
{
  return mat + value;
}

/**
 * Return difference of two non-boolean matrices as a `double` matrix.
 *
 * @tparam n_rows number of matrix rows
 * @tparam n_cols number of matrix cols
 * @tparam T first `value_type`
 * @tparam U second `value_type`
 *
 * @param a first matrix
 * @param b second matrix
 */
template <std::size_t n_rows, std::size_t n_cols, typename T, typename U>
auto operator-(
  const matrix<n_rows, n_cols, T>& a, const matrix<n_rows, n_cols, U>& b)
{
  static_assert(!std::is_same_v<T, bool>, "left matrix has bool value_type");
  static_assert(!std::is_same_v<U, bool>, "right matrix has bool value_type");
  matrix<n_rows, n_cols, double> c;
// use OpenMP if possible
#ifdef _OPENMP
  #pragma omp parallel for collapse(2)
#endif  // _OPENMP
  for (std::size_t i = 0; i < n_rows; i++)
    for (std::size_t j = 0; j < n_cols; j++)
      c(i, j) = a(i, j) - b(i, j);
  return c;
}

}  // namespace pddcp

#endif  // PDDCP_MATRIX_H_
