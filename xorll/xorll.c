/**
 * xorll.c
 *
 * implementation of a simple xor linked list. for simplicity we assume that it
 * is only capable of storing integer data. note that stdint.h is required to
 * include a declared type that can store the int value of a pointer.
 *
 * Changelog:
 *
 * 06-08-2019
 *
 * initial creation. see xorll.py for the question statement. added all the
 * necessary append and get (by index) functions required by the question,
 * as well as constructors for the xorll and xor_node.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "xorll.h"

/* constructor for the xorll; returns xorll * */
xorll *xorll__new(void) {
    xorll *xll;
    xll = (xorll *) malloc(sizeof(xorll));
    xll->head = xll->tail = NULL;
    xll->siz = 0;
    return xll;
}
/* constructor for the xor_node; returns xor_node *, requires int */
xor_node *xor_node_new(int val) {
    xor_node *xn;
    xn = (xor_node *) malloc(sizeof(xor_node));
    xn->both = NULL;
    xn->data = val;
    return xn;
}
/* appends a node to the end of an xor linked list. requires xorll *, int. */
void xorll__append(xorll *xll, int val) {
    if (xll == NULL) {
	fprintf(stderr, "%s: error: xorll * expected, NULL received\n", \
		XORLL__APPEND_N);
	exit(1);
    }
    // make new xor_node
    xor_node *xn;
    xn = xor_node_new(val);
    /* if the xll is empty, head and tail pointers will both be NULL, so just
       check that the tail pointer is NULL. set size to 1. */
    if (xll->tail == NULL) {
	xll->head = xll->tail = xn;
	xll->siz = 1;
	return;
    }
    /* we need to modify tail's both field. since tail->both == tail's prev,
       we see that tail->both ^ xn == tail's prev ^ xn (tail's next). in the
       case that there is only one element, xll->tail and xll->head refer to
       the same thing, and xll->head->both == xll->tail->both == 0, so it is
       the same as setting xll->tail->both to xn. note that we need to use
       uintptr_t from stdint.h to have an integer type that is exactly the size
       of a pointer. */
    xll->tail->both = (xor_node *) ( \
	(uintptr_t) xll->tail->both ^ (uintptr_t) xn);
    // we set xn->both to be tail ^ 0, which is tail.
    xn->both = xll->tail;
    // now we set tail to be xn, since it is the new tail
    xll->tail = xn;
    // increase size by 1
    xll->siz = xll->siz + 1;
}
/* returns the xor_node * at the given index k in the linked list. */
xor_node *xorll__getnode(xorll *xll, int k) {
    if (xll == NULL) {
	fprintf(stderr, "%s: error: xorll * expected, NULL received\n", \
		XORLL__GETNODE_N);
	exit(1);
    }
    /* we need prev and cur nodes to traverse xor linked list; old_prev will
       save the previous value of prev */
    xor_node *prev, *cur, *old_prev;
    prev = NULL;
    cur = xll->head;
    /* while k > 0 and cur is not NULL (note that if k > number of nodes in xll,
       NULL will be returned. decrease k after loop comparison */
    while ((k-- > 0) && (cur != NULL)) {
	// save the value of prev in old_prev
	old_prev = prev;
	// update prev to cur since cur will change soon
	prev = cur;
	/* note that since cur->both == prev ^ next, next == prev ^ prev ^ next
	 since prev ^ prev will be 0, and 0 ^ next will be next. */
	cur = (xor_node *) ((uintptr_t) old_prev ^ (uintptr_t)cur->both);
    }
    // return cur
    return cur;
}
