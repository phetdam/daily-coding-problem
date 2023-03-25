/**
 * @file type_traits_test.cc
 * @author Derek Huang
 * @brief type_traits.h unit tests
 * @copyright MIT License
 */

#include "pddcp/type_traits.h"

#include <array>
#include <cstdint>
#include <tuple>
#include <type_traits>
#include <utility>

#include <gtest/gtest.h>

namespace {

/**
 * Test fixture class template for testing `is_homogenous_pair`.
 */
template <typename T>
class TypeTraitsIsHomogenousPairTest : public ::testing::Test {};

/**
 * A user-defined pair-like class template.
 */
template <typename T1, typename T2>
class pair_like {
public:
  using first_type = T1;
  using second_type = T2;

  pair_like(const T1& first, const T2& second)
    : first_{first}, second_{second}
  {}

  const auto& first() const { return first_; }
  const auto& second() const { return second_; }

private:
  T1 first_;
  T2 second_;
};

}  // namespace

/**
 * `std::tuple_size` and `std::tuple_element` specializations for `pair_like`,
 *
 * `std::tuple_size_v<pair_like<T1, T2>>` is always 2 while
 * `std::tuple_element<I, T>` will only contain the `type` member if `I` 0, 1.
 */
namespace std {

template <typename T1, typename T2>
struct tuple_size<pair_like<T1, T2>> : std::integral_constant<std::size_t, 2> {};

template <std::size_t I, typename T1, typename T2>
struct tuple_element<I, pair_like<T1, T2>> {};

template <typename T1, typename T2>
struct tuple_element<0, pair_like<T1, T2>> { using type = T1; };

template <typename T1, typename T2>
struct tuple_element<1, pair_like<T1, T2>> { using type = T2; };

}  // namespace std

namespace {

using TypeTraitsIsHomogenousPairTestTypes = ::testing::Types<
  std::pair<std::pair<bool, bool>, std::true_type>,
  std::pair<std::tuple<double, double>, std::true_type>,
  std::pair<std::pair<int, double>, std::false_type>,
  std::pair<std::array<short, 2>, std::true_type>,
  std::pair<pair_like<float, float>, std::true_type>,
  std::pair<pair_like<int, double>, std::false_type>
>;
TYPED_TEST_SUITE(
  TypeTraitsIsHomogenousPairTest, TypeTraitsIsHomogenousPairTestTypes
);

/**
 * Test that `is_homogenous_pair` works as expected.
 */
TYPED_TEST(TypeTraitsIsHomogenousPairTest, TruthTest)
{
  using truth_type = typename TypeParam::second_type;
  EXPECT_EQ(
    truth_type::value,
    pddcp::is_homogenous_pair_v<typename TypeParam::first_type>
  );
}

}  // namespace
