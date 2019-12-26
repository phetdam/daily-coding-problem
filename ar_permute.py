# daily coding problem #206 (easy)
#
# Changelog:
#
# 12-26-2019
#
# initial creation. i think the question is asking for an in-place solution, and
# technically my in-place solution is not really an in-place solution, as we
# still need extra memory to store the pairs. however, the not in-place solution
# would be preferred since it doesn't modify the original and is O(n).

__doc__ = """
This problem was asked by Twitter.

A permutation can be specified by an array P, where P[i] represents the location
of the element at i in the permutation. For example, [2, 1, 0] represents the
permutation where elements at the index 0 and 2 are swapped.

Given an array and a permutation, apply the permutation to the array. For
example, given the array ["a", "b", "c"] and the permutation [2, 1, 0], return
["c", "b", "a"].
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def ar_permute_inplace(ar, keys):
    """
    an in-place solution for this problem, O(nlogn) in complexity where n is
    the length of the array. the question asks for an inplace permutation, in
    which case we know the naive solution will definitely be O(n^2). this method
    first replaces each element in ar with an (element, index) pair where index
    is the corresponding permutation index given in keys, in-place sort using
    the .sort() method is performed, with sort by the index, and then the pairs
    are replaced with just the elements themselves.
    """
    assert (ar is not None) and (keys is not None) and (len(ar) == len(keys))
    # length of ar
    n = len(ar)
    # for each element ar[i], replace with pairs (ar[i], keys[i])
    for i in range(n): ar[i] = (ar[i], keys[i])
    # use .sort() method to modify array in-place, sorting by keys
    ar.sort(key = lambda x: x[1])
    # replace pairs with just the element
    for i in range(n): ar[i] = ar[i][0]
    # return even though the modification is in-place
    return ar

def ar_permute(ar, keys):
    """
    if we were allowed to provide a not in-place solution, we would have a
    simple O(n) solution with O(n) extra memory. technically this is still
    better than the in-place solution, as the in-place solution still requires
    extra memory to store the pairs so it's not exactly in-place.
    """
    assert (ar is not None) and (keys is not None) and (len(ar) == len(keys))
    # length of ar
    n = len(ar)
    # create new array to return
    out_ar = [None for _ in range(n)]
    # map each element ar[i] to the index keys[i] in out_ar (assume keys valid)
    for i in range(n): out_ar[keys[i]] = ar[i]
    # return permuted array
    return out_ar

# main
if __name__ == "__main__":
    # put ar_permute_inplace second since it performs in-place modification
    func_a, func_b = ar_permute, ar_permute_inplace
    # problem input, answer is ["c", "b", "a"]
    ar, keys = ["a", "b", "c"], [2, 1, 0]
    ufunc_eval(func_a, ar, keys)
    ufunc_eval(func_b, ar, keys)
    # another input, answer is ["f", "c", "e", "d", "b", "a"]
    ar, keys = ["a", "b", "c", "d", "e", "f"], [5, 4, 1, 3, 2, 0]
    ufunc_eval(func_a, ar, keys)
    ufunc_eval(func_b, ar, keys)
