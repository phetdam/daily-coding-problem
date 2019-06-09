# daily coding problem #7 (medium, reminds me of dp days in school)
#
# Changelog:
#
# 06-09-2019
#
# initial creation. solved in one shot. trick is to think about the problem
# recursively; i.e. that the number of ways to decode a string of length n > 2
# if the number of ways to decode a string of length n - 1 and the number of
# ways to decode a string of length n - 2, as long as removing the last two
# characters, when cast to int, are less than 26. the recursive (naive) and
# (top-down) dynamic programming solution are both shown.

__doc__ = """
This problem was asked by Facebook.

Given the mapping a = 1, b = 2, ... z = 26, and an encoded message, count the
number of ways it can be decoded. For example, the message '111' would give 3,
since it could be decoded as 'aaa', 'ka', and 'ak'. You can assume that the
messages are decodable. For example, '001' is not allowed.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def decode_r(msg):
    """
    O(2^n) simply recursive solution. just used to verify that the problem is
    being solved in the correct way. take the recursive approach of removing
    one or two digits from the end of the string each time, but only recursing
    in the case of two digits if those two digits are together less than 26.
    """
    # base case (only one way to interpret empty string)
    if msg == "": return 1
    # get number of ways to decode if we remove only one character
    n = decode_r(msg[:-1])
    # if the string length is >= 2, get last two characters, check if when
    # converted to int are <= 26, and recurse if so
    if len(msg) > 1:
        c = int(msg[-2:])
        if c <= 26: n = n + decode_r(msg[:-2])
    # return result
    return n

def decode_dp(msg, A = None):
    """
    O(n) dynamic programming solution. we use a lookup array A to store the
    number of ways to decode strings of length i; result stored at A[i]. we
    take advantage of optional named arguments to avoid writing a separate
    wrapper function. complexity is O(n) because after the initial string
    of recursive calls (since A is full of 0 except at A[0]), for every call
    on message length k, A[k] will be populated.
    """
    # wrapper
    if A is None:
        # lookup table with n + 1 entries (n == len(msg)); set A[0] to 1
        A = [0 for i in range(len(msg) + 1)]
        A[0] = 1
        return decode_dp(msg, A)
    # get length of message
    n = len(msg)
    # if the result is in the lookup table, return it
    if A[n] > 0: return A[n]
    # else we don't have it, so we need to recurse and save the result. we first
    # get number of ways for to decode if we remove one character only
    A[n] = decode_dp(msg[:-1], A)
    # if the length of the string >= 2, get last two characters, check if when
    # cast to int they are <= 26, and recurse if so
    if n > 1:
        c = int(msg[-2:])
        if c <= 26: A[n] = A[n] + decode_dp(msg[:-2])
    # return result
    return A[n]

# main
if __name__ == "__main__":
    msg = "57981221" # output must be 5
    #func = decode_r
    ufunc_eval(decode_r, msg)
    ufunc_eval(decode_dp, msg)
