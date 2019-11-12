# daily coding problem #161 (easy)
#
# Changelog:
#
# 11-12-2019
#
# initial creation. xor the original integer with an integer of all 1s to
# revsere the bits of the original integer. however, we need a mask of the right
# length, so we first need to count bits. added additional function to show the
# original bit layout with the reversed bit layout

__doc__ = """
This problem was asked by Facebook.

Given a 32-bit integer, return the number with its bits reversed.

For example, given the binary number 1111 0000 1111 0000 1111 0000 1111 0000,
return 0000 1111 0000 1111 0000 1111 0000 1111.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def bit_reverse(n):
    """
    return int n with reversed bits. we first figure out how many bits are in n
    to create our mask of 1s, xor with the 1s, and return the result.
    """
    # ignore signed or unsigned integers
    # power of highest bit
    bp = 0
    # continue to shift bits to the right and increment bp until the result is 1
    temp = n
    while temp > 1:
        temp = temp >> 1
        bp = bp + 1
    # create mask of ones where the highest bit is the bp + 1th bit
    mask = 0
    while bp > -1:
        mask = mask + pow(2, bp)
        bp = bp - 1
    # return masked result
    return n ^ mask

def with_reverse(n):
    """
    helper function that compares the bits of the original element with those of
    the reversed number (bits are reversed). pad with 0s to maintain length.
    """
    # get binary string formats and reversed integer value
    nb = "{0:b}".format(n)
    nr = bit_reverse(n)
    nrb = "{0:b}".format(nr)
    # lengths
    nb_len = len(nb)
    nrb_len = len(nrb)
    # pad shorter string with prefixed 0s
    if nb_len < nrb_len:
        nb = ("0" * (nrb_len - nb_len)) + nb
    if nrb_len < nb_len:
        nrb = ("0" * (nb_len - nrb_len)) + nrb
    # return formatted string
    return ("original: {0} ({1})\nreversed: {2} ({3})"
            "".format(nb, n, nrb, nr))

# main
if __name__ == "__main__":
    # use with_reverse for comparison
    func = with_reverse
    # problem input, answer is 252645135 in binary
    n = 4042322160
    ufunc_eval(func, n)
    # another input, answer is 8299896 in binary
    n = 25254535
    ufunc_eval(func, n)
