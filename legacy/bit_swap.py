# daily coding problem #109 (medium)
#
# Changelog:
#
# 09-21-2019
#
# initial creation. after some thinking, figured out the right bit masks (8-bit
# only) to separate out the even and odd bits, shifted as necessary, and then
# or'd the even and odd bits together to get the answer.

__doc__ = """
This problem was asked by Cisco.

Given an unsigned 8-bit integer, swap its even and odd bits. The 1st and 2nd bit
should be swapped, the 3rd and 4th bit should be swapped, and so on.

For example, 10101010 should be 01010101. 11100010 should be 11010001.

Bonus: Can you do this in one line?
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def swap_bits(k):
    """
    swap even and odd bits of integer k (must be 8-bit size; see assertion).
    we use odd and even 8-bit masks, 01010101 == 85, 10101010 == 170, to first
    separate out odd and even bits. we shift odd bits to the left, even bits to
    the right, and then or the results together to get our answer. returns dict
    {k: "k_binary", k_swap: "k_swap_binary"}
    """
    assert (k >= 0) and (k <= 255)
    # swap even and odd bits (right to left reading)
    k_swap = ((k & 85) << 1) | ((k & 170) >> 1)
    # get string representations; pad with zeroes for aesthetic
    k_bin = "{:b}".format(k)
    k_bin = ((8 - len(k_bin)) * "0") + k_bin
    k_swap_bin = "{:b}".format(k_swap)
    k_swap_bin = ((8 - len(k_swap_bin)) * "0") + k_swap_bin
    return {k: k_bin, k_swap: k_swap_bin}

# main
if __name__ == "__main__":
    func = swap_bits
    # problem input 1, answer is 01010101 (85)
    k = 170
    ufunc_eval(func, k)
    # problem input 2, answer is 11010001 (209)
    k = 226
    ufunc_eval(func, k)
    # another input, answer is 01010111 (87)
    k = 171
    ufunc_eval(func, k)
