/**
 * @file type_traits_test_1.cc
 * @author Derek Huang
 * @brief type_traits.h unit tests 1
 * @copyright MIT License
 */

#include "pddcp/type_traits.h"

#include <array>
#include <cstdint>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "pddcp/enums.h"
#include "pddcp/testing/common.h"
#include "pddcp/testing/type_traits_test.h"

namespace {

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

/**
 * Helper for `pddcp::is_iterable<T>` input type creation.
 *
 * @tparam T input type
 * @tparam truth Expected truth given by the traits type
 */
template <typename T, bool truth>
using is_iterable_input = std::pair<
  pddcp::is_iterable<T>, std::bool_constant<truth>
>;

// types for pddcp::is_iterable<T> testing
using InputType29 = is_iterable_input<std::string, true>;
using InputType30 = is_iterable_input<std::vector<std::string>, true>;
using InputType31 = is_iterable_input<double, false>;
using InputType32 = is_iterable_input<int[30], false>;
using InputType33 = is_iterable_input<std::unordered_map<int, int>, true>;

/**
 * Traits type helper for using `pddcp::value_type_t<T>`.
 *
 * Solves the issue of having `std::is_same<T, U>` for different input types
 * `T1`, `T2` in the case where `pddcp::value_type_t<T1>` is the same as
 * `pddcp::value_type_t<T2>`, so `value_type_t_input<T1, U>` would be the same
 * type as `value_type_t_input<T2, U>` and cause a compile error.
 *
 * @tparam T Input type
 * @tparam U Expected `pddcp::value_type_t<T>` type
 */
template <typename T, typename U>
struct value_type_t_helper {
  static inline constexpr bool value = std::is_same_v<pddcp::value_type_t<T>, U>;
};

/**
 * Helper for `pddcp::value_type_t<T>` input type creation.
 *
 * @tparam T Input type
 * @tparam U Expected `pddcp::value_type_t<T>` type
 */
template <typename T, typename U>
using value_type_t_input = std::pair<value_type_t_helper<T, U>, std::true_type>;

// types for pddcp::value_type_t<T> testing
using InputType34 = value_type_t_input<std::vector<int>, int>;
using InputType35 = value_type_t_input<double, void>;
using InputType36 = value_type_t_input<std::vector<std::string>, std::string>;
using InputType37 = value_type_t_input<
  std::unordered_map<std::string, std::size_t>,
  std::pair<const std::string, std::size_t>
>;

/**
 * Helper for `pddcp::has_value_type<T>` input type creation.
 *
 * @tparam T input type
 * @tparam truth Expected truth given by the traits type
 */
template <typename T, bool truth>
using has_value_type_input = std::pair<
  pddcp::has_value_type<T>, std::bool_constant<truth>
>;

// types for pddcp::has_value_type<T> testing
using InputType38 = has_value_type_input<int, false>;
using InputType39 = has_value_type_input<std::vector<double>, true>;
using InputType40 = has_value_type_input<double[20], false>;
using InputType41 = has_value_type_input<std::string, true>;

/**
 * Traits type helper for using `pddcp::innermost_value_type_t<T>`.
 *
 * Solves the same issue that `value_type_t_helper<T, U>` solves, but for input
 * types intended for `pddcp::innermost_value_type_t<T>` testing.
 *
 * @tparam T Input type
 * @tparam U Expected `pddcp::innermost_value_type_t<T>` type
 */
template <typename T, typename U>
struct innermost_value_type_t_helper {
  static inline constexpr bool value = std::is_same_v<
    pddcp::innermost_value_type_t<T>, U
  >;
};

/**
 * Helper for `pddcp::innermost_value_type_t<T>` input creation.
 *
 * @tparam T Input type
 * @tparam U Expected `pddcp::innermost_value_type_t<T>` type
 */
template <typename T, typename U>
using innermost_value_type_t_input = std::pair<
  innermost_value_type_t_helper<T, U>, std::true_type
>;

// types for pddcp::innermost_value_type_t<T> testing
using InputType42 = innermost_value_type_t_input<std::vector<double>, double>;
using InputType43 = innermost_value_type_t_input<double, void>;
using InputType44 = innermost_value_type_t_input<
  std::vector<std::unordered_map<std::string, std::size_t>>,
  std::pair<const std::string, std::size_t>
>;
using InputType45 = innermost_value_type_t_input<
  std::vector<std::vector<std::vector<std::wstring>>>, wchar_t
>;

/**
 * Traits type helper for using `pddcp::innermost_value_type_depth<T>`.
 *
 * Solves an issue similar to what `value_type_t_helper<T, U>` solves.
 *
 * @tparam T Input type
 * @tparam depth Expected `pddcp::innermost_value_type_depth<T>` value
 */
template <typename T, decltype(pddcp::innermost_value_type_depth<T>) depth>
struct innermost_value_type_depth_helper {
  static inline constexpr bool value = (
    depth == pddcp::innermost_value_type_depth<T>
  );
};

/**
 * Helper for `pddcp::innermost_value_type_depth<T>` input creation.
 *
 * @tparam T Input type
 * @tparam depth Expected `pddcp::innermost_value_type_depth<T>` value
 */
template <typename T, decltype(pddcp::innermost_value_type_depth<T>) depth>
using innermost_value_type_depth_input = std::pair<
  innermost_value_type_depth_helper<T, depth>, std::true_type
>;

// types for pddcp::innermost_value_type_depth<T> testing
using InputType46 = innermost_value_type_depth_input<std::vector<double>, 1>;
using InputType47 = innermost_value_type_depth_input<double, 0>;
using InputType48 = innermost_value_type_depth_input<
  std::vector<std::unordered_map<std::string, std::size_t>>, 2
>;
using InputType49 = innermost_value_type_depth_input<
  std::vector<std::vector<std::vector<std::wstring>>>, 4
>;

// input types and type-parametrized test suite instantiation
using TypeTraitsTestTypes1 = ::testing::Types<
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
  InputType24, InputType25, InputType26, InputType27, InputType28,
  // types for pddcp::is_iterable<T> testing
  InputType29, InputType30, InputType31, InputType32, InputType33,
  // types for pddcp::value_type_t<T> testing
  InputType34, InputType35, InputType36, InputType37,
  // types for pddcp::has_value_type<T> testing
  InputType38, InputType39, InputType40, InputType41,
  // types for pddcp::innermost_value_type_t<T> testing
  InputType42, InputType43, InputType44, InputType45,
  // types for pddcp::innermost_value_type_depth<T> testing
  InputType46, InputType47, InputType48, InputType49
>;

}  // namespace

// typed test suite registration done inside pddcp testing namespace to allow
// unqualified TypeTraitsTest use in INSTANTIATE_TEST_SUITE_P
PDDCP_TESTING_NAMESPACE_BEGIN

INSTANTIATE_TYPED_TEST_SUITE_P(Types1, TypeTraitsTest, TypeTraitsTestTypes1);

PDDCP_TESTING_NAMESPACE_END
