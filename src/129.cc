/**
 * @file 129.cc
 * @author Derek Huang
 * @brief #129 from Daily Coding Problem
 * @copyright MIT License
 *
 * Given a real number n, find the square root of n. For example, given n = 9,
 * return 3.
 */

#include <cmath>
#include <complex>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>

#include <gtest/gtest.h>

#include "pddcp/rtti.h"
#include "pddcp/utility.h"
#include "pddcp/warnings.h"

namespace {

/**
 * Return the real square root of a floating point value.
 *
 * Implementation uses Newton's method to find the square root of `op`.
 *
 * @tparam T floating point type
 *
 * @param op Value to take real square root of, must be nonnegative
 * @param tol Root-finding tolerance, defaults to the type's epsilon
 */
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
auto real_sqrt(const T& op, const T& tol = std::numeric_limits<T>::epsilon())
{
  // don't support negative values, use complex_sqrt for that
  if (op < T{})
    throw std::runtime_error{
      PDDCP_PRETTY_FUNCTION_NAME +
      std::string{": cannot take square root of negative number"}
    };
  // use Newton's root-finding method to find the desired root. we basically
  // just want to find the root x_hat s.t. x_hat * x_hat - op = 0. x_hat_prev
  // has its initial value chosen to guarantee it is different from x_hat
  auto x_hat = op;
  auto x_hat_prev = x_hat - 1;
  // iterate until we reach the tolerance or repeat the same guess twice. we
  // use exact equality here because there are cases where the desired
  // tolerance cannot be reached but x_hat is no longer updating
  while (x_hat_prev != x_hat && std::abs(x_hat * x_hat - op) >= tol) {
    x_hat_prev = x_hat;
// MSVC complains about potential loss of data converting from double. this
// happens when T is float, as 0.5 is technically double
PDDCP_MSVC_WARNING_PUSH()
PDDCP_MSVC_WARNING_DISABLE(4244)
    // simplified version of (x_hat * x_hat - op) / (2 * x_hat)
    x_hat -= 0.5 * (x_hat - op / x_hat);
PDDCP_MSVC_WARNING_POP()
  }
  return x_hat;
}

/**
 * Return the real square root of an integral value.
 *
 * Implementation uses Newton's method to find the square root of `op`.
 *
 * @tparam T integral type
 *
 * @param op Value to take real square root of, must be nonnegative
 * @param tol Root-finding tolerance, defaults to the type's epsilon
 */
template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline auto real_sqrt(
  const T& op, double tol = std::numeric_limits<double>::epsilon())
{
  return real_sqrt(static_cast<double>(op), tol);
}

/**
 * Return the complex square root of a floating point value.
 *
 * @tparam T floating point type
 *
 * @param op Value to take complex square root of
 * @param tol Root-finding tolerance, defaults to the type's epsilon
 */
template <typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
inline auto complex_sqrt(
  const T& op, const T& tol = std::numeric_limits<T>::epsilon())
{
  return (op < T{}) ?
    std::complex{T{}, real_sqrt(-op, tol)} : std::complex{real_sqrt(op, tol)};
}

/**
 * Return the complex square root of an integral value.
 *
 * @tparam T integral type
 *
 * @param op Value to take complex square root of
 * @param tol Root-finding tolerance, defaults to the type's epsilon
 */
template <typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline auto complex_sqrt(
  const T& op, double tol = std::numeric_limits<double>::epsilon())
{
  return complex_sqrt(static_cast<double>(op), tol);
}

/**
 * Base test class template.
 *
 * @tparam IndexedType `pddcp::indexed_type<I, T>`, arithmetic `element_type`
 */
template <typename IndexedType>
class DailyTest129 : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_HELPER_TYPES(IndexedType);
};

// input types used in the specializations + TYPED_TEST_SUITE macro
using InputType1 = pddcp::indexed_type<0, unsigned int>;
using InputType2 = pddcp::indexed_type<1, float>;
using InputType3 = pddcp::indexed_type<2, double>;
using InputType4 = pddcp::indexed_type<3, int>;
using InputType5 = pddcp::indexed_type<4, double>;
using InputType6 = pddcp::indexed_type<5, long int>;

/**
 * Helper macro defining a real root `DailyTest129` specialization.
 *
 * Each specialization has `op_` and `res_` members for the base value to take
 * the square root of and the expected real square root value.
 *
 * @param type `pddcp::indexed_type<I, T>` specialization
 * @param op Base value to take square root of
 * @param res Expected real square root of `value`
 */
#define DAILY_TEST_129_R(type, op, res) \
  template <> \
  class DailyTest129<type> : public ::testing::Test { \
  public: \
    using element_type = typename type::element_type; \
  protected: \
    static inline constexpr element_type op_ = op; \
    static inline constexpr element_type res_ = res; \
  }

/**
 * Helper macro defining a complex root `DailyTest129` specialization.
 *
 * Each specialization has `op_` and `res_` members for the base value to take
 * the square root of and the expected complex square root value.
 *
 * @param type `pddcp::indexed_type<I, T>` specialization
 * @param op Base value to take square root of
 * @param res_r Expected real part complex square root of `value`
 * @param res_i Expected imaginary part complex square root of `value`
 */
#define DAILY_TEST_129_C(type, op, res_r, res_i) \
  template <> \
  class DailyTest129<type> : public ::testing::Test { \
  public: \
    using element_type = typename type::element_type; \
  protected: \
    static inline constexpr element_type op_ = op; \
    /* we use double for any non-floating type */ \
    static inline constexpr std::complex< \
      std::conditional_t< \
        std::is_floating_point_v<element_type>, element_type, double \
      > \
    > res_{res_r, res_i}; \
  }

/**
 * Specialization for the given input/output pair.
 *
 * Uses an unsigned operand to test template overload resolution.
 */
DAILY_TEST_129_R(InputType1, 9, 3);

/**
 * Specialization for the first custom input/output pair.
 *
 * MSVC complains about double being truncated to float here.
 */
PDDCP_MSVC_WARNING_PUSH()
PDDCP_MSVC_WARNING_DISABLE(4305)
DAILY_TEST_129_R(InputType2, 104, 10.198039027185569);
PDDCP_MSVC_WARNING_POP()

/**
 * Specialization for the second custom input/output pair.
 *
 * We use a negative operand to test `complex_sqrt`.
 */
DAILY_TEST_129_C(InputType3, -9, 0, 3);

/**
 * Specialization for the third custom input/output pair.
 *
 * We use a positve int operand this time to test `complex_sqrt`.
 */
DAILY_TEST_129_C(InputType4, 100, 10, 0);

/**
 * Specialization for the fourth custom input/output pair.
 *
 * Testing `complex_sqrt` by using a positive input with an irrational sroot.
 */
DAILY_TEST_129_C(InputType5, 1234, 35.12833614050059, 0);

/**
 * Specialization for the fifth custom input/output pair.
 *
 * Testing `complex_sqrt` by using a negative input with an irrational root.
 */
DAILY_TEST_129_C(InputType6, -4141, 0, 64.35060217278468);

using DailyTest129Types = ::testing::Types<
  InputType1, InputType2, InputType3, InputType4, InputType5, InputType6
>;
TYPED_TEST_SUITE(DailyTest129, DailyTest129Types);

/**
 * Test that `real_sqrt` and `complex_sqrt` work as expected.
 *
 * The correct function is selected at compile time based on the result type.
 */
TYPED_TEST(DailyTest129, SqrtTest)
{
  using element_type = typename TestFixture::element_type;
  using result_type = std::decay_t<decltype(TestFixture::res_)>;
  // integral operands for complex_sqrt return std::complex<double>
  if constexpr (
    std::is_same_v<result_type, std::complex<element_type>> ||
    std::is_same_v<result_type, std::complex<double>>
  ) {
    auto res = complex_sqrt(TestFixture::op_);
    EXPECT_DOUBLE_EQ(TestFixture::res_.real(), res.real());
    EXPECT_DOUBLE_EQ(TestFixture::res_.imag(), res.imag());
  }
  else
    EXPECT_DOUBLE_EQ(TestFixture::res_, real_sqrt(TestFixture::op_));
}

}  // namespace
