/**
 * @file type_traits_test.cc
 * @author Derek Huang
 * @brief type_traits.h unit tests
 * @copyright MIT License
 */

#include "pddcp/type_traits.h"

#include <array>
#include <cstdint>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "pddcp/enums.h"

namespace {

/**
 * Test fixture class template for testing type traits classes.
 *
 * @tparam InputType `std::pair<Traits, TruthType>` where `Traits` is the type
 *   resulting from applying a traits template to a type and `TruthType` is
 *  `std::true_type` or `std::false_type`. See examples for helper type
 *  template definitions that provide simpler use than manual repetition.
 *
 * @note The `Traits` type is expected to be a `std::bool_constant<B>` subclass
 *  or at least contain the static constexpr boolean `value` member.
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
 * Sample unscoped and scoped enum types.
 */
enum unscoped_enum {
  unscoped_enum_first,
  unscoped_enum_second,
  unscoped_enum_third,
  unscoped_enum_count
};
enum class scoped_enum { first, second, third, count };

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
using InputType13 = is_bitmask_type_input<unscoped_enum, false>;
using InputType14 = is_bitmask_type_input<scoped_enum, false>;
using InputType15 = is_bitmask_type_input<pddcp::execution, true>;

/**
 * Helper for `pddcp::is_bitmask_enum<T>` input type creation.
 *
 * @tparam T input type
 * @tparam truth Expected truth given by the traits type
 */
template <typename T, bool truth>
using is_bitmask_enum_input = std::pair<
  pddcp::is_bitmask_enum<T>, std::bool_constant<truth>
>;

// types for pddcp::is_bitmask_enum<T> testing
using InputType16 = is_bitmask_enum_input<unscoped_enum, false>;
using InputType17 = is_bitmask_enum_input<scoped_enum, false>;
using InputType18 = is_bitmask_enum_input<pddcp::execution, true>;

/**
 * Helper for `pddcp::is_std_hashable<T>` input type creation.
 *
 * @tparam T input type
 * @tparam truth Expected truth given by the traits type
 */
template <typename T, bool truth>
using is_std_hashable_input = std::pair<
  pddcp::is_std_hashable<T>, std::bool_constant<truth>
>;

// types for pddcp::is_std_hashable<T> testing
using InputType19 = is_std_hashable_input<double, true>;
using InputType20 = is_std_hashable_input<std::string, true>;
using InputType21 = is_std_hashable_input<pair_like<int, std::string>, false>;
using InputType22 = is_std_hashable_input<std::vector<bool>, true>;
using InputType23 = is_std_hashable_input<std::vector<std::string>, false>;

/**
 * Helper for `pddcp::is_std_vector<T>` input type creation.
 *
 * @tparam T input type
 * @tparam truth Expected truth given by the traits type
 */
template <typename T, bool truth>
using is_std_vector_input = std::pair<
  pddcp::is_std_vector<T>, std::bool_constant<truth>
>;

// types for pddcp::is_std_vector<T> testing
using InputType24 = is_std_vector_input<double, false>;
using InputType25 = is_std_vector_input<std::array<double, 10>, false>;
using InputType26 = is_std_vector_input<std::vector<unsigned int>, true>;
using InputType27 = is_std_vector_input<std::vector<std::string>, true>;
using InputType28 = is_std_vector_input<double[20], false>;

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
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType13);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType14);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType15);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType16);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType17);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType18);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType19);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType20);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType21);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType22);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType23);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType24);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType25);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType26);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType27);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType28);

using TypeTraitsTestTypes = ::testing::Types<
  // types for pddcp::is_homogenous_pair<N, T> testing
  InputType1, InputType2, InputType3, InputType4, InputType5, InputType6,
  // types for pddcp::is_bitmask_type<T> testing
  InputType7,
  InputType8,
  InputType9,
  InputType10,
  InputType11,
  InputType12,
  InputType13,
  InputType14,
  InputType15,
  // types for pddcp::is_bitmask_enum<T> testing
  InputType16, InputType17, InputType18,
  // types for pddcp::is_std_hashable<T> testing
  InputType19, InputType20, InputType21, InputType22, InputType23,
  // types for pddcp::is_std_vector<T> testing
  InputType24, InputType25, InputType26, InputType27, InputType28
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
