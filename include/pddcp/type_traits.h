/**
 * @file type_traits.h
 * @author Derek Huang
 * @brief C++ header providing some <type_traits> extensions
 * @copyright MIT License
 */

#ifndef PDDCP_TYPE_TRAITS_H_
#define PDDCP_TYPE_TRAITS_H_

#include <tuple>
#include <type_traits>

namespace pddcp {

/**
 * Class template for determining if a type is a homogenous pair.
 *
 * That is, if the type is like a `std::pair<T, T>`.
 *
 * @tparam T type
 */
template <typename T>
struct is_homogenous_pair : std::bool_constant<
  std::tuple_size_v<T> == 2 &&
  std::is_same_v<std::tuple_element_t<0, T>, std::tuple_element_t<1, T>>> {};

/**
 * Helper for `is_homogenous_pair` to get the constexpr bool value.
 */
template <typename T>
inline constexpr bool is_homogenous_pair_v = is_homogenous_pair<T>::value;

}  // namespace pddcp

#endif  // PDDCP_TYPE_TRAITS_H_
