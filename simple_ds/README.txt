initial creation: 12-23-2019
last updated:     12-23-2019

simple_ds: simple data structure implementations for hassle-free software
           engineering inverview preparation

contains some simple implementations of basic data structures like binary trees
and linked lists, for the purpose of interview questions, so none of the
implementations are particularly nice. also contains related functions that are
useful, such as traversal functions for linked lists and trees, etc.

i wrote this simple package to make the process of writing code for interview
problems easier since i always ended up defining the same basic classess for the
same basic data structures and wrote the same methods to traverse/check these
data structures. so i decided to just write a simple package to solve this
annoying problem once and for all, and this is the result.

contents

binary_tree.py:

    contains a simple binary tree implementation and a function to return the
    elements of the tree as a list, with options for different orderings such as
    inorder, preorder, postorder, and level order/breadth first traversal.

linked_list.py:

    contains a simple singly-linked list implementation, a function to traverse
    the linked list and returns its contents as a string, as well as functions
    to convert between linked list and iterable.
