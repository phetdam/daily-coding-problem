/**
 * @file warnings.h
 * @author Derek Huang
 * @brief C/C++ header for warnings helpers
 * @copyright MIT License
 */

#ifndef PDDCP_WARNINGS_H_
#define PDDCP_WARNINGS_H_

#include "pddcp/common.h"

// macros for using _Pragma without needing to explicitly quote
#define PDDCP_PRAGMA_I(x) _Pragma(#x)
#define PDDCP_PRAGMA(x) PDDCP_PRAGMA_I(x)

// helper macros for disabling and re-enabling MSVC warnings
#ifdef _MSC_VER
/**
 * Push warning state.
 */
#define PDDCP_MSVC_WARNING_PUSH() __pragma(warning(push))

/**
 * Disable specified MSVC warnings.
 *
 * @param wnos Warning number(s), e.g. 4242
 */
#define PDDCP_MSVC_WARNING_DISABLE(wnos) __pragma(warning(disable: wnos))

/**
 * Pop warning state.
 */
#define PDDCP_MSVC_WARNING_POP() __pragma(warning(pop))
#else
#define PDDCP_MSVC_WARNING_PUSH()
#define PDDCP_MSVC_WARNING_DISABLE(wnos)
#define PDDCP_MSVC_WARNING_POP()
#endif  // _MSC_VER

// helper macros for disabling and re-enabling GCC/Clang warnings
#ifdef __GNUC__
/**
 * Push warning state.
 */
#define PDDCP_GNU_WARNING_PUSH() PDDCP_PRAGMA(GCC diagnostic push)

/**
 * Disable specified GCC/Clang warning.
 *
 * @param wname GCC/Clang warning name without -W, e.g. self-move, narrowing
 */
#define PDDCP_GNU_WARNING_DISABLE(wname) \
  PDDCP_PRAGMA(GCC diagnostic ignored PDDCP_STRINGIFY(PDDCP_CONCAT(-W, wname)))

/**
 * Pop warning state.
 */
#define PDDCP_GNU_WARNING_POP() PDDCP_PRAGMA(GCC diagnostic pop)
#else
#define PDDCP_GNU_WARNING_PUSH()
#define PDDCP_GNU_WARNING_DISABLE(wname)
#define PDDCP_GNU_WARNING_POP()
#endif  // __GNUC__

// helper macros for disabling and re-enabling Clang warnings. these are useful
// when there are Clang-specific warnings that have no GCC equivalent.
#ifdef __clang__
/**
 * Push warning state.
 */
#define PDDCP_CLANG_WARNING_PUSH() PDDCP_PRAGMA(clang diagnostic push)

/**
 * Disable specified Clang warning.
 *
 * @param wname Clang warning name without -W, e.g. unused-lambda-capture
 */
#define PDDCP_CLANG_WARNING_DISABLE(wname) \
  PDDCP_PRAGMA(clang diagnostic ignored PDDCP_STRINGIFY(PDDCP_CONCAT(-W, wname)))

/**
 * Pop warning state.
 */
#define PDDCP_CLANG_WARNING_POP() PDDCP_PRAGMA(clang diagnostic pop)
#else
#define PDDCP_CLANG_WARNING_PUSH()
#define PDDCP_CLANG_WARNING_DISABLE(wname)
#define PDDCP_CLANG_WARNING_POP()
#endif  // __clang__

#endif  // PDDCP_WARNINGS_H_
