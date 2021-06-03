import sys
from nltk.tokenize import sent_tokenize


def lines(a, b):
    """Return lines in both a and b"""

    # Empty list
    lines_list = []

    # split the lines in strings into a list
    a_list = a.split(sep="\n")
    b_list = b.split(sep="\n")

    # if item in a and b list return into the lines_list using append i.e return similar items
    for element in a_list:
        if element in b_list and element not in lines_list:
            lines_list.append(element)

    # return list
    return lines_list


def sentences(a, b):
    """Return sentences in both a and b"""

    # Empty list
    sentences_list = []

    # Split sentences in strings a and b
    a_sentences = sent_tokenize(a)
    b_sentences = sent_tokenize(b)

    # append empty list with similar values in both lists
    for element in a_sentences:
        if element in b_sentences and element not in sentences_list:
            sentences_list.append(element)

    # return list
    return sentences_list


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""

    substrings_list = []

    a_sublist = set(get_substring(a, n))
    b_sublist = set(get_substring(b, n))

    for element in a_sublist:
        if element in b_sublist and element not in substrings_list:
            substrings_list.append(element)

    return substrings_list


def get_substring(a, n):
    """Get's substring of words"""
    get_substring = []

    # in order to continue the i loop we range over this
    hold = len(a) - n + 1

    for i in range(hold):
        get_substring.append(a[i:n+i])

    return get_substring

