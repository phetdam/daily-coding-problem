/**
 * @file 7.cc
 * @author Derek Huang
 * @brief #7 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Facebook.
 *
 * Given the mapping a = 1, b = 2, ... z = 26, and an encoded message, count
 * the number of ways it can be decoded. For example, the message '111' would
 * give 3, since it could be decoded as 'aaa', 'ka', and 'ak'. You can assume
 * that the messages are decodable. For example, '001' is not allowed.
 */

#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include <gtest/gtest.h>

namespace {

// just to indicate that this API shouldn't be used directly
namespace detail {

/**
 * Determine the number of possible decodings for a size `length` message.
 *
 * A naive O(2^N) recursive implementation. For any length `n` message, it
 * should have the same decoding possibilities as a length `n - 1` message, as
 * decoding ambiguities only occur for groups of two characters 10-26. Thus, if
 * the last 2 chars of the length `n` message can be interpreted as 10-26, then
 * the decoding possibilities is the sum of the `n - 1` possibilities with the
 * number of decoding possibilities from the `n - 2` message.
 *
 * @tparam CharContainer *Container* with `char` as the `value_type`
 * @tparam UIntType unsigned integral type
 *
 * @param message `char` *Container* message to decode
 * @param length message length, must be <= `message.size()` (unchecked)
 */
template <typename CharContainer, typename UIntType>
UIntType possible_decodings_r(const CharContainer& message, UIntType length)
{
  static_assert(std::is_same_v<typename CharContainer::value_type, char>);
  static_assert(std::is_unsigned_v<UIntType>);
  // only one way to decode an empty string
  if (!length || length == 1)
     return 1;
  // possible decodings includes those for length - 1 message
  auto n_decodes = possible_decodings_r(message, length - 1);
  // if the last two characters are 10-26, there are additional decodings
  // possible based on the length - 2 message
  if (length > 1 && message[length - 2] <= '2' && message[length - 1] <= '6')
    return n_decodes + possible_decodings_r(message, length - 2);
  return n_decodes;
}

/**
 * Determine the number of possible decodings for a size `length` message.
 *
 * An O(N) recursive implementation using dynamic programming with O(N) space
 * requirement implementing using an `unordered_map`. See the doc of
 * `possible_decodings_r` for an explanation of the recursive strategy.
 *
 * @tparam CharContainer *Container* with `char` as the `value_type`
 * @tparam UIntType unsigned integral type
 *
 * @param message `char` *Container* message to decode
 * @param length message length, must be <= `message.size()` (unchecked)
 * @param cache cache for storing intermediate calculations
 */
template <typename CharContainer, typename UIntType>
UIntType possible_decodings_dp(
  const CharContainer& message,
  UIntType length,
  std::unordered_map<UIntType, UIntType>& cache)
{
  static_assert(std::is_same_v<typename CharContainer::value_type, char>);
  static_assert(std::is_unsigned_v<UIntType>);
  // we would like the UIntType to be able to index into the Container
  static_assert(std::is_convertible_v<UIntType, typename CharContainer::size_type>);
  // note addition of writing value to cache
  if (!length || length == 1) {
    cache[length] = 1;
    return 1;
  }
  // only calculate n_decodings for length - 1 if value not in cache
  if (cache.find(length - 1) == cache.cend())
    cache[length - 1] = possible_decodings_dp(message, length - 1, cache);
  // again, only calculate n_decodings for length - 2 if value not in cache
  if (length > 1 && message[length - 2] <= '2' && message[length - 1] <= '6') {
    if (cache.find(length - 2) == cache.cend())
      cache[length - 2] = possible_decodings_dp(message, length - 2, cache);
    return cache.at(length - 1) + cache.at(length - 2);
  }
  return cache.at(length - 1);
}

}  // namespace detail

template <typename CharContainer>
inline auto possible_decodings_r(const CharContainer& message)
{
  return detail::possible_decodings_r(message, message.size());
}

template <typename CharContainer>
inline auto possible_decodings_dp(const CharContainer& message)
{
  using size_type = typename CharContainer::size_type;
  std::unordered_map<size_type, size_type> cache;
  return detail::possible_decodings_dp(message, message.size(), cache);
}

// convenience type aliases
using pair_type = std::pair<std::string, typename std::string::size_type>;

/**
 * Test fixture class for parametrized testing.
 */
class DailyTest7 : public ::testing::TestWithParam<pair_type> {};

INSTANTIATE_TEST_SUITE_P(
  SamplePairs,
  DailyTest7,
  ::testing::Values(pair_type{"111", 3})
);

INSTANTIATE_TEST_SUITE_P(
  CustomPairs,
  DailyTest7,
  ::testing::Values(
    /*
    1 1 2 3 2
    11 2 3 2
    1 12 3 2
    1 1 23 2
    11 23 2
    */
    pair_type{"11232", 5},
    /*
    1 1 1 4
    11 1 4
    1 11 4
    1 1 14
    11 14
    */
    pair_type{"1114", 5},
    /*
    5 7 9 8 1 2 2 1
    5 7 9 8 12 2 1
    5 7 9 8 1 22 1
    5 7 9 8 1 2 21
    5 7 9 8 12 21
    */
    pair_type{"57981221", 5},
    /*
    3 5 2 5 1 2 3 4 3 4
    3 5 25 1 2 3 4 3 4
    3 5 2 5 12 3 4 3 4
    3 5 2 5 1 23 4 3 4
    3 5 25 12 3 4 3 4
    3 5 25 1 23 4 3 4
    */
    pair_type{"3525123434", 6}
  )
);

/**
 * Test that `possible_decodings_r` works correctly.
 */
TEST_P(DailyTest7, RecParamTest)
{
  EXPECT_EQ(GetParam().second, possible_decodings_r(GetParam().first));
}

/**
 * Test that `possible_decodings_dp` works correctly.
 */
TEST_P(DailyTest7, DynParamTest)
{
  EXPECT_EQ(GetParam().second, possible_decodings_dp(GetParam().first));
}

}  // namespace
