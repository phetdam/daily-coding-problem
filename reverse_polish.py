# daily coding problem #163 (hard, but a simple problem really)
#
# Changelog:
#
# 11-15-2019
#
# initial creation. a good exercise that is definitely not hard.

__doc__ = """
This problem was asked by Jane Street.

Given an arithmetic expression in Reverse Polish Notation, write a program to
evaluate it. The expression is given as a list of numbers and operands [sic].

For example: [5, 3, '+'] should return 5 + 3 = 8.

For example, [15, 7, 1, 1, '+', '-', '/', 3, '*', 2, 1, 1, '+', '+', '-'] should
return 5, since it is equivalent to

((15 / (7 - (1 + 1))) * 3) - (2 + (1 + 1)) = 5.

You can assume the given expression is always valid.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# reverse polish calculator class
class rpolish:

    def __init__(self):
        # data stack for operands
        self.oprs = None

    def eval(self, l):
        """
        assume given list l of operands and operators is valid, and evaluate.
        the approach is to put operands on the stack, and upon the instance of
        an operator, the top two values are operated upon with the operator,
        popped from the stack, and the result is pushed onto the stack.
        """
        assert (l is not None) and (len(l) > 0)
        # make sure stack is empty
        self.oprs = []
        # for each element in the list
        for e in l:
            # if the element is an operand, put it on the stack
            if isinstance(e, int) or isinstance(e, float):
                self.oprs.append(e)
            # else assume it is a valid arithmetic operator, ex.
            else:
                # result
                res = None
                # if it is an addition operator, add last two operands
                if e == "+": res = self.oprs[-2] + self.oprs[-1]
                # else if it is a subtraction operator, substract last two
                elif e == "-": res = self.oprs[-2] - self.oprs[-1]
                # else multiply
                elif e == "*": res = self.oprs[-2] * self.oprs[-1]
                # else divide
                elif e == "/": res = self.oprs[-2] / self.oprs[-1]
                # else error
                else: raise Exception("unknown operator {0}".format(e))
                # pop twice to remove original operands
                self.oprs.pop()
                self.oprs.pop()
                # push result onto stack
                self.oprs.append(res)
        # return answer
        return self.oprs[0]

# main
if __name__ == "__main__":
    # reverse polish calculator
    rpc = rpolish()
    # problem input, answer is 5
    l = [15, 7, 1, 1, '+', '-', '/', 3, '*', 2, 1, 1, '+', '+', '-']
    ufunc_eval(rpc.eval, l)
    # another input, answer is 28.75
    #(-4 / 16) + (5 + 8 - (7 * 6)) * -1
    l = [-4, 16, '/', 5, 8, 7, 6, '*', '-', '+', -1, '*', '+']
    ufunc_eval(rpc.eval, l)
