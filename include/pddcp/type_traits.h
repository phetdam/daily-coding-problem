/**
 * @file type_traits.h
 * @author Derek Huang
 * @brief C++ header providing some <type_traits> extensions
 * @copyright MIT License
 */

#ifndef PDDCP_TYPE_TRAITS_H_
#define PDDCP_TYPE_TRAITS_H_

#include <cstdint>
#include <tuple>
#include <type_traits>

namespace pddcp {

/**
 * Class template for checking if the first `N` types of a tuple are the same.
 *
 * @tparam N Number of types to check
 * @tparam T Tuple-like type with at least `N` types
 */
template <std::size_t N, typename T>
struct is_leading_homogenous_tuple : std::bool_constant<
  is_leading_homogenous_tuple<N - 1, T>::value &&
  std::is_same_v<
    std::tuple_element_t<N - 1, T>, std::tuple_element_t<N - 2, T>>> {};

/**
 * Base case specialization for `is_leading_homogenous_tuple<N, T>`.
 *
 * @tparam T Tuple-like type with at least 1 type
 */
template <typename T>
struct is_leading_homogenous_tuple<1, T> : std::true_type {};

/**
 * Helper for `is_leading_homogenous_tuple` to get the constexpr bool value.
 *
 * @tparam N Number of types to check
 * @tparam T Tuple-like type with at least `N` types
 */
template <std::size_t N, typename T>
inline constexpr bool
is_leading_homogenous_tuple_v = is_leading_homogenous_tuple<N, T>::value;

/**
 * Class template for checking if all the types of a tuple are the same.
 *
 * @tparam N Number of types in tuple
 * @tparam T Tuple-like type with `N` types
 */
template <std::size_t N, typename T>
struct is_homogenous_tuple : std::bool_constant<
  std::tuple_size_v<T> == N && is_leading_homogenous_tuple_v<N, T>> {};

/**
 * Helper for `is_homogenous_type` to get the constexpr bool value.
 *
 * @tparam N Number of types in tuple
 * @tparam T Tuple-like type with `N` types
 */
template <std::size_t N, typename T>
inline constexpr bool is_homogenous_tuple_v = is_homogenous_tuple<N, T>::value;

/**
 * Class template for determining if a type is a homogenous pair.
 *
 * That is, if the type is like a `std::pair<T, T>`.
 *
 * @tparam T type
 */
template <typename T>
struct is_homogenous_pair : is_homogenous_tuple<2, T> {};

/**
 * Helper for `is_homogenous_pair` to get the constexpr bool value.
 */
template <typename T>
inline constexpr bool is_homogenous_pair_v = is_homogenous_pair<T>::value;

}  // namespace pddcp

#endif  // PDDCP_TYPE_TRAITS_H_
