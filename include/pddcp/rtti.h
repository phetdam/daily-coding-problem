/**
 * @file rtti.h
 * @author Derek Huang
 * @brief C++ header for run-time type information helpers
 * @copyright MIT License
 */

#ifndef PDDCP_RTTI_H_
#define PDDCP_RTTI_H_

/**
 * Macro for checking that Itanium ABI is used by this compiler.
 */
#ifdef __has_include
#if __has_include(<cxxabi.h>)
#define PDDCP_ITANIUM_CXX_ABI
#endif  // !__has_include(<cxxabi.h>)
#endif  // __has_include

#ifdef PDDCP_ITANIUM_CXX_ABI
#include <cxxabi.h>

#include <cstddef>
#include <cstdlib>
#include <stdexcept>
#endif  // PDDCP_ITANIUM_CXX_ABI
#include <string>
#include <typeinfo>

/**
 * Unqualified name of the current function.
 */
#define PDDCP_FUNCTION_NAME __func__

/**
 * Pretty function name, fully-qualified and with signature.
 *
 * `PDDCP_FUNCTION_NAME` is used as the fallback.
 */
#if defined(__GNUC__) || defined(__clang__)  // Clang defines __GNUC__ too
#define PDDCP_PRETTY_FUNCTION_NAME __PRETTY_FUNCTION__
// check _MSC_VER second, as Clang also defines _MSC_VER
#elif defined(_MSC_VER)
#define PDDCP_PRETTY_FUNCTION_NAME __FUNCSIG__
#else
#define PDDCP_PRETTY_FUNCTION_NAME PDDCP_FUNCTION_NAME
#endif  // !defined(__GNUC__) && !defined(__clang__) && !defined(_MSC_VER)

namespace pddcp {

/**
 * Return the demangled name of a type from its mangled type name.
 *
 * The expected mangling format is dependent on the compiler ABI.
 *
 * @param name Mangled type name
 */
inline std::string demangled_name(const char* name)
{
// otherwise, any compiler that uses Itanium ABI needs to demangle
#ifdef PDDCP_ITANIUM_CXX_ABI
  int status;
  auto name_buf = abi::__cxa_demangle(name, nullptr, nullptr, &status);
  // on success, status is 0. we need to copy into a string and free the buffer
  if (!status) {
    std::string clean_name{name_buf};
    free(name_buf);
    return clean_name;
  }
  // otherwise, there's an error. free first, handle afterwards
  free(name_buf);
  if (status == -1)
    throw std::runtime_error{
      PDDCP_PRETTY_FUNCTION_NAME +
      std::string{": failed to allocate memory for demangled name"}
    };
  if (status == -2)
    throw std::runtime_error{
      PDDCP_PRETTY_FUNCTION_NAME + std::string{": "} + name +
      std::string{" is not a valid mangled type name"}
    };
  if (status == -3)
    throw std::runtime_error{
      PDDCP_PRETTY_FUNCTION_NAME +
      std::string{": abi::__cxa_demangle called with an invalid argument "}
    };
  // unreachable, abi::__cxa_demangle only returns -3, ... 0
  throw std::runtime_error{
    PDDCP_PRETTY_FUNCTION_NAME +
    std::string{": abi::__cxa_demangle returned unexpected status "} +
    std::to_string(status)
  };
// backup is to undecorated name. note that MSVC does not mangle the type
#else
  return name;
#endif  // PDDCP_ITANIUM_CXX_ABI
}

/**
 * Return the demangled name of a type from its mangled type name.
 *
 * The expected mangling format is dependent on the compiler ABI.
 *
 * @param name Mangled type name
 */
inline std::string demangled_name(const std::string& name)
{
  return demangled_name(name.c_str());
}

/**
 * Return the demangled name of a type from its type info object.
 *
 * @param info Type info object
 */
inline std::string demangled_name(const std::type_info& info)
{
  return demangled_name(info.name());
}

/**
 * Return a demangled type name from a type or an expression.
 *
 * @param expr Type or expressions
 * @returns `std::string` with the demangled name
 */
#define PDDCP_DEMANGLED_NAME(expr) pddcp::demangled_name(typeid(expr))

}  // namespace pddcp

#endif  // PDDCP_RTTI_H_
