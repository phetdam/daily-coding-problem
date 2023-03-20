/**
 * @file 47.cc
 * @author Derek Huang
 * @brief #47 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Facebook.
 *
 * Given a array of numbers representing the stock prices of a company in
 * chronological order, write a function that calculates the maximum profit you
 * could have made from buying and selling that stock once. You must buy before
 * you can sell it. For example, given [9, 11, 8, 5, 7, 10], you should return
 * 5, since you could buy the stock at 5 dollars and sell it at 10 dollars.
 */

#include <algorithm>
#include <iterator>
#include <limits>
#include <type_traits>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

namespace {

/**
 * Return max possible profit of a single buy and sell given price history.
 *
 * Solution works in O(N) time by moving in reverse to collect a running max
 * of the max profit and max sell price. This implementation works with
 * negative prices as well, and if only losses are possible, returns the
 * smallest possible loss given the losing price history.
 *
 * @tparam InputIt *LegacyRandomAccessIterator* and *LegacyContiguousIterator*
 *
 * @param first Iterator to first price
 * @param last Iterator to one past the last price
 */
template <typename InputIt>
auto max_profit(InputIt first, InputIt last)
{
  // shorthand for the iterator value type + type check
  using result_type = typename std::iterator_traits<InputIt>::value_type;
  static_assert(
    std::is_arithmetic_v<result_type>,
    "InputIt value_type must be arithmetic type"
  );
  // if these are equal, no prices, so return 0
  if (first == last)
    return result_type{0};
  // current max profit, current stock price, current best sell price
  auto profit = std::numeric_limits<result_type>::min();
  result_type price;
  auto sell_price = *(--last);
  // go backwards from last price to first so we can do a rolling max profit
  while (--last != first) {
    price = *last;
    profit = std::max(profit, sell_price - price);
    // if we could have sold at a higher price in the past, update
    sell_price = std::max(sell_price, price);
  }
  return profit;
}

/**
 * Return max possible profit of a single buy and sell given price history.
 *
 * Main overload that should be used with common *Container* classes.
 *
 * @tparam Container *Container* with arithmetic `value_type`
 *
 * @param values Container of price values in chronological order
 */
template <typename Container>
inline auto max_profit(const Container& values)
{
  return max_profit(values.begin(), values.end());
}

// convenience type aliases
using input_type = std::vector<double>;
using result_type = typename input_type::value_type;
using pair_type = std::pair<input_type, result_type>;

/**
 * Test fixture class for parametrized testing.
 */
class DailyTest47 : public ::testing::TestWithParam<pair_type> {};

INSTANTIATE_TEST_SUITE_P(
  SamplePairs,
  DailyTest47,
  ::testing::Values(pair_type{{9, 11, 8, 5, 7, 10}, 5})
);

INSTANTIATE_TEST_SUITE_P(
  CustomPairs,
  DailyTest47,
  ::testing::Values(
    pair_type{{4, 3.5, -14, -1.5, 10.2, 9}, 24.2},
    pair_type{{4, 1, 14.156, -3.1, 4.45, 13, 20.1, 11}, 23.2}
  )
);

/**
 * Test that `max_profit` works correctly.
 */
TEST_P(DailyTest47, ParamTest)
{
  EXPECT_DOUBLE_EQ(GetParam().second, max_profit(GetParam().first));
}

}  // namespace
