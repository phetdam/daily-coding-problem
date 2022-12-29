# daily coding problem #96 (easy, but i got stuck)
#
# Changelog:
#
# 09-07-2019
#
# decided to just tackle it recursively since this a divide and conquer type of
# problem. worked out well, but of course not super efficient.
#
# 09-06-2019
#
# initial creation. got stuck on an iterative approach.

__doc__ = """
This problem was asked by Microsoft.

Given a number in the form of a list of digits, return all possible permutations. 
For example, given [1, 2, 3], return [[1, 2, 3], [1, 3, 2], [2, 1, 3], [2, 3, 1],
[3, 1, 2], [3, 2, 1]].
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def permutations(ar, cp = None, all_p = None):
    """
    returns all permutations of ar. O(n!) in number of permutations as expected.
    """
    assert ar is not None
    # if current list of permutations is None, set it to []
    if all_p is None: all_p = []
    # if current permutation is None, set it to []
    if cp is None: cp = []
    # get length of ar
    n = len(ar)
    # if n is 0, return all_p (list of all permutations) after appending cp
    if n == 0:
        all_p.append(cp)
        return all_p
    # for each of the elements in ar, copy cp and add the element to the copy.
    # call recursively with ar sans that element and the modified copy.
    for i in range(n):
        ccp = [e for e in cp]
        ccp.append(ar[i])
        permutations(ar[:i] + ar[(i + 1):], cp = ccp, all_p = all_p)
    return all_p

# main
if __name__ == "__main__":
    func = permutations
    # problem input, has 3! == 6 possibilities
    ar = [1, 2, 3]
    print(len(ufunc_eval(func, ar)))
    ar = [1, 5, 7, 8]
    # another input, has 4! == 24 possibilities
    print(len(ufunc_eval(func, ar)))
