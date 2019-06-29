# daily coding problem #27 (easy, just use stack)
#
# Changelog:
#
# 06-29-2019
#
# initial creation. uses stack to check if brackets are balanced. ignores angle
# brackets and can be used on any string that has brackets.

__doc__ = """
This problem was asked by Facebook.

Given a string of round, curly, and square open and closing brackets, return
whether the brackets are balanced (well-formed). For example, given the string
"([])[]({})", you should return true. Given the string "([)]" or "((()", you
should return false.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# small dict to make it easy to figure out what bracket precedes what
_pre = {"{": None, "}": "{", "(": None, ")": "(", "[": None, "]": "["}

def check_brackets(bs):
    """
    use a stack to keep track of all the brackets there, and when one bracket is
    matched with its closing bracket, then pop the bracket from the stack. can
    be used on any string that has brackets in _pre.
    """
    # sanity checks
    if bs is None: return True
    n = len(bs)
    if n == 0: return True
    # stack to hold brackets
    stack = []
    # for each character in the string
    for e in bs:
        # if the character is a bracket
        if e in _pre:
            # if the length of the stack is nonzero and the previous element on
            # the stack is the bracket that precedes it (that e matches), then
            # pop from the stack
            if (len(stack) > 0) and (stack[-1] == _pre[e]): stack.pop()
            # else just push e onto the stack
            else: stack.append(e)
    # if the length of the stack is greater than 0, we did not match something
    return len(stack) == 0

def g(x):
    """
    description of solution runtime and any other comments. usually necessary
    because follow up question are often asked.
    """
    return False

# main
if __name__ == "__main__":
    func = check_brackets
    # given input string 1, returns True
    bs = "([])[]({})"
    ufunc_eval(func, bs)
    # given input string 2, returns False
    bs = "([)]"
    ufunc_eval(func, bs)
    # given input string 3, also returns False
    bs = "((()"
    ufunc_eval(func, bs)
    # arbitrary string of C code, returns True (ignore angle brackets)
    bs = """
    int tree_size(tnode *root) {
        if (root == NULL) {
            return 0;
        }
        return tree_size(root.left) + 1 + tree_size(root.right);
    }

    tnode * get_kth_smallest(tnode *root, k) {
        if (root == NULL) || (k < 1) {
            return NULL;
        }
        lsiz = tree_size(root.left)
        if (lsiz == k - 1) {
            return root;
        }
        else if (lsiz > k - 1) {
            return get_kth_smallest(root.left, k);
        }
        return get_kth_smallest(root.right, k - lsiz - 1);
    }
    """
    ufunc_eval(func, bs)
