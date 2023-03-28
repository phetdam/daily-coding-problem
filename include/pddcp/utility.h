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
auto swap_adjacent_bits(T value)
{
  static_assert(std::is_integral_v<T>);
  return
    ((value & alternating_bitmasks<T>::odd_mask) << 1) |
    ((value & alternating_bitmasks<T>::even_mask) >> 1);
}

}  // namespace pddcp

#endif  // PDDCP_UTILITY_H_
