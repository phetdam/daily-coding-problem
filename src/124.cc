/**
 * @file 124.cc
 * @author Derek Huang
 * @brief #124 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Microsoft.
 *
 * You have n fair coins and you flip them all at the same time. Any that come
 * up tails you set aside. The ones that come up heads you flip again. How many
 * rounds do you expect to play before only one coin remains?
 *
 * Write a function that, given n, returns the number of rounds you'd expect to
 * play until one coin remains.
 */

#include <cmath>
#include <cstdint>
#include <type_traits>
#include <utility>

// only needed by the simulation functions in standalone runner
#ifdef PDDCP_GTEST_STANDALONE
#include <algorithm>
#include <future>
#include <optional>
#include <random>
#include <stdexcept>
#include <thread>
#include <vector>
#endif  // PDDCP_GTEST_STANDALONE

#include <gtest/gtest.h>

namespace {

/**
 * Return expected number of coin flip rounds.
 *
 * Given n fair coins, we expect half of them to land tails each round. If k is
 * such that 2 ^ k = n, k is the expected number of rounds we'd play until only
 * one coin remains. For n = 0, we obviously use n = 1 instead.
 *
 * @tparam UIntType unsigned integral type
 *
 * @param n Number of rounds
 */
template <typename UIntType>
inline auto expected_rounds(UIntType n)
{
  static_assert(std::is_unsigned_v<UIntType>);
  return std::log2(!n ? n + 1 : n);
}

#ifdef PDDCP_GTEST_STANDALONE
/**
 * Simulate flipping `n` coins, removing ones landing tails, until 1 is left.
 *
 * The coins are all fair coins.
 *
 * @tparam UIntType unsigned integral type
 * @tparam PRng *UniformRandomBitGenerator*
 *
 * @param n Number of rounds
 * @param Rng PRNG instance
 * @returns Number of rounds simulated before 0 or 1 coin is left
 */
template <typename UIntType, typename Rng>
auto simulate_rounds(UIntType n, Rng rng)
{
  static_assert(std::is_unsigned_v<UIntType>);
  // [0, 1) uniform dist, number of founds, remaining coins
  std::uniform_real_distribution udist;
  decltype(n) n_rounds = 0, n_rem = n;
  // until we have 0 or 1 remaining coin, count number of tails, subtract from
  // remaining coins, and increment the number of rounds
  while (n_rem > 1) {
    decltype(n_rem) n_tails = 0;
    for (decltype(n_rem) i = 0; i < n_rem; i++) {
      // < instead of <= since the range is [0, 1) (doesn't really matter)
      if (udist(rng) < 0.5)
        n_tails++;
    }
    n_rem -= n_tails;
    n_rounds++;
  }
  return n_rounds;
}

/**
 * Return Monte Carlo estimate of number of coin flip rounds.
 *
 * Serial implementation that calls `simulate_rounds` sequentially.
 *
 * @tparam UIntType unsigned integral type
 * @tparam PRng *UniformRandomBitGenerator* constructible from its `result_type`
 *
 * @param n Number of rounds
 * @param Rng PRNG instance
 * @param n_trials Number of simulation trials
 */
template <typename UIntType, typename Rng>
auto sim_expected_rounds_serial(UIntType n, Rng rng, UIntType n_trials)
{
  static_assert(std::is_unsigned_v<UIntType>);
  static_assert(std::is_constructible_v<Rng, typename Rng::result_type>);
  if (n_trials < 1)
    throw std::runtime_error("n_trials must be positive");
  // expected number of rounds estimated using MC
  // double avg_rounds = simulate_rounds(n, Rng{rng()});
  double avg_rounds = 0;
  // update for each subsequent trial using average updating formula
  for (UIntType i = 0; i < n_trials; i++)
    avg_rounds += simulate_rounds(n, Rng{rng()});
  return avg_rounds / n_trials;
}

/**
 * Return Monte Carlo estimate of number of coin flip rounds.
 *
 * Parallel implementation calls `simulate_expected_rounds_serial` using
 * `std::async`. If only one job requested, calls serial implementation.
 *
 * @tparam UIntType unsigned integral type
 * @tparam PRng *UniformRandomBitGenerator* constructible from its `result_type`
 *
 * @param n Number of rounds
 * @param Rng PRNG instance
 * @param n_trials Number of simulation trials
 * @param n_jobs Number of jobs to split trials over
 */
template <typename UIntType, typename Rng>
auto sim_expected_rounds(
  UIntType n, Rng rng, UIntType n_trials, UIntType n_jobs)
{
  static_assert(std::is_unsigned_v<UIntType>);
  static_assert(std::is_constructible_v<Rng, typename Rng::result_type>);
  if (n_trials < 1)
    throw std::runtime_error("n_trials must be positive");
  if (n_jobs < 1)
    throw std::runtime_error("n_jobs must be positive");
  // special case for n_jobs = 1
  if (n_jobs == 1)
    return sim_expected_rounds_serial(n, rng, n_trials);
  // seeds for each async task
  std::vector<UIntType> seeds(n_jobs);
  std::for_each(seeds.begin(), seeds.end(), [&](auto& seed) { seed = rng(); });
  // number of trials per job, truncating result and evenly distributed rest
  std::vector<UIntType> trial_counts(n_jobs, n_trials / n_jobs);
  for (UIntType i = 0; i < n_trials % n_jobs; trial_counts[i++]++);
  // futures for each async call + populate
  std::vector<std::future<double>> futures(n_jobs);
  for (UIntType i = 0; i < n_jobs; i++) {
    futures[i] = std::async(
      std::launch::async,
      sim_expected_rounds_serial<UIntType, Rng>,
      n,
      Rng{seeds[i]},
      trial_counts[i]
    );
  }
  // get expected number of rounds estimated using MC + return. note that get()
  // method of future is not const, so must use non-const iter + auto&
  double avg_rounds = 0;
  std::for_each(
    futures.begin(),
    futures.end(),
    // average the computed averages
    [&](auto& future) { avg_rounds += future.get() / n_jobs; }
  );
  return avg_rounds;
}

/**
 * Return Monte Carlo estimate of number of coin flip rounds.
 *
 * Uses the 32-bit Mersenne Twister as the chosen PRNG and the value of
 * `std::thread::hardware_concurrency()` to determine `n_jobs`.
 *
 * @tparam UIntType unsigned integral type
 *
 * @param n Number of rounds
 * @param seed Seed for the 32-bit Mersenne Twister
 * @param n_trials Number of simulation trials
 */
template <typename UIntType>
inline auto sim_expected_rounds(
  UIntType n, std::uint_fast32_t seed, UIntType n_trials)
{
  return sim_expected_rounds(
    n, std::mt19937{seed}, n_trials, std::thread::hardware_concurrency()
  );
}

/**
 * Return Monte Carlo estimate of number of coin flip rounds.
 *
 * Uses the 32-bit Mersenne Twister as the chosen PRNG with a random seed and
 * uses `std::thread::hardware_concurrency()` to determine `n_jobs`.
 *
 * @tparam UIntType unsigned integral type
 *
 * @param n Number of rounds
 * @param n_trials Number of simulation trials
 */
template <typename UIntType>
inline auto sim_expected_rounds(UIntType n, UIntType n_trials)
{
  return sim_expected_rounds(n, std::random_device{}(), n_trials);
}
#endif  // PDDCP_GTEST_STANDALONE

// convenience type aliases
using result_type = double;
using input_type = unsigned int;
using pair_type = std::pair<input_type, result_type>;

/**
 * Test fixture class for parametrized testing.
 */
class DailyTest124 : public ::testing::TestWithParam<pair_type> {
// members only needed for simulation test in standalone test runner
#ifdef PDDCP_GTEST_STANDALONE
protected:
  // seed for the Mersenne Twister, number of trials, simulation tolerance
  static inline constexpr std::uint_fast32_t seed_ = 888;
  static inline constexpr input_type n_trials_ = 1000000;
  // simulation tolerance needs to be a bit higher
  static inline constexpr double tol_ = 0.35;
#endif  // PDDCP_GTEST_STANDALONE
};

INSTANTIATE_TEST_SUITE_P(
  CustomPairs,
  DailyTest124,
  ::testing::Values(
    // edge cases should both evaluate to zero
    pair_type{0, 0.},
    pair_type{1, 0.},
    // even cases
    pair_type{2, 1.},
    pair_type{6, 2.584962500721156},
    pair_type{8, 3.},
    // odd cases
    pair_type{3, 1.584962500721156},
    pair_type{5, 2.321928094887362},
    pair_type{7, 2.807354922057604}
  )
);

/**
 * Test that `expected_rounds` works as expected.
 */
TEST_P(DailyTest124, FormulaParamTest)
{
  EXPECT_DOUBLE_EQ(GetParam().second, expected_rounds(GetParam().first));
}

#ifdef PDDCP_GTEST_STANDALONE
/**
 * Test that `expected_rounds` is close enough to the expected result.
 */
TEST_P(DailyTest124, SimParamTest)
{
  EXPECT_NEAR(
    GetParam().second,
    sim_expected_rounds(GetParam().first, seed_, n_trials_),
    tol_
  );
}
#endif  // PDDCP_GTEST_STANDALONE

}  // namespace
