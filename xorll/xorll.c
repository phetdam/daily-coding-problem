/**
 * @file xorll/xorll.c
 * @author Derek Huang
 * @brief C XOR linked list implementation
 * @copyright MIT License
 *
 * Implementation of a simple XOR linked list that can only take doubles for
 * simplicity since this is an interview question.
 *
 * Changelog:
 *
 * 2022-12-27
 *  Modernization. I have become a much better C/C++ programmer since.
 *
 * 2019-06-09
 *  Fixed a single piece of spacing. Hmm.
 *
 * 2019-06-08
 *  Initial creation. See xorll.py for the question statement. Added all the
 *  necessary append and get (by index) functions required by the question,
 *  as well as constructors for the xorll and xor_node.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "pddcp/xorll/xorll.h"

/**
 * Append a new XOR linked list node.
 *
 * @param xll Original XOR linked list
 * @param value New node value
 * @returns `PDDCP_XORLL_OK` on success, `PDDCP_XORLL_ERR_*` otherwise.
 */
PDDCP_XORLL_PUBLIC
pddcp_xorll_error
pddcp_xorll_append(pddcp_xorll *xll, double value)
{
  if (!xll)
    return PDDCP_XORLL_ERR_POINTER_NULL;
  // make new XOR node
  pddcp_xor_node *xn = pddcp_xor_node_alloc(value);
  if (!xn)
    return PDDCP_XORLL_ERR_MALLOC_NULL;
  // if list is empty, set both the head and tail pointer
  if (!xll->n_nodes) {
    xll->head = xll->tail = xn;
    xll->n_nodes = 1;
    return PDDCP_XORLL_OK;
  }
  // otherwise, we need to modify tail's both field. since a ^ 0 = a,
  // tail->both is just the address of tail's previous node (next is NULL). in
  // the case where xll->n_nodes == 1, head == tail, with head->both and
  // tail->both both NULL, so we just need to set xll->tail->both to xn.
  uintptr_t both = (uintptr_t) xll->tail->both ^ (uintptr_t) xn;
  xll->tail->both = (pddcp_xor_node *) both;
  // we set xn->both to be tail ^ 0, which is tail, and update tail and n_nodes
  xn->both = xll->tail;
  xll->tail = xn;
  xll->n_nodes++;
  return PDDCP_XORLL_OK;
}

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
pddcp_xorll_get(pddcp_xorll *xll, size_t k, pddcp_xor_node **out)
{
  if (!xll || !out)
    return PDDCP_XORLL_ERR_POINTER_NULL;
  if (k >= xll->n_nodes)
    return PDDCP_XORLL_ERR_OUT_OF_BOUNDS;
  // previous, current, old previous value of the node address
  pddcp_xor_node *prev, *cur, *old_prev;
  prev = NULL;
  cur = xll->head;
  // loop until we reach the kth node
  size_t i = 0;
  while (cur && i++ < k) {
    // save the value of prev in old_prev + update prev (cur will change soon)
    old_prev = prev;
    prev = cur;
    // note: cur->both == prev ^ next, next == prev ^ prev ^  next
    cur = (pddcp_xor_node *) ((uintptr_t) old_prev ^ (uintptr_t) cur->both);
  }
  *out = cur;
  return PDDCP_XORLL_OK;
}
