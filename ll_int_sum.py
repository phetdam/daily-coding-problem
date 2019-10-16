# daily coding problem #127 (easy, but does take a little thought)
#
# Changelog:
#
# 10-16-2019
#
# initial creation. relatively straightforward question; one just needs to be
# careful with how to handle carrying an extra digit and managing pointers.

__doc__ = """
This problem was asked by Microsoft.

Let's represent an integer in a linked list format by having each node represent
a digit in the number. The nodes make up the number in reversed order.

For example, the following linked list:

1 -> 2 -> 3 -> 4 -> 5

is the number 54321.

Given two linked lists in this format, return their sum in the same linked list
format. For example, given

9 -> 9
5 -> 2

return 124 (99 + 25) as:

4 -> 2 -> 1
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# digit node class
class digit:

    def __init__(self, val, next_digit = None):
        self.val = val
        self.next_digit = next_digit

def add_digits(a, b):
    """
    adds two digit linked lists together to produce another list that represents
    the sum of the two integers represented by the input lists.
    """
    assert (a is not None) and (b is not None)
    # carry digit value
    carry = 0
    # first digit of c as a and b have at least one digit each
    chead = digit((a.val + b.val) % 10)
    # current digit of c
    c = chead
    # if a.val + b.val greater than 9, then carry is 1
    if a.val + b.val > 9: carry = 1
    # advance a and b
    a = a.next_digit
    b = b.next_digit
    # while a and b are not None
    while (a is not None) and (b is not None):
        # add new digit, including carry value
        c.next_digit = digit((a.val + b.val + carry) % 10)
        # if sum of vals of a, b, carry > 9, then carry is 1 again, else carry 0
        if a.val + b.val + carry > 9: carry = 1
        else: carry = 0
        # advance a, b, c
        a = a.next_digit
        b = b.next_digit
        c = c.next_digit
    # either a or b is None
    while a is not None:
        # add new digit plus carry
        c.next_digit = digit((a.val + carry) % 10)
        # if sum of vals > 9, carry is 1 again, else carry is 0
        if a.val + carry > 9: carry = 1
        else: carry = 0
        # advance a, c
        a = a.next_digit
        c = c.next_digit
    while b is not None:
        c.next_digit = digit((b.val + carry) % 10)
        if b.val + carry > 9: carry = 1
        else: carry = 0
        b = b.next_digit
        c = c.next_digit
    # if carry is still 1, then add it to the end as an extra
    if carry == 1: c.next_digit = digit(carry)
    # return chead
    return chead

def int2digit(k):
    """
    converts an integer k into the digit representation that we want. note that
    because we don't have a sign, k must be nonnegative.
    """
    assert k >= 0
    # rightmost digit
    rmd = digit(k % 10)
    # current digit node
    cur = rmd
    # divide 10 out of k
    k = k // 10
    # while k is not zero, make additional digits
    while k > 0:
        # set next digit using next digit of k
        cur.next_digit = digit(k % 10)
        # floor after dividing by 10 and update cur
        k = k // 10
        cur = cur.next_digit
    # return rmd
    return rmd

def digit2int(d):
    """
    returns the integer representation of a digit with least significant digit d
    """
    if d is None: return 0
    # output value
    val = 0
    # current tens place
    ctp = 0
    # while we have not exhausted all the digits, add to val
    while d is not None:
        # add in correct digit position
        val = val + (d.val * pow(10, ctp))
        # advance to next digit and increase tens place
        d = d.next_digit
        ctp = ctp + 1
    # return val
    return val

# main
if __name__ == "__main__":
    func = add_digits
    # problem input, answer is 124
    a, b = int2digit(99), int2digit(25)
    c = ufunc_eval(func, a, b)
    print(digit2int(c))
    # another input, answer is 4353
    a, b = int2digit(3415), int2digit(938)
    c = ufunc_eval(func, a, b)
    print(digit2int(c))
