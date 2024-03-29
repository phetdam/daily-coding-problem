/**
 * @file utility.h
 * @author Derek Huang
 * @brief C++ header for miscellaneous utilities
 * @copyright MIT License
 */

#ifndef PDDCP_UTILITY_H_
#define PDDCP_UTILITY_H_

#include <climits>
#include <cstdint>
#include <type_traits>

#include "pddcp/warnings.h"

namespace pddcp {

namespace detail {

/**
 * Class template impl providing even and odd bitmasks for a single byte.
 *
 * Bits are counted from least to most significant, i.e. right to left.
 *
 * @tparam I Number of bits in an even-bit byte
 * @tparam T member type whose size is 1 byte
 */
template <std::size_t I, typename T>
struct alternating_byte_bitmasks {
  static_assert(I > 0, "cannot be used with zero-bit bytes");
  static_assert(I % 2 == 0, "cannot be used with odd-bit bytes");
  // odd bit mask
  static inline constexpr T odd_mask{
    alternating_byte_bitmasks<2, T>::odd_mask +
    (alternating_byte_bitmasks<I - 2, T>::odd_mask << 2)
  };
  // even bit mask
  static inline constexpr T even_mask{
    alternating_byte_bitmasks<2, T>::even_mask +
    (alternating_byte_bitmasks<I - 2, T>::even_mask << 2)
  };
};

/**
 * `alternating_byte_bitmasks` specialization for 2-bit byte.
 *
 * @tparam T member type whose size is 1 byte
 */
template <typename T>
struct alternating_byte_bitmasks<2, T> {
  static inline constexpr T odd_mask = 0b01;
  static inline constexpr T even_mask = 0b10;
};

/**
 * Class template impl providing even and odd bitmasks for an integral type.
 *
 * Bits are counted from least to most significant, i.e. right to left.
 *
 * @tparam I Number of even-bit bytes in mask
 * @tparam T member type holding `I` bytes
 */
template <std::size_t I, typename T>
struct alternating_bitmasks {
  // odd bit mask
  static inline constexpr T odd_mask{
    alternating_bitmasks<1, T>::odd_mask +
    (alternating_bitmasks<I - 1, T>::odd_mask << CHAR_BIT)
  };
  // even bit mask
  static inline constexpr T even_mask{
    alternating_bitmasks<1, T>::even_mask +
    (alternating_bitmasks<I - 1, T>::even_mask << CHAR_BIT)
  };
};

/**
 * `alternating_bitmasks` specialization for a single byte.
 *
 * @tparam T member type holding at least 1 byte
 */
template <typename T>
struct alternating_bitmasks<1, T> : alternating_byte_bitmasks<CHAR_BIT, T> {};

}  // namespace detail

/**
 * Class template providing even and odd bitmasks for a single byte.
 *
 * Bits are counted from least to most significant, i.e. right to left.
 *
 * @tparam T member type whose size is 1 byte
 */
template <typename T>
struct alternating_byte_bitmasks
  : detail::alternating_byte_bitmasks<CHAR_BIT, T> {
  static_assert(sizeof(T) == 1, "sizeof(T) must equal 1");
};

/**
 * Class template providing even and odd bitmasks for an integral type.
 *
 * Bits are counted from least to most significant, i.e. right to left.
 *
 * @tparam T member type
 */
template <typename T>
struct alternating_bitmasks : detail::alternating_bitmasks<sizeof(T), T> {};

/**
 * Return a copy of an integral value with its even and odd bits swapped.
 *
 * Bits are counted from least to most significant, i.e. right to left.
 *
 * @param value Value whose even and odd bits are to be swapped
 */
template <typename T>
inline T swap_adjacent_bits(T value)
{
  static_assert(std::is_integral_v<T>);
// MSVC complains about signed/unsigned mismatch from int to T
PDDCP_MSVC_WARNING_PUSH()
PDDCP_MSVC_WARNING_DISABLE(4365)
  return
    ((value & alternating_bitmasks<T>::odd_mask) << 1) |
    ((value & alternating_bitmasks<T>::even_mask) >> 1);
PDDCP_MSVC_WARNING_POP()
}

/**
 * Impl macro for defining common container helper type aliases.
 *
 * Do not use directly, prefer using `PDDCP_CONTAINER_HELPER_TYPES`.
 *
 * @param type *Container* type
 */
#define PDDCP_CONTAINER_HELPER_TYPES_I(type) \
  using size_type = typename type::size_type; \
  using value_type = typename type::value_type

/**
 * Macro for defining common container helper type aliases.
 *
 * The `container_type` alias allows `type` to be a type member of another
 * type, i.e. so we could use `typename type_a::type_b` as a parameter.
 *
 * @param type *Container* type
 */
#define PDDCP_CONTAINER_HELPER_TYPES(type) \
  using container_type = type; \
  PDDCP_CONTAINER_HELPER_TYPES_I(container_type)

/**
 * Class template for an "indexed" type.
 *
 * Allows us to create a wrapper type around the same type `T` so as to make
 * unique types with the same `element_type`. One application is with the
 * Google Test `TYPED_TEST_SUITE`, as we can use identical input types `T` but
 * use `index_type<index_, T>` to create distinct types for distinct
 * specializations of the test fixture class template.
 *
 * @tparam I type index
 * @tparam T type
 */
template <std::size_t I, typename T>
struct indexed_type {
  using element_type = T;
  static inline constexpr decltype(I) index = I;
};

/**
 * Macro defining some useful indexed type helper type aliases.
 *
 * @param type `pddcp::indexed_type<I, T>` specialization
 */
#define PDDCP_INDEXED_TYPE_HELPER_TYPES(type) \
  using indexed_type = type; \
  using element_type = typename indexed_type::element_type

/**
 * Macro for defining some useful indexed helper type aliases for containers.
 *
 * @param type `pddcp::indexed_type<I, T>` specialization, `T` a *Container*
 */
#define PDDCP_INDEXED_TYPE_CONTAINER_HELPER_TYPES(type) \
  PDDCP_INDEXED_TYPE_HELPER_TYPES(type); \
  PDDCP_CONTAINER_HELPER_TYPES_I(element_type)

}  // namespace pddcp

#endif  // PDDCP_UTILITY_H_
