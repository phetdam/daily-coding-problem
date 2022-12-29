# daily coding problem #29 (easy)
#
# Changelog:
#
# 07-01-2019
#
# initial creation. not much to say here.

__doc__ = """
This problem was asked by Amazon.

Run-length encoding is a fast and simple method of encoding strings. The basic
idea is to represent repeated successive characters as a single count and
character. For example, the string "AAAABBBCCDAA" would be encoded as
"4A3B2C1D2A". Implement run-length encoding and decoding. You can assume the
string to be encoded have no digits and consists solely of alphabetic
characters. You can assume the string to be decoded is valid.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def rle(s):
    """
    simple one-pass solution. count the number of repeated characters and make
    note of that in the output string, while also doing a special check for the
    last character since it will not be written when the loop ends.
    """
    n = len(s)
    # last seen character and count of its consecutive instances
    ls = s[0]
    lsn = 1
    # output string
    outs = ""
    # for each character in the string (except for the first)
    for i in range(1, n):
        # if the current character does not match ls, append lsn and ls to outs.
        # then update ls to s[i] and set lsn to 1.
        if s[i] != ls:
            outs = outs + str(lsn) + ls
            ls = s[i]
            lsn = 1
        # else just increment lsn
        else: lsn = lsn + 1
    # the last character will not be written, so write lsn and ls before return
    outs = outs + str(lsn) + ls
    return outs

def rld(s):
    """
    one pass decoding. we assume that s is a valid decodable string.
    """
    n = len(s)
    # output string
    outs = ""
    # for each character in the string, skipping an index
    for i in range(1, n, 2):
        # add i - 1 s[i] to outs before incrementing twice
        outs = outs + (int(s[i - 1]) * s[i])
    return outs

# main
if __name__ == "__main__":
    # input given by problem. encode then decode
    s = "AAAABBBCCDAA"
    s = ufunc_eval(rle, s)
    s = ufunc_eval(rld, s)
    # arbitrary string this time
    s = "sdfiiiishhhhdDDDofSIDFDFEFFFISDIHDIFDF"
    s = ufunc_eval(rle, s)
    s = ufunc_eval(rld, s)
