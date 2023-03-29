/**
 * @file 109.cc
 * @author Derek Huang
 * @brief #109 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Cisco.
 *
 * Given an unsigned 8-bit integer, swap its even and odd bits. The 1st and 2nd
 * bit should be swapped, the 3rd and 4th bit should be swapped, and so on.
 *
 * For example, 10101010 should be 01010101. 11100010 should be 11010001.
 *
 * Bonus: Can you do this in one line?
 */

#include <cstdint>
#include <utility>

#include <gtest/gtest.h>

#include "pddcp/utility.h"
#include "pddcp/warnings.h"

namespace {

/**
 * Return a copy of an 8-bit unsigned value with its even and odd bits swapped.
 *
 * The mask is in hex for brevity, as 0x5 is 0b0101, 0xA is 0b1010. This is the
 * original inspiration for `pddcp::swap_adjacent_bits`, which generalizes this
 * approach to any integral type for implementations with even-bit bytes. It
 * was clear that overloading was not the answer, as the number of bits in a
 * byte and bytes in a type is implementation dependent.
 *
 * @param value Value whose even and odd bits are to be swapped
 */
auto swap_adjacent_bits(std::uint8_t value)
{
  return ((value & 0x55) << 1) | ((value & 0xAA) >> 1);
}

// convenience type aliases
using input_type = std::uint8_t;
using result_type = input_type;
using pair_type = std::pair<input_type, result_type>;

class DailyTest109 : public ::testing::TestWithParam<pair_type> {};

// MSVC complains that the hex value conversion may result in loss of data
PDDCP_MSVC_WARNING_PUSH()
PDDCP_MSVC_WARNING_DISABLE(4242)
INSTANTIATE_TEST_SUITE_P(
  SamplePairs,
  DailyTest109,
  ::testing::Values(pair_type{0xAA, 0x55}, pair_type{0xE2, 0xD1})
);
PDDCP_MSVC_WARNING_POP()

INSTANTIATE_TEST_SUITE_P(
  CustomPairs,
  DailyTest109,
  ::testing::Values(pair_type{0x82, 0x41}, pair_type{0x1B, 0x27})
);

/**
 * Test that `swap_adjacent_bits` works as expected.
 */
TEST_P(DailyTest109, ParamTest)
{
  EXPECT_EQ(GetParam().second, swap_adjacent_bits(GetParam().first));
}

/**
 * Test that `pddcp::swap_adjacent_bits` also works as expected.
 */
TEST_P(DailyTest109, GenericParamTest)
{
  EXPECT_EQ(GetParam().second, pddcp::swap_adjacent_bits(GetParam().first));
}

}  // namespace
