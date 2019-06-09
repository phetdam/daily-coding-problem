/**
 * xorll.h
 *
 * declares the struct and functions for operating on an xor linked list that
 * can only take integers (for simplicity, since this is an interview question).
 *
 * Changelog:
 *
 * 06-08-2019
 *
 * initial creation.
 */

#ifndef XORLL_H
#define XORLL_H
/* xor node. the both field contains the xor of the previous and next nodes'
   memory location. */
struct xor_node {
    struct xor_node *both;
    int data;
};
typedef struct xor_node xor_node;
/* xor doubly linked list wrapper. head and tail pointers. */
struct xorll {
    xor_node *head, *tail;
    // contain size for ease of comparison purposes
    int siz;
};
typedef struct xorll xorll;
// function names
#define XORLL__NEW "xorll__new"
#define XOR_NODE_NEW "xor_node_new"
#define XORLL__APPEND_N "xorll__append"
#define XORLL__GETNODE_N "xorll__getnode"
/* constructor for the xorll; returns xorll * */
xorll *xorll__new(void);
/* constructor for the xor_node; returns xor_node *, requires int */
xor_node *xor_node_new(int val);
/* appends a node to the end of an xor linked list. requires xorll *, int. */
void xorll__append(xorll *xll, int val);
/* returns the xor_node * at the given index k in the linked list. */
xor_node *xorll__getnode(xorll *xll, int k);
#endif /* XORLL_H */
