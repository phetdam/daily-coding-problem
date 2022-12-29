# daily coding problem #22 (medium)
#
# Changelog:
#
# 06-24-2019
#
# initial creation. more or less simple version that allows arbitary word
# placement and repetition but will produce extra words if some words are
# substrings of other words, so words like "a" and "at" cause problems.

__doc__ = """
This problem was asked by Microsoft.

Given a dictionary of words and a string made up of those words (no spaces),
return the original sentence in a list. If there is more than one possible
reconstruction, return any of them. If there is no possible reconstruction, then
return null.

For example, given the set of words 'quick', 'brown', 'the', 'fox', and the
string "thequickbrownfox", you should return ['the', 'quick', 'brown', 'fox'].

Given the set of words 'bed', 'bath', 'bedbath', 'and', 'beyond', and the string
"bedbathandbeyond", return either ['bed', 'bath', 'and', 'beyond] or ['bedbath',
'and', 'beyond'].
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def make_sentence(s, wlist):
    """
    for each word in the list, find its starting index in the string, and use
    its length to get its ending index. save the beginning and ending indices
    as a tuple in a list, and use those to determine where to put the other
    words if any are also found in the sentence.
    """
    # quick sanity check
    if (s is None) or (wlist is None) or (len(wlist) == 0): return None
    # list of starting and ending index tuples
    indices = []
    # collection of strings to make as a sentence
    sent = []
    # for each word in the word list
    for e in wlist:
        # find the starting index of the word in s
        si = s.find(e)
        # initially set ending index to -1; no offset in search for next
        # occurrence of the word
        ei = -1
        # for all occurrences of this word in the string
        while si >= 0:
            # we found the word (find returns -1 if not found), so get the
            # ending index too and add offset to si (if any)
            si = si + ei + 1
            ei = si + len(e) - 1
            # if indices and sent are empty, just append
            if (len(indices) == 0) and (len(sent) == 0):
                indices.append((si, ei))
                sent.append(e)
            # else search in indices to find where to insert the new word. we
            # want to find the tuple (osi, oei) s.t. ei < osi.
            else:
                # insertion index
                ii = 0
                while ii < len(indices):
                    # if we find that ei is less than the starting index of
                    # tuple at indices[i], this is right place to insert
                    if ei < indices[ii][0]: break
                    # else increment ii
                    ii = ii + 1
                # insert at ii
                indices.insert(ii, (si, ei))
                sent.insert(ii, e)
            # find again, but on the substring after ei (offset by ei + 1)
            si = s[(ei + 1):].find(e)
    # return one of potentially several sentence combinations
    return sent

def g(x):
    """
    description of solution runtime and any other comments. usually necessary
    because follow up question are often asked.
    """
    return False

# main
if __name__ == "__main__":
    func = make_sentence
    # first given problem input
    s = "thequickbrownfox"
    wlist = ["the", "quick", "brown", "fox"]
    ufunc_eval(func, s, wlist)
    # second given problem input
    s = "bedbathandbeyond"
    wlist = ["bed", "bath", "and", "beyond"]
    ufunc_eval(func, s, wlist)
    # third input, varying wlist only
    wlist = ["bedbath", "and", "beyond"]
    ufunc_eval(func, s, wlist)
    # arbitrary sentence (from eminem's rap god)
    s = ("Lyricscomingatyouatsupersonicspeed,(JJFad)Uh,samalamaadumalamaayou"
         "assumingI'mahumanWhatIgottadotogetitthroughtoyouI'msuperhuman")
    wlist = ["supersonic", "rubber", "Lyrics", "at", "you", "coming", "speed",
             "Fad", "JJ", "Uh", "lamaa", "sama", "duma", "I'm", "human"]
    ufunc_eval(func, s, wlist)
