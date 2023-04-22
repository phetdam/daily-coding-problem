/**
 * @file enums.h
 * @author Derek Huang
 * @brief C++ header for common enums
 * @copyright MIT License
 */

#ifndef PDDCP_ENUMS_H_
#define PDDCP_ENUMS_H_

#include <type_traits>

#include "pddcp/common.h"

namespace pddcp {

/**
 * Macro for enum class helper types.
 *
 * @param enum_type_ Enum class type
 */
#define PDDCP_ENUM_CLASS_HELPER_TYPES(enum_type_) \
  using enum_type = enum_type_; \
  using value_type = std::underlying_type_t<enum_type>

/**
 * Defines a binary operator for an enum intended for use as a bitmask.
 *
 * @param enum_type Enum type
 * @param op Binary bitwise operator, e.g. &, |, ^
 */
#define PDDCP_BITWISE_ENUM_BINARY_OPERATOR(enum_type, op) \
  inline constexpr auto operator op (enum_type a, enum_type b) \
  { \
    using value_type = std::underlying_type_t<enum_type>; \
    return enum_type{static_cast<value_type>(a) op static_cast<value_type>(b)}; \
  }

/**
 * Defines a binary update operator for an enum intended for use as a bitmask.
 *
 * @param enum_type Enum type
 * @param op Binary bitwise operator, e.g. &, |, ^
 */
#define PDDCP_BITWISE_ENUM_UPDATE_OPERATOR(enum_type, op) \
  inline enum_type& operator PDDCP_CONCAT(op, =)( \
    enum_type& target, enum_type incoming) \
  { \
    target = target op incoming; \
    return target; \
  }

/**
 * Enum class for describing algorithm execution modes.
 *
 * If there are multiple implementations of an algorithm, this can be used in
 * a delegation wrapper to determine which implementation to use. With the
 * provided operators and `flags_contain` implementation, members can be used
 * together as if these were masking bits together.
 */
enum class execution : unsigned int {
  unassigned = 0x0,
  sync = 0x1,
  async = 0x2,
  recursive = 0x4,
  dynamic = 0x8
};

// define bitwise operators for execution enum
PDDCP_BITWISE_ENUM_BINARY_OPERATOR(execution, &)
PDDCP_BITWISE_ENUM_BINARY_OPERATOR(execution, |)
PDDCP_BITWISE_ENUM_BINARY_OPERATOR(execution, ^)

/**
 * Unary bitwise negation operator for the `execution` enum.
 *
 * @param a Value to bitweise negate
 */
inline constexpr auto operator~(execution a)
{
  using value_type = std::underlying_type_t<decltype(a)>;
  return execution{~static_cast<value_type>(a)};
}

// define bitwise update operators for execution enum
PDDCP_BITWISE_ENUM_UPDATE_OPERATOR(execution, &)
PDDCP_BITWISE_ENUM_UPDATE_OPERATOR(execution, |)
PDDCP_BITWISE_ENUM_UPDATE_OPERATOR(execution, ^)

/**
 * Check that the flags contain the specified target flag.
 *
 * @param flags Flags
 * @param target_flag Target flag to check membership for
 */
inline constexpr bool flags_contain(execution flags, execution target_flag)
{
  if ((flags & target_flag) == target_flag)
    return true;
  return false;
}

}  // namespace pddcp

#endif  // PDDCP_ENUMS_H_
