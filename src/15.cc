/**
 * @file 15.cc
 * @author Derek Huang
 * @brief #15 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Google.
 *
 * The area of a circle is defined as pi * r^2. Estimate pi to 3 decimal places
 * using a Monte Carlo method. Hint: The basic equation of a circle is
 * x2 + y2 = r2.
 */

#include <cmath>
#include <cstdint>

// C++20 header we could use for a pi constant
#if __cplusplus >= 202002L
#include <numbers>
#endif  // __cplusplus < 202002L

#include <type_traits>
#include <random>

#include <gtest/gtest.h>

namespace {

/**
 * Estimate pi using Monte Carlo.
 *
 * Uses the classic "circle-filling" method where we count the number of points
 * sampled in [-1, 1] x [-1, 1] that fall in the unit circle, divide that by
 * the number of points sampled, and multiply by 4 to estimate pi.
 *
 * @tparam UintType unsigned integral type
 * @tparam Rng *UniformRandomBitGenerator*
 *
 * @param n_samples Number of random samples to draw
 * @param rng *UniformRandomBitGenerator* PRNG instance
 */
template <typename UintType, typename Rng>
double mcpi(UintType n_samples, Rng rng)
{
  static_assert(std::is_integral_v<UintType>);
  std::uniform_real_distribution udist{-1., 1.};
  double x, y;
  UintType n_inside = 0;
  for (UintType i = 0; i < n_samples; i++) {
    x = udist(rng);
    y = udist(rng);
    if (x * x + y * y <= 1) n_inside++;
  }
  return 4. * (static_cast<double>(n_inside) / n_samples);
}

/**
 * Estimate pi using Monte Carlo.
 *
 * Uses the 32-bit Mersenne Twister as the PRNG.
 *
 * @tparam UintType unsigned integral type
 *
 * @param n_samples Number of random samples to draw
 * @param seed Seed for the 32-bit Mersenne Twister, if not specified then
 *  `std::random_device{}()` is used to generate a seed.
 */
template <typename UintType>
inline double mcpi(
  UintType n_samples, std::uint_fast32_t seed = std::random_device{}())
{
  return mcpi(n_samples, std::mt19937{seed});
}

/*
 * Test fixture template class for testing.
 *
 * Just to check that the `mcpi` function template works as expected.
 */
template <typename UintType>
class DailyTest15 : public ::testing::Test {
protected:
// if we use C++20, we can use the exact value of pi
#if __cplusplus >= 202002L
  static constexpr double pi_ = std::numbers::pi;
#else
  static inline const double pi_{4 * std::atan(1.)};
#endif  // __cplusplus < 202002L
  // ballpark minimum of samples needed to get within current tol_
  static inline const UintType n_samples_ = 200000;
  // up to 3 decimals places -> 1e-3 tolerance
  static constexpr double tol_ = 1e-3;
  static inline const std::uint_fast32_t seed_{88};
};

using DailyTestTypes15 = ::testing::Types<unsigned int, std::uint_fast64_t>;
TYPED_TEST_SUITE(DailyTest15, DailyTestTypes15);

/**
 * Test that `mcpi` estimates pi within `tol_`.
 */
TYPED_TEST(DailyTest15, TypedTest)
{
  EXPECT_NEAR(
    TestFixture::pi_,
    mcpi(TestFixture::n_samples_, TestFixture::seed_),
    TestFixture::tol_
  );
}

}  // namespace
