/**
 * @file warnings.h
 * @author Derek Huang
 * @brief C/C++ header for warnings helpers
 * @copyright MIT License
 */

#ifndef PDDCP_WARNINGS_H_
#define PDDCP_WARNINGS_H_

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
#define PDDCP_GNU_WARNING_PUSH() _Pragma("GCC diagnostic push")

/**
 * Disable specified GCC/Clang warning.
 *
 * @param wname GCC/Clang warning name without -W, e.g. self-move, narrowing
 */
#define PDDCP_GNU_WARNING_DISABLE(wname) \
  _Pragma("GCC diagnostic ignored \"-W" #wname "\"")

/**
 * Pop warning state.
 */
#define PDDCP_GNU_WARNING_POP() _Pragma("GCC diagnostic pop")
#else
#define PDDCP_GNU_WARNING_PUSH()
#define PDDCP_GNU_WARNING_DISABLE(wname)
#define PDDCP_GNU_WARNING_POP()
#endif  // __GNUC__

#endif  // PDDCP_WARNINGS_H_
