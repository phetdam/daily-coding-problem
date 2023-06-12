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

/**
 * Check if a type is iterable.
 *
 * An iterable type `T` will have `begin()` and `end()` members.
 *
 * @tparam T type
 */
template <typename T, typename = void, typename = void>
struct is_iterable : std::false_type {};

/**
 * True specialization for when `T` is iterable.
 *
 * @tparam T type
 */
template <typename T>
struct is_iterable<
  T,
  std::void_t<decltype(std::declval<T>().begin())>,
  std::void_t<decltype(std::declval<T>().end())>> : std::true_type {};

/**
 * Boolean helper to check if a type is iterable.
 *
 * @tparam T type
 */
template <typename T>
inline constexpr bool is_iterable_v = is_iterable<T>::value;

/**
 * Get the `value_type` type member of a type, `void` if no member exists.
 *
 * Useful in template metaprogramming when `T` may not have the `value_type`
 * type member, as otherwise using `typename T::value_type` will cause a
 * compile error if used on types without the `value_type` type member.
 *
 * @tparam T type
 */
template <typename T, typename = void>
struct value_type { using type = void; };

/**
 * Specialization providing `value_type` for types that have the type member.
 *
 * @tparam T type
 */
template <typename T>
struct value_type<T, std::void_t<typename T::value_type>> {
  using type = typename T::value_type;
};

/**
 * Helper type for the `value_type` type member of a type.
 *
 * If the type has no `value_type` type member, the helper type is `void`.
 *
 * @tparam T type
 */
template <typename T>
using value_type_t = typename value_type<T>::type;

/**
 * Check if a type has a `value_type` type member.
 *
 * Use `pddcp::is_iterable<T>` instead to check if a type is iterable.
 *
 * @note We could have implemented `has_value_type<T>` as a subclass of
 *  `std::bool_constant<!std::is_same_v<void, pddcp::value_type_t<T>>>;
 *
 * @tparam T type
 */
template <typename T, typename = void>
struct has_value_type : std::false_type {};

/**
 * True specialization for when `T` contains a `value_type` type member.
 *
 * @tparam T type
 */
template <typename T>
struct has_value_type<T, std::void_t<typename T::value_type>>
  : std::true_type {};

/**
 * Boolean helper to check if a type has a `value_type` type member.
 *
 * @tparam T type
 */
template <typename T>
inline constexpr bool has_value_type_v = has_value_type<T>::value;

/**
 * Get the innermost `value_type` member of a type, `void` if no member exists.
 *
 * The `type` member is an alias for the innermost `value_type` type member or
 * `void` if the type has no `value_type` member while the `depth` member gives
 * how many `value_type` members had to be iterated through to get `type`.
 *
 * For example, given `std::vector<std::vector<std::string>>` as the type, then
 * `type` would be `char`, the `value_type` of the `std::string`, and `depth`
 * would be `3`, as we iterate through 3 `value_type` members.
 *
 * @tparam T type
 */
template <typename T>
struct innermost_value_type {
  using type = std::conditional_t<
    // base case: T has no value_type type member
    !has_value_type_v<T>,
    void,
    std::conditional_t<
      // base case: T has value_type without its own value_type type member
      !has_value_type_v<value_type_t<T>>,
      value_type_t<T>,
      // otherwise, recurse into value_type of T
      typename innermost_value_type<value_type_t<T>>::type
    >
  >;
  static inline constexpr std::size_t depth = []
  {
    // we include the "u" suffix explicitly to suppress MSVC C4365 warning
    if constexpr (!has_value_type_v<T>)
      return 0u;
    else
      return 1u + innermost_value_type<value_type_t<T>>::depth;
  }();
};

/**
 * `innermost_value_type<T>` specialization for `void`.
 *
 * The `void` specialization makes `innermost_value_type<void>` a complete
 * type, as otherwise no members will be accessible.
 */
template <>
struct innermost_value_type<void> {
  using type = void;
  static inline constexpr std::size_t depth = 0;
};

/**
 * Helper type for the innermost `value_type` member of a type.
 *
 * If the type has no `value_type` type member, the helper type is `void`.
 *
 * @tparam T
 */
template <typename T>
using innermost_value_type_t = typename innermost_value_type<T>::type;

/**
 * Helper for the depth of the innermost `value_type` member of a type.
 *
 * @tparam T
 */
template <typename T>
inline constexpr std::size_t
innermost_value_type_depth = innermost_value_type<T>::depth;

/**
 * Check if a type has a container-like `push_back` member.
 *
 * A suitable type should have a `value_type` type member and either or both of
 * `push_back(const value_type&)`, `push_back(value_type&&)` overloads.
 *
 * @tparam T type
 */
template <typename T, typename = void>
struct is_push_back_container : std::false_type {};

/**
 * True specialization for a type with a container-like `push_back` member.
 *
 * @tparam T type
 */
template <typename T>
struct is_push_back_container<
  T,
  std::void_t<
    decltype(
      std::declval<T>().push_back(std::declval<typename T::value_type>()))>
> : std::true_type {};

/**
 * Boolean helper to check if a type has a container-like `push_back` member.
 *
 * @tparam T
 */
template <typename T>
inline constexpr bool
is_push_back_container_v = is_push_back_container<T>::value;

/**
 * Check if a type has a container-like `emplace_back` member.
 *
 * A suitable type should have a `value_type` type member and either or both of
 * `emplace_back(const avlue_type&)`, `emplace_back(value_type&&)` overloads.
 *
 * @tparam T type
 */
template <typename T, typename = void>
struct is_emplace_back_container : std::false_type {};

/**
 * True specialization for a type with a container-like `emplace_back` member.
 *
 * @tparam T type
 */
template <typename T>
struct is_emplace_back_container<
  T,
  std::void_t<
    decltype(
      std::declval<T>().emplace_back(std::declval<typename T::value_type>()))>
> : std::true_type {};

/**
 * Boolean helper to check if a type has a container-like `emplace_back` member.
 *
 * @tparam T type
 */
template <typename T>
inline constexpr bool
is_emplace_back_container_v = is_emplace_back_container<T>::value;

/**
 * Get the `iterator` type member of a type, `void` if no member exists.
 *
 * Useful in template metaprogramming when `T` may not have the `iterator` type
 * member, as otherwise using `typename T::iterator` will cause a compile error
 * if used on types without the `iterator` type member.
 *
 * @tparam T type
 */
template <typename T, typename = void>
struct iterator { using type = void; };

/**
 * Specialization providing `iterator` for types that have the type member.
 *
 * @tparam T type
 */
template <typename T>
struct iterator<T, std::void_t<typename T::iterator>> {
  using type = typename T::iterator;
};

/**
 * Helper type for the `iterator` type member of a type.
 *
 * If the type has no `iterator` type member, the helper type is `void`.
 *
 * @tparam T type
 */
template <typename T>
using iterator_t = typename iterator<T>::type;

/**
 * Check if a type has an `iterator` type member.
 *
 * Use `pddcp::is_iterable<T>` instead to check if a type is iterable, which
 * usually also implies that `T` has an `iterator` type member.
 *
 * @tparam T type
 */
template <typename T, typename = void>
struct has_iterator : std::false_type {};

/**
 * True specialization for a type with an `iterator` type member.
 *
 * @tparam T type
 */
template <typename T>
struct has_iterator<T, std::void_t<typename T::iterator>> : std::true_type {};

/**
 * Boolean helper to check if a type has an `iterator` type member.
 *
 * @tparam T type
 */
template <typename T>
inline constexpr bool has_iterator_v = has_iterator<T>::value;

/**
 * Check if a type has a `const_iterator` type member.
 *
 * Use `pddcp::is_iterable<T>` instead ot check if a type is iterable, which
 * for STL containers also implies that `T` has a `const_iterator` type member.
 *
 * @tparam T type
 */
template <typename T, typename = void>
struct has_const_iterator : std::false_type {};

/**
 * True specialization for a type with a `const_iterator` type member.
 *
 * @tparam T type
 */
template <typename T>
struct has_const_iterator<T, std::void_t<typename T::const_iterator>>
  : std::true_type {};

/**
 * Boolean helper to check if a type has a `const_iterator` type member.
 *
 * @tparam T type
 */
template <typename T>
inline constexpr bool has_const_iterator_v = has_const_iterator<T>::value;

}  // namespace pddcp

#endif  // PDDCP_TYPE_TRAITS_H_
