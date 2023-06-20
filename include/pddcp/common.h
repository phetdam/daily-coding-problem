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
 * Create an initializer list out from variadic arguments.
 *
 * @param ... Arguments for the initializer list
 */
#define PDDCP_INIT_LIST(...) {__VA_ARGS__}

/**
 * Stringify a token with a user-provided prefix.
 *
 * @param p Prefix char, e.g. `L` for wide string, `u` for `char16_t`
 * @param x Token to stringify
 */
#define PDDCP_XSTRINGIFY(p, x) PDDCP_CONCAT(p, PDDCP_STRINGIFY(x))

/**
 * C++ version compatibility macro.
 *
 * Allows correct C++ long integer version reporting for MSVC without needing
 * to compile with `/Zc:__cplusplus`, as it is 199711L by default.
 */
#ifdef _MSC_VER
#define PDDCP_CPLUSPLUS _MSVC_LANG
#else
#define PDDCP_CPLUSPLUS __cplusplus
#endif  // _MSC_VER

/**
 * Indicate if we are compiling under C++20 or not.
 *
 * Always having the macro defined allows use in normal or constexpr ifs.
 */
#if PDDCP_CPLUSPLUS >= 202002L
#define PDDCP_HAS_CPP20 1
#else
#define PDDCP_HAS_CPP20 0
#endif  // PDDCP_CPLUSPLUS < 202002L

/**
 * Enable some block of code when compiling with C++20.
 *
 * Use of variadic args is to allow chunks that contain commas.
 *
 * @param ... Statements to enable when compiling with C++20
 */
#if PDDCP_HAS_CPP20
#define PDDCP_CPP20_ENABLE(...) __VA_ARGS__
#else
#define PDDCP_CPP20_ENABLE(...)
#endif  // !PDDCP_HAS_CPP20

#endif  // PDDCP_COMMON_H_
