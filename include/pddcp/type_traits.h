/**
 * @file type_traits.h
 * @author Derek Huang
 * @brief C++ header providing some <type_traits> extensions
 * @copyright MIT License
 */

#ifndef PDDCP_TYPE_TRAITS_H_
#define PDDCP_TYPE_TRAITS_H_

#include <cstdint>
#include <functional>
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

/**
 * Base template checking if a type supports bitwise and.
 *
 * @tparam T type
 */
template <typename T, typename = void>
struct has_bitwise_and : std::is_integral<T> {};

/**
 * Specialization for types with user-defined `operator&`.
 *
 * @tparam T type
 */
template <typename T>
struct has_bitwise_and<
  T, std::void_t<decltype(operator&(std::declval<T>(), std::declval<T>()))>
> : std::true_type {};

/**
 * Base template checking if a type supports bitwise or.
 *
 * @tparam T type
 */
template <typename T, typename = void>
struct has_bitwise_or : std::is_integral<T> {};

/**
 * Specialization for types with user-defined `operator|`.
 *
 * @tparam T type
 */
template <typename T>
struct has_bitwise_or<
  T, std::void_t<decltype(operator|(std::declval<T>(), std::declval<T>()))>
> : std::true_type {};

/**
 * Base template checking if a type supports bitwise exclusive or.
 *
 * @tparam T type
 */
template <typename T, typename = void>
struct has_bitwise_xor : std::is_integral<T> {};

/**
 * Specialization for types with user-defined `operator^`.
 *
 * @tparam T type
 */
template <typename T>
struct has_bitwise_xor<
  T, std::void_t<decltype(operator^(std::declval<T>(), std::declval<T>()))>
> : std::true_type {};

/**
 * Base template checking if a type supports bitwise not.
 *
 * @tparam T type
 */
template <typename T, typename = void>
struct has_bitwise_not : std::is_integral<T> {};

/**
 * Specialization for types with user-defined `operator~`.
 *
 * @tparam T type
 */
template <typename T>
struct has_bitwise_not<T, std::void_t<decltype(operator~(std::declval<T>()))>>
  : std::true_type {};

/**
 * Boolean helper to check if a type supports bitwise and.
 *
 * @tparam T type
 */
template <typename T>
inline constexpr bool has_bitwise_and_v = has_bitwise_and<T>::value;

/**
 * Boolean helper to check if a type supports bitwise or.
 *
 * @tparam T type
 */
template <typename T>
inline constexpr bool has_bitwise_or_v = has_bitwise_or<T>::value;

/**
 * Boolean helper to check if a type supports bitwise exclusive or.
 *
 * @tparam T type
 */
template <typename T>
inline constexpr bool has_bitwise_xor_v = has_bitwise_xor<T>::value;

/**
 * Boolean helper to check if a type supports bitwise not.
 *
 * @tparam T type
 */
template <typename T>
inline constexpr bool has_bitwise_not_v = has_bitwise_not<T>::value;

/**
 * Checks if a type is a bitmask type.
 *
 * @note This does not fully meet the *BitmaskType* named requirements yet.
 *
 * @tparam T type
 */
template <typename T>
struct is_bitmask_type : std::bool_constant<
  has_bitwise_and_v<T> &&
  has_bitwise_or_v<T> &&
  has_bitwise_xor_v<T> &&
  has_bitwise_not_v<T>
> {};

/**
 * Boolean helper to check if a type is a bitmask type.
 *
 * @tparam T type
 */
template <typename T>
inline constexpr bool is_bitmask_type_v = is_bitmask_type<T>::value;

/**
 * Check if a type is an enum usable as a bitmask type.
 *
 * @note This does not make a distinction between scoped and unscoped enums.
 *
 * @tparam T type
 */
template <typename T>
struct is_bitmask_enum : std::bool_constant<
  std::is_enum_v<T> && is_bitmask_type_v<T>> {};

/**
 * Boolean helper to check if a type is a bitmask enum type.
 *
 * @tparam T type
*/
template <typename T>
inline constexpr bool is_bitmask_enum_v = is_bitmask_enum<T>::value;

/**
 * Check if a type `T` has a `std::hash<T>` specialization.
 *
 * @tparam T type
 */
template <typename T, typename = void>
struct is_std_hashable : std::false_type {};

/**
 * True specialization for when `T` has a `std::hash<T>` specialization.
 *
 * @tparam T type
 */
template <typename T>
struct is_std_hashable<
  T, std::void_t<decltype(std::hash<T>{}(std::declval<T>()))>
> : std::true_type {};

/**
 * Boolean helper to check if a type is a hashable type.
 *
 * @tparam T type
 */
template <typename T>
inline constexpr bool is_std_hashable_v = is_std_hashable<T>::value;

/**
 * Check if a type is a STL vector.
 *
 * @tparam T type
 */
template <typename T, typename = void, typename = void>
struct is_std_vector : std::false_type {};

/**
 * True specialization for when `T` is a STL vector.
 *
 * @tparam T type
 */
template <typename T>
struct is_std_vector<
  T,
  std::void_t<typename T::value_type>,
  std::void_t<typename T::allocator_type>
> : std::bool_constant<
  std::is_same_v<
    std::decay_t<T>,
    std::vector<typename T::value_type, typename T::allocator_type>>> {};

/**
 * Boolean helper to check if a type is a STL vector.
 *
 * @tparam T type
 */
template <typename T>
inline constexpr bool is_std_vector_v = is_std_vector<T>::value;

}  // namespace pddcp

#endif  // PDDCP_TYPE_TRAITS_H_
