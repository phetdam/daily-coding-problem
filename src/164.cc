/**
 * @file 164.cc
 * @author Derek Huang
 * @brief #164 from Daily Coding Problem
 * @copyright MIT License
 *
 * This problem was asked by Google.
 *
 * You are given an array of length n + 1 whose elements belong to the set
 * {1, 2, ..., n}. By the pigeonhole principle, there must be a duplicate. Find
 * it in linear time and space.
 */

#include <deque>
#include <list>
#include <map>
#include <numeric>
#include <set>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include <gtest/gtest.h>

#include "pddcp/type_traits.h"
#include "pddcp/utility.h"

namespace {

/**
 * Return the duplicate in a N + 1 length container, values from {1, ... N}.
 *
 * The actual values in the container are not checked and are assumed to be
 * drawn the set {1, ... N}. This implementation is O(N) and requires only
 * O(1) space, as it exploits the fact that the sum of 1 to N is just
 * N * (N + 1) / 2, so by taking the sum of the container elements and
 * subtracting N * (N + 1) / 2 from it, we get the duplicate element.
 *
 * @tparam Container *Container* with integral `value_type`
 *
 * @param value *Container* with N + 1 values drawn from {1, ... N}.
 */
template <typename Container>
auto pigeonhole_duplicate(const Container& values)
{
  using value_type = typename Container::value_type;
  static_assert(
    std::is_integral_v<value_type>, "Container value_type must be integral"
  );
  // size must be 2 or greater under problem assumptions. if size is 0, we get
  // wraparound to unsigned value of -1, but we'll throw anyways
  auto n = values.size() - 1;
  if (values.size() < 2)
    throw std::runtime_error{"values must contain 2 or more elements"};
  // since the sum of 1 to n is n * (n + 1) / 2, we can detect the value of the
  // duplicate by simply summing the values and subtract n * (n + 1 ) / 2
  auto values_sum = std::accumulate(values.begin(), values.end(), value_type{});
  // need cast to value_type in case value_type is smaller than size_type
  return values_sum - static_cast<value_type>(n * (n + 1) / 2);
}

/**
 * Base helper template for the value frequency map for a container type `T`.
 *
 * This is chosen when `T` does not have a `std::hash` specialization for its
 * `value_type` and set `type` to a `std::map` specialization.
 *
 * @tparam T *Container* type
 */
template <typename T, typename = void>
struct container_value_frequency_map {
  using type = std::map<typename T::value_type, typename T::size_type>;
};


/**
 * Partial specialization for when `T` has a `std::hash<T>` specialization.
 *
 * This is chosen when `T` has a `std::hash` specialization for its
 * `value_type`, so we can use `std::unordered_map` instead of `std::map`.
 *
 * @tparam T *Container* type
 */
template <typename T>
struct container_value_frequency_map<
    T, std::void_t<pddcp::is_std_hashable<typename T::value_type>>> {
  using type = std::unordered_map<typename T::value_type, typename T::size_type>;
};

/**
 * Helper type alias template for the value frequency map for container `T`.
 *
 * @tparam T *Container* type
 */
template <typename T>
using container_value_frequency_map_t =
  typename container_value_frequency_map<T>::type;

/**
 * Return a set of all the duplicates in a given *Container*.
 *
 * Barring a user-specified `MapType`, the runtime of this O(N) space
 * implementation can be O(N) or O(N log(N)) depending on the default value
 * frequency map type chosen for `MapType`. However, the worst-case is still
 * O(N log(N)) if the number of duplicates is large, i.e. close to N, as
 * `std::set` has logarithmic insertion time.
 *
 * @tparam Container *Container* type
 * @tparam MapType Mapping type that maps the `value_type` of `Container` to
 *  the `size_type` of `Container`. Defaults to
 *  `container_value_frequency_map_t<Container>`, but can be user-specified.
 *
 * @param values *Container* of values to find all duplicates for
 */
template <
  typename Container,
  typename MapType = container_value_frequency_map_t<Container>>
auto find_duplicates(const Container& values)
{
  // enforce that value and size types match (if user specifies MapType)
  using value_type = typename Container::value_type;
  static_assert(
    std::is_same_v<typename MapType::key_type, value_type>,
    "MapType key_Type must equal Container value_type"
  );
  static_assert(
    std::is_same_v<typename MapType::size_type, typename Container::size_type>,
    "MapType size_type must equal Container size_type"
  );
  // check duplicates by counting number of occurrences
  MapType value_map;
  for (const auto& value : values)
    value_map[value] += 1;
  // collect duplicates and return
  std::set<value_type> dups;
  for (const auto& [value, count] : value_map)
    if (count > 1)
      dups.emplace(value);
  return dups;
}

/**
 * Base test class template.
 *
 * @tparam IndexedType `pddcp::indexed_type<I, T>`, `T` a *Container*
 */
template <typename IndexedType>
class DailyTest164 : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_CONTAINER_HELPER_TYPES(IndexedType);
};

// input types used in the specializations + TYPED_TEST_SUITE macro
using InputType1 = pddcp::indexed_type<0, std::vector<int>>;
using InputType2 = pddcp::indexed_type<1, std::vector<std::string>>;
using InputType3 = pddcp::indexed_type<2, std::list<unsigned int>>;
using InputType4 = pddcp::indexed_type<3, std::deque<double>>;

/**
 * Specialization for first input/output pair.
 *
 * Here `pigeonhole_duplicate` and `find_duplicates` will be run.
 */
template <>
class DailyTest164<InputType1> : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_CONTAINER_HELPER_TYPES(InputType1);
protected:
  static inline const element_type values_{3, 1, 2, 3, 4, 5};
  static inline const std::set<value_type> duplicates_{3};
};

/**
 * Specialization for the second input/output pair using strings as values.
 *
 * Only `find_duplicates` will run on this input.
 */
template <>
class DailyTest164<InputType2> : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_CONTAINER_HELPER_TYPES(InputType2);
protected:
  static inline const element_type values_{
    "abc", "hello", "cheese", "hello", "banana", "apple", "banana", "yeetus"
  };
  static inline const std::set<value_type> duplicates_{"banana", "hello"};
};

/**
 * Specialization for the third input/output pair.
 *
 * Here `pigeonhole_duplicate` and `find_duplicates` will be run. The input
 * container is a `std::list<T>` to show that the functions work for non-vector
 * containers, in particular any object that exposes iterators.
 */
template <>
class DailyTest164<InputType3> : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_CONTAINER_HELPER_TYPES(InputType3);
protected:
  static inline const element_type values_{4, 1, 6, 2, 3, 4, 5};
  static inline const std::set<value_type> duplicates_{4};
};

/**
 * Specialization for the fourth input/output pair.
 *
 * Only `find_duplicates` will run on this input. The input container is a
 * `std::deque<double>` that contains multiple duplicates.
 */
template <>
class DailyTest164<InputType4> : public ::testing::Test {
public:
  PDDCP_INDEXED_TYPE_CONTAINER_HELPER_TYPES(InputType4);
protected:
  static inline const element_type values_{
    4.1, 4.1, 1., 13., 23., 14.5, 14.5, 2.2, 2.2, 2., 16., 14.
  };
  static inline const std::set<value_type> duplicates_{2.2, 4.1, 14.5};
};

using DailyTest164Types = ::testing::Types<
  InputType1, InputType2, InputType3, InputType4
>;
TYPED_TEST_SUITE(DailyTest164, DailyTest164Types);

/**
 * Test that `pigeonhole_duplicate` works as expected.
 *
 * This test is skipped for any `values_` input with non-integral `value_type`.
 */
TYPED_TEST(DailyTest164, PigeonDuplicateTest)
{
  if constexpr (std::is_integral_v<typename TestFixture::value_type>)
    // first element of duplicates_ is the expected pigeonhole_duplicate result
    EXPECT_EQ(
      *TestFixture::duplicates_.begin(),
      pigeonhole_duplicate(TestFixture::values_)
    );
  else
    GTEST_SKIP();
}

/**
 * Test that `find_duplicates` works as expected.
 */
TYPED_TEST(DailyTest164, FullDuplicateTest)
{
  EXPECT_EQ(TestFixture::duplicates_, find_duplicates(TestFixture::values_));
}

}  // namespace
