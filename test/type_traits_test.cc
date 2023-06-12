/**
 * @file type_traits_test.cc
 * @author Derek Huang
 * @brief type_traits.h unit tests
 * @copyright MIT License
 */

#include "pddcp/type_traits.h"

#include <array>
#include <cstdint>
#include <deque>
#include <string>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "pddcp/enums.h"
#include "pddcp/warnings.h"

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

// we use type-parametrized testing since we exceeded the 50-type limit for the
// types that ::testing::Types<...> can support for typed tests
TYPED_TEST_SUITE_P(TypeTraitsTest);

/**
 * Test that a traits class works as expected.
 */
TYPED_TEST_P(TypeTraitsTest, TraitsTest)
{
  EXPECT_EQ(TestFixture::expected_, TestFixture::actual_);
}

// unlike typed tests, need to individually register each test
REGISTER_TYPED_TEST_SUITE_P(TypeTraitsTest, TraitsTest);

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
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType29);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType30);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType31);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType32);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType33);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType34);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType35);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType36);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType37);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType38);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType39);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType40);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType41);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType42);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType43);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType44);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType45);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType46);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType47);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType48);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType49);

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
INSTANTIATE_TYPED_TEST_SUITE_P(Types1, TypeTraitsTest, TypeTraitsTestTypes1);

/**
 * A class implementing a container-like `push_back`.
 *
 * @tparam T value type
 */
template <typename T>
class push_back_container {
public:
  using value_type = T;
  push_back_container([[maybe_unused]] const std::vector<T>& values) {}
  auto push_back(const T& value) { return value; }
// Clang correctly warns about value returned as copy, but it is intentional
#ifdef __clang__
PDDCP_GNU_WARNING_PUSH()
PDDCP_GNU_WARNING_DISABLE(return-std-move)
#endif  // __clang__
  auto push_back(T&& value) { return value; }
#ifdef __clang__
PDDCP_GNU_WARNING_POP()
#endif  // __clang__
};

/**
 * First class incorrectly implementing `push_back`.
 *
 * This one is missing the `value_type` type member.
 *
 * @tparam T value type
 */
template <typename T>
class bad_push_back_container_1 {
public:
  bad_push_back_container_1() {}
  auto push_back(const T& value) { return value; }
};

/**
 * Second class incorrectly implementing `push_back`.
 *
 * This one has incorrect `push_back` signatures.
 *
 * @tparam T value type
 */
template <typename T>
class bad_push_back_container_2 {
public:
  using value_type = T;
  bad_push_back_container_2() {}
  auto push_back() { return 1u; }
  auto push_back(const T& a, [[maybe_unused]] const T& b) { return a; }
};

/**
 * Helper for `pddcp::is_push_back_container<T>` input creation.
 *
 * @tparam T input type
 * @tparam truth Expected truth given by the traits type
 */
template <typename T, bool truth>
using is_push_back_container_input = std::pair<
  pddcp::is_push_back_container<T>, std::bool_constant<truth>
>;

// types for pddcp::is_push_back_container<T> testing
using InputType50 = is_push_back_container_input<std::vector<int>, true>;
using InputType51 = is_push_back_container_input<int, false>;
using InputType52 = is_push_back_container_input<
  std::unordered_map<std::string, std::size_t>, false
>;
using InputType53 = is_push_back_container_input<
  push_back_container<std::vector<std::string>>, true
>;
using InputType54 = is_push_back_container_input<
  bad_push_back_container_1<std::unordered_map<std::string, int>>, false
>;
using InputType55 = is_push_back_container_input<
  bad_push_back_container_2<std::vector<int>>, false
>;
using InputType56 = is_push_back_container_input<std::deque<int>, true>;

/**
 * A class implementing a container-like `emplace_back`.
 *
 * @tparam T value type
 */
template <typename T>
class emplace_back_container {
public:
  using value_type = T;
  emplace_back_container() {}
  auto emplace_back(const T& value) { return value; }
  auto emplace_back([[maybe_unused]] T&& value) { return 1; }
};

/**
 * First class incorrectly implementing `emplace_back`.
 *
 * This one is missing the `value_type` type member.
 *
 * @tparam T value type
 */
template <typename T>
class bad_emplace_back_container_1 {
public:
  bad_emplace_back_container_1() {}
  auto emplace_back(const T& value) { return value; }
};

/**
 * Second class incorrectly implementing `emplace_back`.
 *
 * This one has incorrect `emplace_back` signatures.
 *
 * @tparam T value type
 */
template <typename T>
class bad_emplace_back_container_2 {
public:
  using value_type = T;
  bad_emplace_back_container_2() {}
  auto emplace_back() const { return 0; }
  auto emplace_back(const T& a, [[maybe_unused]] const T& b) { return a; }
};

/**
 * Helper for `pddcp::is_emplace_back_container<T>` input creation.
 *
 * @tparam T input type
 * @tparam truth Expected truth given by the traits type
 */
template <typename T, bool truth>
using is_emplace_back_container_input = std::pair<
  pddcp::is_emplace_back_container<T>, std::bool_constant<truth>
>;

// types for pddcp::is_emplace_back_container<T> testing
using InputType57 = is_emplace_back_container_input<std::vector<double>, true>;
using InputType58 = is_emplace_back_container_input<double, false>;
using InputType59 = is_emplace_back_container_input<
  emplace_back_container<std::vector<std::string>>, true
>;
using InputType60 = is_emplace_back_container_input<
  bad_emplace_back_container_1<std::string>, false
>;
using InputType61 = is_emplace_back_container_input<
  std::deque<std::vector<double>>, true
>;
using InputType62 = is_emplace_back_container_input<
  std::unordered_map<std::string, std::size_t>, false
>;

/**
 * Traits type helper for using `pddcp::iterator_t<T>`.
 *
 * Solves an issue similar to what `value_type_t_helper<T, U>` solves.
 *
 * @tparam T Input type
 * @tparam U Expected `pddcp::iterator_t<T>` type
 */
template <typename T, typename U>
struct iterator_t_helper {
  static inline constexpr bool value = std::is_same_v<pddcp::iterator_t<T>, U>;
};

/**
 * Helper for `pddcp::iterator_t<T>` input creation.
 *
 * @tparam T Input type
 * @tparam U Expected `pddcp::iterator_t<T>` type
 */
template <typename T, typename U>
using iterator_t_input = std::pair<iterator_t_helper<T, U>, std::true_type>;

// types for pddcp::iterator_t<T> testing
using InputType63 = iterator_t_input<
  std::vector<double>, typename std::vector<double>::iterator
>;
using InputType64 = iterator_t_input<
  std::wstring, typename std::wstring::iterator
>;
using InputType65 = iterator_t_input<double[40], void>;
using InputType66 = iterator_t_input<
  std::deque<std::string>, typename std::deque<std::string>::iterator
>;
using InputType67 = iterator_t_input<const char*, void>;

/**
 * Helper for `pddcp::has_iterator<T>` input creation.
 *
 * @tparam T input type
 * @tparam truth Expected truth given by the traits type
 */
template <typename T, bool truth>
using has_iterator_input = std::pair<
  pddcp::has_iterator<T>, std::bool_constant<truth>
>;

// types for pddcp::has_iterator<T> testing
using InputType68 = has_iterator_input<std::vector<double>, true>;
using InputType69 = has_iterator_input<double[20], false>;
using InputType70 = has_iterator_input<std::string, true>;
using InputType71 = has_iterator_input<int, false>;

/**
 * Helper for `pddcp::has_const_iterator<T>` input creation.
 *
 * @tparam T input type
 * @tparam truth Expected truth given by the traits type
 */
template <typename T, bool truth>
using has_const_iterator_input = std::pair<
  pddcp::has_const_iterator<T>, std::bool_constant<truth>
>;

// types for pddcp::has_const_iterator<T> testing
using InputType72 = has_const_iterator_input<std::vector<std::string>, true>;
using InputType73 = has_const_iterator_input<const std::string*, false>;
using InputType74 = has_const_iterator_input<std::wstring, true>;
using InputType75 = has_const_iterator_input<double, false>;

// specialization creation using the input types
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType50);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType51);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType52);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType53);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType54);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType55);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType56);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType57);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType58);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType59);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType60);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType61);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType62);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType63);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType64);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType65);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType66);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType67);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType68);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType69);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType70);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType71);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType72);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType73);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType74);
PDDCP_TYPE_TRAITS_TEST_CLASS(InputType75);

// input types and type-parametrized test suite instantiation
using TypeTraitsTestTypes2 = ::testing::Types<
  // types for pddcp::is_push_back_container<T> testing
  InputType50,
  InputType51,
  InputType52,
  InputType53,
  InputType54,
  InputType55,
  InputType56,
  // types for pddcp::is_emplace_back_container<T> testing
  InputType57, InputType58, InputType59, InputType60, InputType61, InputType62,
  // types for pddcp::iterator_t<T> testing
  InputType63, InputType64, InputType65, InputType66, InputType67,
  // types for pddcp::has_iterator<T> testing
  InputType68, InputType69, InputType70, InputType71,
  // types for pddcp::has_const_iterator<T> testing
  InputType72, InputType73, InputType74, InputType75
>;
INSTANTIATE_TYPED_TEST_SUITE_P(Types2, TypeTraitsTest, TypeTraitsTestTypes2);

}  // namespace
