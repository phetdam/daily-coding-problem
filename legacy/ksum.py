# daily coding problem #1 (easy, which it is)
#
# Changelog:
#
# 06-07-2019
#
# updated to include sentence indicating which company asked this question, as
# noted in the changes made to sol_template.py.
#
# 06-03-2019
#
# initial creation. problem solved in O(n^2) and O(n). if dictionary was
# replaced with balanced binary tree, solution would be O(nlogn).

__doc__ = """
This problem was recently asked by Google.

Given a list of numbers and a number k, return whether any two numbers from the
list add up to k. For example, given [10, 15, 3, 7] and k of 17, return true
since 10 + 7 is 17. Bonus: Can you do this in one pass?
"""

def ksum(A, k):
    """
    O(n^2) solution. naive.
    """
    # choose 0, ... n - 2 to prevent double counting n - 1th element.
    for i in range(len(A) - 1):
        # start from i + 1 to prevent double counting
        for j in range(i + 1, len(A)):
            if A[i] + A[j] == k:
                return True
    return False

def ksum_fast(A, k):
    """
    O(n) solution. with balanced binary tree, this would be O(nlogn) solution
    """
    # use dictionary as lookup in lieu of balanced binary tree
    look = {}
    for i in range(len(A)):
        # try to locate k = A[i] in look
        try:
            d = look[k - A[i]]
            return True
        except KeyError:
            look[A[i]] = A[i]
    return False

# main
if __name__ == "__main__":
    A = [-6, 7, 4, 34, 2, 3, 5, -9]
    k = -18
    func = ksum_fast
    print("{0}({1}, {2}) -> {3}".format(func.__name__, A, k, func(A, k)))
