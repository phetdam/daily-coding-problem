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
 * Test fixture class template for testing type traits classes.
 *
 * @tparam InputType `std::pair<Traits, TruthType>` where `Traits` is the type
 *   resulting from applying a traits template to a type and `TruthType` is
 *  `std::true_type` or `std::false_type`. See examples for helper type
 *  template definitions that provide simpler use than manual repetition.
 */
template <typename InputType>
class TypeTraitsTest : public ::testing::Test {};

/**
 * Macro for simplifying `TypeTraitsTest` specializations.
 *
 * @param traits_input `InputType` as described above
 */
#define PDDCP_TYPE_TRAITS_TEST_CLASS(traits_input) \
  template <> \
  class TypeTraitsTest<traits_input> : public ::testing::Test { \
  public: \
    using traits_type = typename traits_input::first_type; \
    using value_type = typename traits_type::value_type; \
    using truth_type = typename traits_input::second_type; \
  protected: \
    static inline constexpr bool expected_ = truth_type::value; \
    static inline constexpr bool actual_ = traits_type::value; \
  }

/**
 * A user-defined pair-like class template.
 *
 * We use this to test `pddcp::is_homogenous_pair<N, T>`.
 *
 * @tparam T1 first type
 * @tparam T2 second type
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

/**
 * Helper for `pddcp::is_homogenous_pair<N, T>` input type creation.
 *
 * @tparam T input type
 * @tparam truth Expected truth given by the traits type
 */
template <typename T, bool truth>
using is_homogenous_pair_input = std::pair<
  pddcp::is_homogenous_pair<T>, std::bool_constant<truth>
>;

// types for pddcp::is_homogenous_pair_v<N, T> testing
using InputType1 = is_homogenous_pair_input<std::pair<bool, bool>, true>;
using InputType2 = is_homogenous_pair_input<std::tuple<double, double>, true>;
using InputType3 = is_homogenous_pair_input<std::pair<int, double>, false>;
using InputType4 = is_homogenous_pair_input<std::array<short, 2>, true>;
using InputType5 = is_homogenous_pair_input<pair_like<float, float>, true>;
using InputType6 = is_homogenous_pair_input<pair_like<int, double>, false>;

/**
 * Helper for `pddcp::is_bitmask_type<T>` input type creation.
 *
 * @tparam T input type
 * @tparam truth Expected truth given by the traits type
 */
template <typename T, bool truth>
using is_bitmask_type_input = std::pair<
  pddcp::is_bitmask_type<T>, std::bool_constant<truth>
>;

// types for pddcp::is_bitmask_type<T> testing
using InputType7 = is_bitmask_type_input<int, true>;
using InputType8 = is_bitmask_type_input<short, true>;
using InputType9 = is_bitmask_type_input<std::tuple<double, int>, false>;
using InputType10 = is_bitmask_type_input<std::size_t, true>;
using InputType11 = is_bitmask_type_input<const char*, false>;
using InputType12 = is_bitmask_type_input<std::array<char*, 9>, false>;

// specialization creation using the input types
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType1);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType2);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType3);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType4);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType5);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType6);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType7);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType8);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType9);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType10);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType11);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType12);

using TypeTraitsTestTypes = ::testing::Types<
  // types for pddcp::is_homogenous_pair<N, T> testing
  InputType1, InputType2, InputType3, InputType4, InputType5, InputType6,
  // types for pddcp::is_bitmask_type<T> testing
  InputType7, InputType8, InputType9, InputType10, InputType11, InputType12
>;
TYPED_TEST_SUITE(TypeTraitsTest, TypeTraitsTestTypes);

/**
 * Test that a traits class works as expected.
 */
TYPED_TEST(TypeTraitsTest, TraitsTest)
{
  EXPECT_EQ(TestFixture::expected_, TestFixture::actual_);
}

}  // namespace
