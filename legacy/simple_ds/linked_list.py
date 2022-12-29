# contains simple singly-linked list implementation and related functions
#
# Changelog:
#
# 12-23-2019
#
# initial creation. got tired of constantly defining the same classes for the
# same simple data structures and their related functions, so finally decided
# to just write a module containing class definitions and useful helping
# functions, like traversal functions etc. used to be simple_ds.py, until i
# realized that in order to do imports like simple_ds.xyz i would have to make
# simple_ds an entire directory in order to define a package. also changed name
# from LinkedList.py to linked_list.py since i hate case changes.

__doc__ = """
simple_ds.linked_list: module containing simple implementations of linked lists
for software engineering interviews, and related helpful functions.
"""
from sys import stderr

_MODULE_NAME = "simple_ds.LinkedList"

class ll_node:
    """
    bare-bones implementation of a node for a singly-linked list. makes it
    easy to create an entire linked list in one line by using the next_node
    named argument in the constructor. for example, one can have

    from simple_ds.LinkedList import ll_node
    my_list = ll_node(19, next_node = ll_node(-2, next_node = ll_node(4)))
    """
    def __init__(self, data, next_node = None):
        self.data = data
        self.next = next_node

def traverse(head):
    """
    traverses a linked list and returns all its elements in a string. if you
    want to return elements as a list, use ll2ar instead.
    """
    fname = traverse.__name__
    # trivial return
    if head is None: return "empty list"
    # check type
    if not isinstance(head, ll_node):
        raise TypeError("{0}: head must be of type ll_node".format(fname))
    outs = ""
    sep = " ~> "
    cur = head
    while cur is not None:
        outs = outs + str(cur.data) + sep
        cur = cur.next
    # remove the last space on the right
    return outs.rstrip()

def ar2ll(ar):
    """
    converts a list of elements into a linked list with insertion order being
    the original order of the list elements. if you want to get the original
    list of elements again, use the ll2ar function. can also work on iterables.
    if None is passed, None will be returned with a warning.
    """
    fname = ar2ll.__name__
    # warn if None
    if ar is None:
        print("{0}: warning: None passed, None returned".format(fname),
              file = stderr)
        return None
    # if not iterable, raise TypeError
    if "__iter__" not in dir(ar):
        raise TypeError("{0}: ar must be an iterable".format(fname))
    # head of the linked list, current node
    head, cur = None, None
    # for each of the elements, create new node, update cur.next and cur
    for e in ar:
        # special case for the first element
        if (head is None) and (cur is None):
            head = ll_node(e)
            cur = head
        # else just update cur.next and cur with the new node
        else:
            new_node = ll_node(e)
            cur.next = new_node
            cur = new_node
    # return head
    return head

def ll2ar(head):
    """
    converts a linked list into a list of elements, preserving the insertion
    order. if None is passed, then an empty list is returned.
    """
    fname = traverse.__name__
    # return empty list if None
    if head is None: return []
    # check type
    if not isinstance(head, ll_node):
        raise TypeError("{0}: head must be of type ll_node".format(fname))
    # output list
    outl = []
    # add all elements to outl and return
    cur = head
    while cur is not None:
        outl.append(cur.data)
        cur = cur.next
    return outl

# main
if __name__ == "__main__":
    print("{0}: do not run module as a script.".format(_MODULE_NAME),
          file = stderr)
