/**
 * @file 128.cc
 * @author Derek Huang
 * @brief #128 from Daily Coding Problem
 *
 * The Tower of Hanoi is a puzzle game with three rods and n disks, each a
 * different size. All the disks start off on the first rod in a stack. They
 * are ordered by size, with the largest disk on the bottom and the smallest
 * one at the top. The goal of this puzzle is to move all the disks from the
 * first rod to the last rod while following these rules:
 *
 * 1. You can only move one disk at a time.
 * 2. A move consists of taking the uppermost disk from one of the stacks and
 *    placing it on top of another stack.
 * 3. You cannot place a larger disk on top of a smaller disk.
 *
 * Write a function that prints out all the steps necessary to complete the
 * Tower of Hanoi. You should assume that the rods are numbered, with the first
 * rod being 1, the second (auxiliary) rod being 2, and the last (goal) rod
 * being 3.
 *
 * For example, with n = 3, we can do this in 7 moves:
 *
 * Move 1 to 3
 * Move 1 to 2
 * Move 3 to 2
 * Move 1 to 3
 * Move 2 to 1
 * Move 2 to 3
 * Move 1 to 3
 */

// MSVC reports __STDC_WANT_SECURE_LIB__ not defined in limits.h, which it
// replaces with an #if 0 for the conditional, so we manually suppress this
#if _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4668)
#endif  // _MSC_VER

#include <cstdint>
#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#ifdef _MSC_VER
#pragma warning (pop)
#endif  // _MSC_VER

namespace {

/**
 * Enum class for the Tower of Hanoi rods.
 */
enum class hanoi_rod : unsigned int { START = 1, AUX, GOAL };

/**
 * `operator<<` overload for `std::ostream` to implicitly convert `hanoi_rod`.
 *
 * @param e `hanoi_rod` enum value
 */
std::ostream& operator<<(std::ostream& stream, hanoi_rod e)
{
  stream << static_cast<std::underlying_type_t<hanoi_rod>>(e);
  return stream;
}

/**
 * Plays through the Tower of Hanoi game using a recursive solution.
 *
 * The idea is that for an n-disk problem, we first move the n - 1 disks on top
 * to the auxiliary rod, move the nth (the largest) disk to the goal rod, and
 * then move the n - 1 disks from the auxiliary to the goal rod.
 *
 * @tparam UIntType unsigned integral type
 *
 * @param n_disks number of disks
 * @param start starting rod
 * @param aux auxiliary rod
 * @param goal goal rod
 * @param stream stream to print moves to, if not specified is `std::cout`
 *
 * @returns number of moves used, should be 2^n - 1.
 */
template <typename UIntType>
std::size_t tower_of_hanoi(
  UIntType n_disks,
  hanoi_rod start,
  hanoi_rod aux,
  hanoi_rod goal,
  std::ostream& stream = std::cout)
{
  static_assert(std::is_unsigned_v<UIntType>);
  if (!n_disks)
    return 0;
  if (n_disks == 1) {
    stream << "Move " << start << " to " << goal << "\n";
    return 1;
  }
  auto n_moves = tower_of_hanoi(n_disks - 1, start, goal, aux, stream);
  stream << "Move " << start << " to " << goal << "\n";
  return 1 + n_moves + tower_of_hanoi(n_disks - 1, aux, start, goal, stream);
}

/**
 * Plays through the Tower of Hanoi game using a recursive solution.
 *
 * @tparam UIntType unsigned integral type
 *
 * @param n_disks number of disks
 * @param stream stream to print moves to, if not specified is `std::cout`
 *
 * @returns number of moves used, should be 2^n - 1.
 */
template <typename UIntType>
inline std::size_t tower_of_hanoi(
  UIntType n_disks, std::ostream& stream = std::cout)
{
  return tower_of_hanoi(
    n_disks, hanoi_rod::START, hanoi_rod::AUX, hanoi_rod::GOAL, stream
  );
}

// convenience type aliases
using result_type = std::pair<std::size_t, std::string>;
using input_type = unsigned int;
using pair_type = std::pair<input_type, result_type>;

/**
 * Test fixture class for parametrized testing.
 */
// MSVC complains about move, copy ctors + operator= implicitly being deleted
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4625 4626 5026 5027)
#endif  // _MSC_VER
class DailyTest128 : public ::testing::TestWithParam<pair_type> {};
#ifdef _MSC_VER
#pragma warning (pop)
#endif  // _MSC_VER

INSTANTIATE_TEST_SUITE_P(
  SamplePairs,
  DailyTest128,
  ::testing::Values(
    pair_type{
      3,
      {
        7,
        "Move 1 to 3\nMove 1 to 2\nMove 3 to 2\nMove 1 to 3\nMove 2 to 1\n"
        "Move 2 to 3\nMove 1 to 3\n"
      }
    }
  )
);

// for larger numbers of disks, we add extra cases without output comparison.
// we need to duplicate the GoogleTest macro call, otherwise MSVC errors out.
// GCC allows us to insert the preprocessor directives in the middle of the
// ::testing::Values call, but MSVC does not like that at all.
#ifdef PDDCP_GTEST_STANDALONE
INSTANTIATE_TEST_SUITE_P(
  CustomPairs,
  DailyTest128,
  ::testing::Values(
    pair_type{1, {1, "Move 1 to 3\n"}},
    pair_type{2, {3, "Move 1 to 2\nMove 1 to 3\nMove 2 to 3\n"}},
    pair_type{7, {127, ""}},
    pair_type{11, {2047, ""}}
  )
);
#else
INSTANTIATE_TEST_SUITE_P(
  CustomPairs,
  DailyTest128,
  ::testing::Values(
    pair_type{1, {1, "Move 1 to 3\n"}},
    pair_type{2, {3, "Move 1 to 2\nMove 1 to 3\nMove 2 to 3\n"}}
  )
);
#endif  // PDDCP_GTEST_STANDALONE

/**
 * Test that `tower_of_hanoi` works correctly.
 */
TEST_P(DailyTest128, ParamTest)
{
  const auto& [expected_n_moves, expected_output] = GetParam().second;
  std::stringstream stream;
  EXPECT_EQ(expected_n_moves, tower_of_hanoi(GetParam().first, stream));
// don't perform output comparisons if n_disks > 3
#ifdef PDDCP_GTEST_STANDALONE
  if (GetParam().first <= 3) {
#endif  // PDDCP_GTEST_STANDALONE
  EXPECT_EQ(expected_output, stream.str());
// need braces to avoid warning about ambiguous else
#ifdef PDDCP_GTEST_STANDALONE
  }
#endif  // PDDCP_GTEST_STANDALONE
}

}  // namespace
