/**
 * @file 32.cc
 * @author Derek Huang
 * @brief #32 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Jane Street.
 *
 * Suppose you are given a table of currency exchange rates, represented as a
 * 2D array. Determine whether there is a possible arbitrage: that is, whether
 * there is some sequence of trades you can make, starting with some amount A
 * of any currency, so that you can end up with some amount greater than A of
 * that currency. There are no transaction costs and you can trade fractional
 * quantities.
 */

#include <cmath>
#include <limits>
#include <stdexcept>
#include <type_traits>

#include <gtest/gtest.h>

#include "pddcp/matrix.h"
#include "pddcp/utility.h"
#include "pddcp/warnings.h"

namespace {

/**
 * Class denoting a currency triple.
 *
 * Originally, a `std::array<T, 3>` was used, but it was a little annoying to
 * use due to the aggregate initialization rules requiring extra brackets.
 *
 * @tparam T Integral type or other denoting a currency identifier
 */
template <typename T>
class fx_triangle {
public:
  using value_type = T;
  using size_type = std::size_t;

  /**
   * Ctor.
   *
   * @param first First currency identifier
   * @param second Second currency identifier
   * @param third Third currency identifier
   */
  constexpr fx_triangle(const T& first, const T& second, const T& third)
    : first_{first}, second_{second}, third_{third}
  {}

  constexpr auto& first() const { return first_; }
  constexpr auto& second() const { return second_; }
  constexpr auto& third() const { return third_; }

  constexpr bool operator==(const fx_triangle& other) const
  {
    return first_ == other.first() &&
      second_ == other.second() &&
      third_ == other.third();
  }

private:
  T first_;
  T second_;
  T third_;
};

/**
 * Type alias for vector holding currency triples.
 *
 * @tparam T Integral type or other denoting a currency identifier
 */
template <typename T>
using fx_triangle_vector = std::vector<fx_triangle<T>>;

/**
 * Return a vector of currency triangles that contain arbitrage.
 *
 * Cubic time complexity algorithm that checks for all currency triangles that
 * can be arbitraged. That is, if `r(i, j)` denotes the exchange rate between
 * currencies `i`, `j` such that itis the fractional quantity of `i` in units
 * of `j`, we check if the condition `r(i, j) * r(j, k) * r(k, i) == 1` is
 * violated, which would lead to a currencfy arbitrage. It is assumed there are
 * no transaction costs and fractional quantities can be traded.
 *
 * If there are no currency triangles to arbitrage, vector is empty.
 *
 * @tparam Matrix `pddcp::matrix_base<Matrix>` subclass with floating point
 *  `value_type` and square dimensions, i.e. `row_count` equals `col_count`
 *
 * @param fx_rates Square matrix of currency exchange rates, i.e.
 *  `fx_rates[i, j]` denotes the fractional quantity of `i` in units of `j`
 */
template <typename Matrix>
auto triangular_arbitrage(const pddcp::matrix_base<Matrix>& fx_rates)
{
  // convenience type aliases + necessary check that rates are float values
  using size_type = typename Matrix::size_type;
  using value_type = typename Matrix::value_type;
  static_assert(
    std::is_floating_point_v<value_type>,
    "fx_rates matrix must have floating value_type"
  );
  // matrix must be a square matrix
  static_assert(
    Matrix::row_count == Matrix::col_count, "fx_rates matrix must be square"
  );
  // we look for triangular arbitrage. letting r(i, j) be the exchange rate
  // between currencies i, j such that given N(i), a quantity of currency i, we
  // have the equation N(i) = r(i, j) * N(j). in other words, r(i, j) is the
  // fractional number of units of i per unit of j. therefore, given currencies
  // i, j, and k, the following equations must be true:
  //
  // N(i) = r(i, j) * N(j)
  // N(j) = r(j, k) * N(k)
  // N(k) = r(k, i) * N(i)
  //
  // substituting and dividing by N(i), we see that
  //
  // 1 = r(i, j) * r(j, k) * r(k, i)
  //
  // if we have currencies i, j, and k such that this equality does not hold,
  // then we have discovered a triangular arbitrage.
  constexpr auto n_ccys = Matrix::row_count;
  fx_triangle_vector<size_type> arbs;
  // note that we don't need to reverse the currencies; the arbitrage
  // opportunity is symmetric with regards to starting currency
  for (size_type i = 0; i < n_ccys; i++)
    for (size_type j = i + 1; j < n_ccys; j++)
      for (size_type k = j + 1; k < n_ccys; k++)
        // need epsilon due to imprecise nature of flops
        if (
          std::abs(1 - fx_rates(i, j) * fx_rates(j, k) * fx_rates(k, i)) >
          std::numeric_limits<value_type>::epsilon()
        )
          arbs.emplace_back(typename decltype(arbs)::value_type{i, j, k});
  return arbs;
}

/**
 * Base test class template.
 *
 * @tparam IndexedType `pddcp::indexed_type<I, T>` with matrix `element_type`
 */
template <typename IndexedType>
class DailyTest32 : public ::testing::Test {};

using InputType1 = pddcp::indexed_type<0, pddcp::dense_matrix<3, 3, float>>;
using InputType2 = pddcp::indexed_type<1, pddcp::dense_matrix<3, 3, float>>;

/**
 * Specialization for the first input/output pair.
 *
 * No arbitrage, i.e. `r(i, j) * r(j, k) * r(k, i) == 1` for all `i != j != k`.
 */
template <>
class DailyTest32<InputType1> : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_CONTAINER_HELPER_TYPES(InputType1);
protected:
// MSVC complains about truncation from double to float
PDDCP_MSVC_WARNING_PUSH()
PDDCP_MSVC_WARNING_DISABLE(4305)
  static inline const element_type fx_rates_{
    {1., 0.8, 0.4},
    {1.25, 1., 0.5},
    {2.5, 2., 1.}
  };
PDDCP_MSVC_WARNING_POP()
  static inline const fx_triangle_vector<size_type> arbs_{};
};

/**
 * Specialization for the second input/output pair.
 *
 * Here there is a triangular arbitrage opportunity, i.e.
 * `r(0, 1) * r(1, 2) * r(2, 0) == 2 != 1`.
 */
template <>
class DailyTest32<InputType2> : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_CONTAINER_HELPER_TYPES(InputType2);
protected:
// MSVC complains about truncation from double to float
PDDCP_MSVC_WARNING_PUSH()
PDDCP_MSVC_WARNING_DISABLE(4305)
  static inline const element_type fx_rates_{
    {1., 0.8, 0.2},
    {1.25, 1., 0.5},
    {5., 2., 1.}
  };
PDDCP_MSVC_WARNING_POP()
  static inline const fx_triangle_vector<size_type> arbs_{{0, 1, 2}};
};

using DailyTest32Types = ::testing::Types<InputType1, InputType2>;
TYPED_TEST_SUITE(DailyTest32, DailyTest32Types);

/**
 * Test that `triangular_arbitrage` works as expected.
 */
TYPED_TEST(DailyTest32, TypedTest)
{
  EXPECT_EQ(TestFixture::arbs_, triangular_arbitrage(TestFixture::fx_rates_));
}

}  // namespace
