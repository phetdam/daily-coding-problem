/**
 * @file omp.h
 * @author Derek Huang
 * @brief C/C++ header for OpenMP helpers
 */

#ifndef PDDCP_OMP_H_
#define PDDCP_OMP_H_

// macro indicating that we're using MSVC's OpenMP, which is stuck at 2.0
#if defined(_OPENMP) && defined(_MSC_VER)
#define PDDCP_OMP_MSVC
#endif  // !defined(_OPENMP) || !defined(_MSC_VER)

// macros for disabling and reenabling MSVC signed/unsigned mismatch warnings
// when indexing matrices when PDDCP_OMP_MSVC is defined
#ifdef PDDCP_OMP_MSVC
#define PDDCP_OMP_MSVC_SIGN_MISMATCH_DISABLE() \
  _Pragma("warning (push)") \
  _Pragma("warning (disable: 4365)")
#define PDDCP_OMP_MSVC_SIGN_MISMATCH_ENABLE() _Pragma("warning (pop)")
#else
#define PDDCP_OMP_MSVC_SIGN_MISMATCH_DISABLE()
#define PDDCP_OMP_MSVC_SIGN_MISMATCH_ENABLE()
#endif  // PDDCP_OMP_MSVC

// macro for OpenMP parallel for loop annotation
#ifdef _OPENMP
#ifdef _MSC_VER
// must use MSVC __pragma here instead of _Pragma otherwise you get an internal
// compiler error. still an issue in Visual Studio 2022
#define PDDCP_OMP_PARALLEL_FOR __pragma(omp parallel for)
#else
#define PDDCP_OMP_PARALLEL_FOR _Pragma("omp parallel for")
#endif  // _MSC_VER
#else
#define PDDCP_OMP_PARALLEL_FOR
#endif  // _OPENMP

#endif  // PDDCP_OMP_H_
