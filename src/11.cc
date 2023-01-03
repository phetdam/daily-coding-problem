/**
 * @file 11.cc
 * @author Derek Huang
 * @brief #11 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Twitter.
 *
 * Implement an autocomplete system. That is, given a query string s and a set
 * of all possible query strings, return all strings in the set that have s as
 * a prefix. For example, given the query string de and the set of strings
 * [dog, deer, deal], return [deer, deal]. Hint: Try preprocessing the
 * dictionary into a more efficient data structure to speed up queries.
 */

#include <set>
#include <string>
#include <utility>

#include <gtest/gtest.h>

#include "pddcp/prefix_tree.h"

namespace {

// convenience type aliases
using result_type = std::set<std::string>;
using input_type = std::pair<std::string, result_type>;
using pair_type = std::pair<input_type, result_type>;

/**
 * Test fixture for parametrized unit testing.
 */
class DailyTest11 : public ::testing::TestWithParam<pair_type> {};

INSTANTIATE_TEST_SUITE_P(
  SamplePairs,
  DailyTest11,
  ::testing::Values(
    pair_type{
      {"de", {"dog", "deer", "deal"}},
      {"deer", "deal"}
    }
  )
);

INSTANTIATE_TEST_SUITE_P(
  CustomPairs,
  DailyTest11,
  ::testing::Values(
    pair_type{
      {"bre", {"breeze", "brash", "breeder", "brilliant"}},
      {"breeze", "breeder"}
    },
    pair_type{
      {"flam", {"famous", "flanders", "flamboyant", "flatulence"}},
      {"flamboyant"}
    },
    pair_type{
      {"a", {"apple", "abject", "artifice", "camaraderie"}},
      {"apple", "abject", "artifice"}
    }
  )
);

/**
 * Test that `pddcp::trie::autocomplete` works as expected.
 */
TEST_P(DailyTest11, ParamTest)
{
  const auto& [prefix, values] = GetParam().first;
  const auto& true_autocomplete = GetParam().second;
  auto trie = pddcp::trie::from(values);
  EXPECT_EQ(true_autocomplete, pddcp::trie::autocomplete(&trie, prefix));
}

}  // namespace
