/**
 * @file 51.cc
 * @author Derek Huang
 * @brief #51 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Facebook.
 *
 * Given a function that generates perfectly random numbers between 1 and k
 * (inclusive), where k is an input, write a function that shuffles a deck of
 * cards represented as an array using only swaps. It should run in O(N) time.
 *
 * Hint: Make sure each one of the 52! permutations of the deck is equally
 * likely.
 */

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <random>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

namespace {

/**
 * Return a deck of cards shuffled in O(N) complexity through swaps.
 *
 * Since shuffling a deck of cards is the same as permuting it, the algorithm
 * is quite simple--for each index `i`, randomly select an index from `i` to
 * the last index inclusive, swap the two values at those positions, repeat.
 *
 * @tparam IntType Integral type for a card's value
 * @tparam Rng *UniformRandomBitGenerator*
 *
 * @param deck `IntType` vector representing the deck
 * @param rng *UniformRandomBitGenerator* PRNG
 */
template <typename IntType, typename Rng>
std::vector<IntType> permute(const std::vector<IntType>& deck, Rng rng)
{
// in C++20, we can use std::remove_cvref_t instead which is more concise
#if __cplusplus >= 202002L
  using size_type = typename std::remove_cvref_t<decltype(deck)>::size_type;
#else
  using size_type = typename std::remove_const_t<
    std::remove_reference_t<decltype(deck)>
  >::size_type;
#endif  // __cplusplus < 202002L
  // only integral types + Rng result type is convertible to size_type
  static_assert(std::is_integral_v<IntType>);
  static_assert(std::is_convertible_v<typename Rng::result_type, size_type>);
  // use copy, although we could also do this in-place
  std::vector<IntType> new_deck = deck;
  // shuffling a deck is simply permuting it, so for each index we choose one
  // of the remaining cards, perform the swap, and repeat for each index
  const auto n_cards = deck.size();
  for (size_type i = 0; i < n_cards; i++) {
    std::uniform_int_distribution<size_type> udist(i, n_cards - 1);
    std::swap(new_deck[i], new_deck[udist(rng)]);
  }
  return new_deck;
}

/**
 * Return a deck of cards shuffled in O(N) complexity through swaps.
 *
 * Uses the 32-bit Mersenne Twister as the PRNG.
 *
 * @tparam IntType Integral type for a card's value
 *
 * @param deck `IntType` vector representing the deck
 * @param seed Seed value for the Mersenne Twister
 */
template <typename IntType>
inline std::vector<IntType> permute(
  const std::vector<IntType>& deck, std::uint_fast32_t seed)
{
  return permute(deck, std::mt19937{seed});
}

/**
 * Return a deck of cards shuffled in O(N) complexity through swaps.
 *
 * Uses the 32-bit Mersenne Twister as PRNG and `std::random_device` to seed.
 *
 * @tparam IntType Integral type for a card's value
 *
 * @param deck `IntType` vector representing the deck
 */
template <typename IntType>
inline std::vector<IntType> permute(const std::vector<IntType>& deck)
{
  return permute(deck, std::random_device{}());
}

// convenience type aliases
using result_type = std::vector<unsigned int>;
using pair_type = std::pair<result_type, result_type>;

/**
 * Test fixture template class for testing.
 *
 * Since we can't predict the actual output, we instead test that using the
 * same seed and with different types, all the shuffled decks are valid, i.e.
 * each deck of size N has N unique values post-shuffle. The decks are
 * constructed with a `std::iota` call, so values are consectutively ascending.
 */
template <typename TypePair>
class DailyTest51 : public ::testing::TestWithParam<pair_type> {
protected:
  // convenience type aliases
  using int_type = typename TypePair::first_type;
  using seed_type = typename TypePair::second_type;
  // the actual value vector used for the deck + PRNG seed value
  static inline const auto deck_{
    []()
    {
      std::vector<int_type> deck(52);
      std::iota(deck.begin(), deck.end(), 1);
      return deck;
    }()
  };
  static inline const seed_type seed_{88};
};

using DailyTestTypes51 = ::testing::Types<
  std::pair<unsigned int, std::uint_fast32_t>,
  std::pair<long, std::uint_fast32_t>,
  std::pair<std::uint_fast64_t, std::uint_fast32_t>
>;
TYPED_TEST_SUITE(DailyTest51, DailyTestTypes51);

/**
 * Test that `permute` works as intended on a "deck" for different types.
 */
TYPED_TEST(DailyTest51, TypedTest)
{
  using int_type = typename TestFixture::int_type;
  const auto valid_shuffle = [&]()
  {
    const auto new_deck = permute(TestFixture::deck_, TestFixture::seed_);
    std::unordered_map<int_type, unsigned short> value_map;
    for (const auto& card : new_deck)
      value_map[card] = 1;
    return (value_map.size() == new_deck.size()) ? true : false;
  }();
  EXPECT_TRUE(valid_shuffle);
}

}  // namespace
