# daily coding problem #2 (hard, which it is not, but the follow up may be hard
# depending on how strict one's interpretation of division is)
#
# Changelog:
#
# 06-07-2019
#
# updated to include sentence indicating which company asked this question, as
# noted in the changes made to sol_template.py.
#
# 06-04-2019
#
# initial creation. solved in O(n) using division, O(n^2) not using division.
# i didn't remember how to do the bit shift trick for division. i think the
# O(n^2) method is actually the naive way of doing it.

__doc__ = """
This problem was asked by Uber.

Given an array of integers, return a new array such that each element at index i
of the new array is the product of all the numbers in the original array except
the one at i. For example, if our input was [1, 2, 3, 4, 5], the expected output
would be [120, 60, 40, 30, 24]. If our input was [3, 2, 1], the expected output
would be [2, 3, 6]. Follow-up: what if you can't use division?
"""

from ufunc_eval import ufunc_eval

def xonesum(A):
    """
    O(n) solution. uses division.
    """
    # multiply all elements in A
    p = 1
    for e in A: p = p * e
    # empty list B
    B = []
    # for all elements e in A, divide p by e and append to B
    for e in A:
        # can cast to int because all e in A are factors of p
        B.append(int(p / e))
    return B

def xonesum_nodiv(A):
    """
    O(n^2) no division. i would think this is the naive way.
    """
    B = []
    p = 1
    for i in range(len(A)):
        for j in range(len(A)):
            if j != i:
                p = p * A[j]
        # append to B and reset p
        B.append(p)
        p = 1
    return B

# main
if __name__ == "__main__":
    A = [6, 3, 7, 5, 3, 4, -2, -8]
    func = xonesum_nodiv
    ufunc_eval(func, A)
