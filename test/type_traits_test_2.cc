/**
 * @file type_traits_test_2.cc
 * @author Derek Huang
 * @brief type_traits.h unit tests 2
 * @copyright MIT License
 */

#include "pddcp/type_traits.h"

#include <cstdint>
#include <deque>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include <gtest/gtest.h>

#include "pddcp/warnings.h"
#include "pddcp/testing/common.h"
#include "pddcp/testing/type_traits_test.h"

namespace {

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
PDDCP_CLANG_WARNING_PUSH()
PDDCP_CLANG_WARNING_DISABLE(return-std-move)
  auto push_back(T&& value) { return value; }
PDDCP_CLANG_WARNING_POP()
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
using InputType1 = is_push_back_container_input<std::vector<int>, true>;
using InputType2 = is_push_back_container_input<int, false>;
using InputType3 = is_push_back_container_input<
  std::unordered_map<std::string, std::size_t>, false
>;
using InputType4 = is_push_back_container_input<
  push_back_container<std::vector<std::string>>, true
>;
using InputType5 = is_push_back_container_input<
  bad_push_back_container_1<std::unordered_map<std::string, int>>, false
>;
using InputType6 = is_push_back_container_input<
  bad_push_back_container_2<std::vector<int>>, false
>;
using InputType7 = is_push_back_container_input<std::deque<int>, true>;

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
using InputType8 = is_emplace_back_container_input<std::vector<double>, true>;
using InputType9 = is_emplace_back_container_input<double, false>;
using InputType10 = is_emplace_back_container_input<
  emplace_back_container<std::vector<std::string>>, true
>;
using InputType11 = is_emplace_back_container_input<
  bad_emplace_back_container_1<std::string>, false
>;
using InputType12 = is_emplace_back_container_input<
  std::deque<std::vector<double>>, true
>;
using InputType13 = is_emplace_back_container_input<
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
using InputType14 = iterator_t_input<
  std::vector<double>, typename std::vector<double>::iterator
>;
using InputType15 = iterator_t_input<
  std::wstring, typename std::wstring::iterator
>;
using InputType16 = iterator_t_input<double[40], void>;
using InputType17 = iterator_t_input<
  std::deque<std::string>, typename std::deque<std::string>::iterator
>;
using InputType18 = iterator_t_input<const char*, void>;

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
using InputType19 = has_iterator_input<std::vector<double>, true>;
using InputType20 = has_iterator_input<double[20], false>;
using InputType21 = has_iterator_input<std::string, true>;
using InputType22 = has_iterator_input<int, false>;

/**
 * Traits type helper for using `pddcp::const_iterator_t<T>`.
 *
 * Solves an issue similar to what `value_type_t_helper<T, U>` solves.
 *
 * @tparam T Input type
 * @tparam U Expected `pddcp::const_iterator_t<T>` type
 */
template <typename T, typename U>
struct const_iterator_t_helper {
  static inline constexpr bool value = std::is_same_v<
    pddcp::const_iterator_t<T>, U
  >;
};

/**
 * Helper for `pddcp::const_iterator_t<T>` input creation.
 *
 * @tparam T Input type
 * @tparam U Expected `pddcp::const_iterator_t<T>` type
 */
template <typename T, typename U>
using const_iterator_t_input = std::pair<
  const_iterator_t_helper<T, U>, std::true_type
>;

// types for pddcp::const_iterator_t<T> testing
using InputType23 = const_iterator_t_input<
  std::vector<int>, typename std::vector<int>::const_iterator
>;
using InputType24 = const_iterator_t_input<const char[88], void>;
using InputType25 = const_iterator_t_input<
  std::deque<std::string>, typename std::deque<std::string>::const_iterator
>;
using InputType26 = const_iterator_t_input<
  std::wstring, typename std::wstring::const_iterator
>;
using InputType27 = const_iterator_t_input<const double*, void>;

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
using InputType28 = has_const_iterator_input<std::vector<std::string>, true>;
using InputType29 = has_const_iterator_input<const std::string*, false>;
using InputType30 = has_const_iterator_input<std::wstring, true>;
using InputType31 = has_const_iterator_input<double, false>;

}  // namespace

// PDDCP_TYPE_TRAITS_TEST_CLASS definition and typed test suite registration
// need to happen within the pddcp testing namespace for proper name resolution
PDDCP_TESTING_NAMESPACE_BEGIN

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

// input types and type-parametrized test suite instantiation
using TypeTraitsTestTypes2 = ::testing::Types<
  // types for pddcp::is_push_back_container<T> testing
  InputType1,
  InputType2,
  InputType3,
  InputType4,
  InputType5,
  InputType6,
  InputType7,
  // types for pddcp::is_emplace_back_container<T> testing
  InputType8, InputType9, InputType10, InputType11, InputType12, InputType13,
  // types for pddcp::iterator_t<T> testing
  InputType14, InputType15, InputType16, InputType17, InputType18,
  // types for pddcp::has_iterator<T> testing
  InputType19, InputType20, InputType21, InputType22,
  // types for pddcp::const_iterator_t<T> testing
  InputType23, InputType24, InputType25, InputType26, InputType27,
  // types for pddcp::has_const_iterator<T> testing
  InputType28, InputType29, InputType30, InputType31
>;
INSTANTIATE_TYPED_TEST_SUITE_P(Types2, TypeTraitsTest, TypeTraitsTestTypes2);

PDDCP_TESTING_NAMESPACE_END
