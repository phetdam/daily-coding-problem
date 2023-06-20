/**
 * @file type_traits_test_3.cc
 * @author Derek Huang
 * @brief type_traits.h unit tests 3
 * @copyright MIT License
 */

#include "pddcp/type_traits.h"

#include <type_traits>
#include <utility>

#include <gtest/gtest.h>

#include "pddcp/common.h"
#include "pddcp/testing/type_traits_test.h"

namespace {

/**
 * Traits type helper for using `pddcp::literal_char_t<T>`.
 *
 * Solves an issue similar to what `value_type_t_helper<T, U>` solves.
 *
 * @tparam T Input type
 * @tparam U Expected `pddcp::literal_char_t<T>` type
 */
template <typename T, typename U>
struct literal_char_t_helper {
  static inline constexpr bool value = std::is_same_v<
    pddcp::literal_char_t<T>, U
  >;
};

/**
 * Helper for `pddcp::literal_char_t<T>` input creation.
 *
 * @tparam T Input type
 * @tparam U Expected `pddcp::literal_char_t<T>` type
 */
template <typename T, typename U>
using literal_char_t_input = std::pair<
  literal_char_t_helper<T, U>, std::true_type
>;

// types for pddcp::literal_char_t<T> testing
using InputType1 = literal_char_t_input<decltype(""), char>;
using InputType2 = literal_char_t_input<decltype(L""), wchar_t>;
using InputType3 = literal_char_t_input<
  decltype(u8""), PDDCP_CPP20_CONDITIONAL(char8_t, char)
>;
using InputType4 = literal_char_t_input<decltype(u""), char16_t>;
using InputType5 = literal_char_t_input<decltype(U""), char32_t>;
using InputType6 = literal_char_t_input<int, void>;
using InputType7 = literal_char_t_input<double[40], void>;
using InputType8 = literal_char_t_input<const char[90], void>;
using InputType9 = literal_char_t_input<const wchar_t*, void>;

using TypeTraitsTestTypes3 = ::testing::Types<
  // types for pddcp::literal_char_t<T> testing
  InputType1,
  InputType2,
  InputType3,
  InputType4,
  InputType5,
  InputType6,
  InputType7,
  InputType8,
  InputType9
>;

}  // namespace

// typed test suite registration done inside pddcp testing namespace to allow
// unqualified TypeTraitsTest use in INSTANTIATE_TEST_SUITE_P
PDDCP_TESTING_NAMESPACE_BEGIN

INSTANTIATE_TYPED_TEST_SUITE_P(Types3, TypeTraitsTest, TypeTraitsTestTypes3);

PDDCP_TESTING_NAMESPACE_END
