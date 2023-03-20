/**
 * @file xorll/xorll.h
 * @author Derek Huang
 * @brief C XOR linked list implementation
 * @copyright MIT License
 *
 * Declares the struct and functions for operating on an XOR linked list that
 * can only take doubles for simplicity, since this is an interview question.
 *
 * Changelog:
 *
 * 2022-12-23
 *  Modernization. I have become a much better C/C++ programmer since. File has
 *  been moved to the include directory, some functions are static inline, and
 *  overall the coding style and conventions have been fixed quite a bit.
 *
 * 2019-06-08
 *  Initial creation.
 */

#ifndef PDDCP_XORLL_XORLL_H_
#define PDDCP_XORLL_XORLL_H_

#include <stddef.h>
#include <stdlib.h>

// stringification macros
#define PDDCP_XORLL_STRINGIFY_I(x) #x
#define PDDCP_XORLL_STRINGIFY(x) PDDCP_XORLL_STRINGIFY_I(x)

// DLL import/export for Windows
#ifdef _WIN32
#ifdef PDDCP_XORLL_BUILD_DLL
#define PDDCP_XORLL_PUBLIC __declspec(dllexport)
#else
#define PDDCP_XORLL_PUBLIC __declspec(dllimport)
#endif  // PDDCP_XORLL_BUILD_DLL
#else
#define PDDCP_XORLL_PUBLIC
#endif  // _WIN32

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
 * XOR node.
 *
 * The `both` field contains the XOR of the previous and next nodes' location.
 */
// MSVC warns that 4 bytes padding is added after pointer
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4820)
#endif  // _MSC_VER
typedef struct pddcp_xor_node {
  double data;
  struct pddcp_xor_node *both;
} pddcp_xor_node;
#ifdef _MSC_VER
#pragma warning (pop)
#endif  // _MSC_VER

/**
 * XOR doubly linked list wrapper.
 *
 * Has pointers to the head and tail of the list and tracks the node count.
 */
typedef struct {
  pddcp_xor_node *head, *tail;
  size_t n_nodes;
} pddcp_xorll;

/**
 * Error code enum.
 */
typedef enum {
  PDDCP_XORLL_OK,
  PDDCP_XORLL_ERR_POINTER_NULL,
  PDDCP_XORLL_ERR_MALLOC_NULL,
  PDDCP_XORLL_ERR_OUT_OF_BOUNDS
} pddcp_xorll_error;

/**
 * Create a new XOR linked list.
 *
 * If `malloc` errors, `PDDCP_XORLL_ERR_MALLOC_NULL` is returned.
 */
static inline pddcp_xorll *
pddcp_xorll_alloc(void)
{
  pddcp_xorll *xll = malloc(sizeof *xll);
  if (!xll)
    return NULL;
  xll->head = xll->tail = NULL;
  xll->n_nodes = 0;
  return xll;
}

/**
 * Create a new XOR linked list node.
 *
 * If `malloc` errors `NULL` is returned.
 *
 * @param value New node value
 */
static inline pddcp_xor_node *
pddcp_xor_node_alloc(double value)
{
  pddcp_xor_node *xn = malloc(sizeof *xn);
  if (!xn)
    return NULL;
  xn->both = NULL;
  xn->data = value;
  return xn;
}

/**
 * Append a new XOR linked list node.
 *
 * @param xll Original XOR linked list
 * @param value New node value
 * @returns `PDDCP_XORLL_OK` on success, `PDDCP_XORLL_ERR_*` otherwise.
 */
PDDCP_XORLL_PUBLIC
pddcp_xorll_error
pddcp_xorll_append(pddcp_xorll *xll, double value);

/**
 * Returns the specified node in linked list.
 *
 * If the list is `NULL` or contains less than k + 1 nodes, returns an error.
 *
 * @param xll Original XOR linked list
 * @param k Node index
 * @param out Address to the `pddcp_xor_node *` we write the result to
 */
PDDCP_XORLL_PUBLIC
pddcp_xorll_error
pddcp_xorll_get(pddcp_xorll *xll, size_t k, pddcp_xor_node **out);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  /* PDDCP_XORLL_XORLL_H_ */
