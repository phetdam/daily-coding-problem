# daily coding problem #86 (medium)
#
# Changelog:
#
# 08-29-2019
#
# initial creation. similar to the problem of balanced brackets; use stack.

__doc__ = """
This problem was asked by Google.

Given a string of parentheses, write a function to compute the minimum number of
parentheses to be removed to make the string valid (i.e. each open parenthesis
is eventually closed). For example, given the string "()())()", you should
return 1. Given the string ")(", you should return 2, since we must remove all
of them.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# dict for parentheses
look = {"(": None, ")": "("}

def n_remove(s):
    """
    O(n) solution. use stack to store open parentheses; remove one if it is
    successfully closed. return length of stack as answer.
    """
    assert s is not None
    # get length of string
    n = len(s)
    # stack
    stack = []
    # for each parenthesis in the string
    for e in s:
        # check not needed but makes the function more general
        if e in look:
            # if it is an open, put on stack
            if look[e] is None: stack.append(e)
            # if it is a close
            elif look[e] is not None:
                # if the stack is not empty and the last thing is an
                # open, remove the open
                if (len(stack) > 0) and (look[stack[-1]] is None):
                    stack.pop()
                # else append the close to the stack; no matching
                else: stack.append(e)
    # return length of stack as our answer
    return len(stack)

# main
if __name__ == "__main__":
    func = n_remove
    # problem input 1, answer is 1
    s = "()())()"
    ufunc_eval(func, s)
    # problem input 2, answer is 2
    s = ")("
    ufunc_eval(func, s)
    # more general input, answer is 3
    s = "there once as (china) no (china())) vietnam)()("
    ufunc_eval(func, s)
