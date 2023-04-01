/**
 * @file common.h
 * @author Derek Huang
 * @brief C++ header for commonly used constructs
 * @copyright MIT License
 */

#ifndef PDDCP_COMMON_H_
#define PDDCP_COMMON_H_

// concatenation macros
#define PDDCP_CONCAT_I(a, b) a ## b
#define PDDCP_CONCAT(a, b) PDDCP_CONCAT_I(a, b)

// stringification macros
#define PDDCP_STRINGIFY_I(x) #x
#define PDDCP_STRINGIFY(x) PDDCP_STRINGIFY_I(x)

/**
 * Stringify a token with a user-provided prefix.
 *
 * @param p Prefix char, e.g. `L` for wide string, `u` for `char16_t`
 * @param x Token to stringify
 */
#define PDDCP_XSTRINGIFY_I(p, x) PDDCP_CONCAT(p, PDDCP_STRINGIFY(x))

#endif  // PDDCP_COMMON_H_
