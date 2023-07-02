/**
 * @file asan.h
 * @author Derek Huang
 * @brief C/C++ header for AddressSanitizer helpers
 * @copyright MIT License
 */

#ifndef PDDCP_ASAN_H_
#define PDDCP_ASAN_H_

// 1 if compiled with AddressSanitizer, 0 otherwise. GCC/MSVC detect using the
// __SANITIZE_ADDRESS__ macro, Clang uses __has_feature(address_sanitizer)
#if defined(__SANITIZE_ADDRESS__)
#define PDDCP_HAS_ASAN 1
#elif defined(__has_feature)
#if __has_feature(address_sanitizer)
#define PDDCP_HAS_ASAN 1
#else
#define PDDCP_HAS_ASAN 0
#endif  // !__has_feature(address_sanitizer)
#else
#define PDDCP_HAS_ASAN 0
#endif  // !defined(__SANITIZE_ADDRESS__) &&
        // (!defined(__has_feature) || !__has_feature(address_sanitizer))

// function/object attribute to disable AddressSanitizer instrumentation
#if PDDCP_HAS_ASAN
// Clang defines __GNUC__ as well
#if defined(__clang__) || defined(__GNUC__)
#define PDDCP_NO_SANITIZE_ADDRESS __attribute__((no_sanitize_address))
#elif defined(_MSC_VER)
#define PDDCP_NO_SANITIZE_ADDRESS __declspec(no_sanitize_address)
#else
#define PDDCP_NO_SANITIZE_ADDRESS
#endif  // !defined(__GNUC__) && !defined(_MSC_VER)
#else
#define PDDCP_NO_SANITIZE_ADDRESS
#endif  // !PDDCP_HAS_ASAN

#endif  // PDDCP_ASAN_H_
