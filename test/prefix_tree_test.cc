/**
 * @file prefix_tree_test.cc
 * @author Derek Huang
 * @brief prefix_tree.h unit tests
 * @copyright MIT License
 */

#include "pddcp/prefix_tree.h"

#include <set>
#include <string>
#include <utility>

#include <gtest/gtest.h>

namespace {

// convenience type aliases
using result_type = std::set<std::string>;
using input_type = std::pair<std::string, result_type>;
using pair_type = std::pair<input_type, result_type>;

/**
 * Test fixture class for parametrized tests.
 *
 * Since we are using `TEST_P` and `GetParam()` can only be called inside the
 * `TEST_P` macro, each `TEST_P` will create one trie per test, which is a bit
 * wasteful but an unfortunate limitation of Google Test.
 */
class PrefixTreeTest : public ::testing::TestWithParam<pair_type> {};

INSTANTIATE_TEST_SUITE_P(
  ParamTest,
  PrefixTreeTest,
  ::testing::Values(
    // format for each pair_type: {prefix, values}, true_autocomplete
    pair_type{
      {"du", {"dullard", "cheeseburger", "tabernacle", "dump", "dumb"}},
      {"dullard", "dump", "dumb"}
    },
    pair_type{
      {"boo", {"boolean", "bulimic", "holy", "bourgeois", "boost"}},
      {"boolean", "boost"}
    },
    pair_type{
      {"cha", {"chalice", "chatter", "chilling", "daunting", "chauvinistic"}},
      {"chalice", "chatter", "chauvinistic"}
    }
  )
);

// macro for unpacking the values of each pair_type GetParam() in TEST_P and
// using pddcp::trie::from to create a trie. since we don't always need
// true_autocomplete, we disable the unused variable compiler warning.
#ifdef _MSC_VER
#define PDDCP_PREFIX_TREE_TEST_SETUP \
  const auto& [prefix, values] = GetParam().first; \
  _Pragma("warning (push)") \
  _Pragma("warning (disable: 4189)") \
  const auto& true_autocomplete = GetParam().second; \
  _Pragma("warning (pop)") \
  auto trie = pddcp::trie::from(values)
#else
#define PDDCP_PREFIX_TREE_TEST_SETUP \
  const auto& [prefix, values] = GetParam().first; \
  _Pragma("GCC diagnostic push") \
  _Pragma("GCC diagnostic ignored \"-Wunused-variable\"") \
  const auto& true_autocomplete = GetParam().second; \
  _Pragma("GCC diagnostic pop") \
  auto trie = pddcp::trie::from(values)
#endif  // _MSC_VER

/**
 * Test that `pddcp::trie::from` and `pddcp::trie::to_set` work as expected.
 *
 * Indirectly tests `pddcp::trie::to_vector`, which `to_set` wraps.
 */
TEST_P(PrefixTreeTest, FromToTest)
{
  PDDCP_PREFIX_TREE_TEST_SETUP;
  EXPECT_EQ(values, pddcp::trie::to_set(&trie));
}

/**
 * Test that `pddcp::trie::contains` works as expected.
 */
TEST_P(PrefixTreeTest, ContainsTest)
{
  PDDCP_PREFIX_TREE_TEST_SETUP;
  EXPECT_TRUE(pddcp::trie::contains(&trie, prefix));
}

/**
 * Test that `pddcp::trie::autocomplete` works as expected.
 */
TEST_P(PrefixTreeTest, AutocompleteTest)
{
  PDDCP_PREFIX_TREE_TEST_SETUP;
  EXPECT_EQ(true_autocomplete, pddcp::trie::autocomplete(&trie, prefix));
}

}  // namespace
