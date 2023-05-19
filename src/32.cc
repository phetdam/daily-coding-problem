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

#include <array>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <type_traits>

#include <gtest/gtest.h>

#include "pddcp/matrix.h"
#include "pddcp/utility.h"
#include "pddcp/warnings.h"

namespace {

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
  std::vector<std::array<size_type, 3>> arbs;
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
        // MSVC correctly warns that aggregate init should have extra braces
          arbs.emplace_back(typename decltype(arbs)::value_type{{i, j, k}});
  return arbs;
}

template <typename IndexedType>
class DailyTest32 : public ::testing::Test {};

using InputType1 = pddcp::indexed_type<0, pddcp::dense_matrix<3, 3, float>>;
using InputType2 = pddcp::indexed_type<1, pddcp::dense_matrix<3, 3, float>>;

/**
 * No arbitrage
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
  static inline const std::vector<std::array<size_type, 3>> arbs_{};
};

/**
 * Arbitrage
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
// MSVC correctly warns about extra braces needed for aggregate init
  static inline const std::vector<std::array<size_type, 3>> arbs_{{{0, 1, 2}}};
};

using DailyTest32Types = ::testing::Types<InputType1, InputType2>;
TYPED_TEST_SUITE(DailyTest32, DailyTest32Types);

TYPED_TEST(DailyTest32, TypedTest)
{
  EXPECT_EQ(TestFixture::arbs_, triangular_arbitrage(TestFixture::fx_rates_));
}

}  // namespace
