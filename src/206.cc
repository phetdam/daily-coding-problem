/**
 * @file 206.cc
 * @author Derek Huang
 * @brief #206 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Twitter.
 *
 * A permutation can be specified by an array P, where P[i] represents the
 * location of the element at i in the permutation. For example, [2, 1, 0]
 * represents the permutation where elements at the index 0 and 2 are swapped.
 *
 * Given an array and a permutation, apply the permutation to the array. For
 * example, given the array ["a", "b", "c"] and the permutation [2, 1, 0],
 * return ["c", "b", "a"].
 */

#include <algorithm>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

namespace {

template <typename IndexContainer>
bool is_permutation(const IndexContainer& indices)
{
  // if using integral indexing, the index type must be unsigned integral
  using index_type = typename IndexContainer::value_type;
  using size_type = typename IndexContainer::size_type;
  static_assert(std::is_unsigned_v<index_type>);
  // bool vector (can't use bitset as size() not guaranteed constexpr) to check
  // that we don't have any repeats in the indices
  std::vector<bool> index_presences(indices.size());
  // set true all the available indices, exiting early if duplicates exist
  for (const auto& index : indices) {
    // must first check if the indices are in bounds of course
    if (size_type{index} > indices.size() - 1 || index_presences[index])
      return false;
    index_presences[index] = true;
  }
  // one more sweep to catch any unset members
  for (auto index_present : index_presences)
    if (!index_present)
      return false;
  return true;
}

template <typename ValueContainer, typename IndexContainer>
auto permute(
  const ValueContainer& values,
  const IndexContainer& indices,
  bool check_indices = false)
{
  // index value type needs to be unsigned integral + convertible to size_type
  using size_type = typename ValueContainer::size_type;
  using index_type = typename IndexContainer::value_type;
  static_assert(std::is_unsigned_v<index_type>);
  static_assert(std::is_convertible_v<index_type, size_type>);
  // sanity check for size + optional permutation check
  if (values.size() != indices.size())
    throw std::runtime_error("values and indices must have same length");
  if (check_indices && !is_permutation(indices))
    throw std::runtime_error("indices do not make a valid permutation");
  // make permutation and return
  ValueContainer new_values(values.size());
  for (size_type i = 0; i < values.size(); i++)
    new_values[i] = values[indices[i]];
  return new_values;
}

// convenience type aliases. we use char to match the sample input.
using input_type = std::pair<
  std::pair<
    std::vector<char>,         // values for permute testing
    std::vector<unsigned int>  // valid permutation for permute testing
  >,
  std::vector<unsigned short>  // for testing is_permutation
>;
// vector to check permute result, bool to check is_permutation result
using result_type = std::pair<std::vector<char>, bool>;
using pair_type = std::pair<input_type, result_type>;

/**
 * Test fixture class for parametrized testnig.
 */
class DailyTest206 : public ::testing::TestWithParam<pair_type> {};

INSTANTIATE_TEST_SUITE_P(
  SamplePairs,
  DailyTest206,
  ::testing::Values(
    pair_type{
      {{{'a', 'b', 'c'}, {2, 1, 0}}, {1, 4, 2}},
      {{'c', 'b', 'a'}, false}
    }
  )
);

INSTANTIATE_TEST_SUITE_P(
  CustomPairs,
  DailyTest206,
  ::testing::Values(
    pair_type{
      {{{'c', 'a', '2', 'd', 's'}, {0, 4, 1, 3, 2}}, {1, 56, 2, 3}},
      {{'c', 's', 'a', 'd', '2'}, false}
    },
    pair_type{
      {{{'a', 'g', '1', 'd', 'b', 'j'}, {0, 2, 5, 1, 4, 3}}, {1, 0, 2, 3}},
      {{'a', '1', 'j', 'g', 'b', 'd'}, true}
    }
  )
);

/**
 * Test that `permute` works correctly.
 *
 * No permutation checking is done here.
 */
TEST_P(DailyTest206, PermuteTest)
{
  const auto& [input, output] = GetParam();
  EXPECT_EQ(output.first, permute(input.first.first, input.first.second));
}

/**
 * Test that `is_permutation` works correctly.
 */
TEST_P(DailyTest206, IsPermutationTest)
{
  EXPECT_EQ(GetParam().second.second, is_permutation(GetParam().first.second));
}

}  // namespace
