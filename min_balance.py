# daily coding problem #199 (hard, but really an easy problem)
#
# Changelog:
#
# 12-20-2019
#
# initial creation. idea is simple: first check if the parentheses are balnaced
# by the usual stack method, and then use the elements in the stack to determine
# how and where to append parentheses. there are multiple balancing solutions,
# so i picked the simplest method, which is to prepend open parentheses and
# append close parentheses as needed for balancing. i don't think this should be
# a hard problem, unless i am missing something crucial.

__doc__ = """
This problem was asked by Facebook.

Given a string of parentheses, find the balanced string that can be produced
from it using the minimum number of insertions and deletions. If there are
multiple solutions, return any of them.

For example, given "(()", you could return "(())". Given "))()(", you could
return "()()()()".
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def min_balance(s):
    """
    returns tuple of new balanced string and number of insertions. uses a stack
    to first determine if the string is balanced. for each closed parenthesis
    still on the stack, and open one will be appended to the beginning of the
    original string, while for each open parenthesis on the stack, a closed
    one will be append to the end of the string. this is simplest method.
    """
    assert s is not None
    # number of character additions
    nca = 0
    # trivial return case
    if len(s) == 0: return s, nca
    # stack
    stk = []
    # for each character in the string (assumed parentheses only)
    for c in s:
        # if the length of the stack is not 0 and the previous element is an
        # open parenthesis while c is a close parenthesis, then we can pop
        if (len(stk) > 0) and (stk[-1] == "(") and (c == ")"): stk.pop()
        # else we need to add c to the stack
        else: stk.append(c)
    # if length of stack is 0, return (s, nca)
    if len(stk) == 0: return s, nca
    # else set nca to len(stk)
    nca = len(stk)
    # number of open parentheses to prepend, number of closes to append
    nop, ncl = 0, 0
    # for each element in the stack
    for e in stk:
        # if e is open, we need to append a close, so increment ncl
        if e == "(": ncl = ncl + 1
        # else if e is closed, we need to prepend an open, so increment nop
        elif e == ")": nop = nop + 1
    # return balanced string and nca
    return (nop * "(") + s + (ncl * ")"), nca

# main
if __name__ == "__main__":
    func = min_balance
    # problem input, answer is ("(())", 1)
    s = "(()"
    ufunc_eval(func, s)
    # another problem input, [an] answer is ("(())()()", 3)
    s = "))()("
    ufunc_eval(func, s)
    # another input, answer is ("(())()(())", 0)
    s = "(())()(())"
    ufunc_eval(func, s)
    # another input, answer is ("((())()(()))(()()())", 5)
    s = "))()(()))(()()("
    ufunc_eval(func, s)
