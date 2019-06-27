from nltk.tokenize import sent_tokenize


def substring_split(text, n):
    """Return all the possible substrings of length n in a given string"""
    substring = []
    for i in range(0, len(text)):
        psubstring = text[i:(n + i)]
        if len(psubstring) == n:
            substring.append(psubstring)
    return substring


def lines(a, b):
    """Return lines in both a and b"""
    def splitter(file): return iter(file.splitlines())
    la = splitter(a)
    lb = splitter(b)
    lines = []
    for line_a in splitter(a):
        for line_b in splitter(b):
            if line_a == line_b and not line_a in lines:
                lines.append(line_a)
    return lines


def sentences(a, b):
    """Return sentences in both a and b"""
    sa = sent_tokenize(a)
    sb = sent_tokenize(b)
    sentences = []
    for sentence_a in sa:
        for sentence_b in sb:
            if sentence_a == sentence_b and not sentence_a in sentences:
                sentences.append(sentence_a)
    return sentences


def substrings(a, b, n):
    """Return substrings of length n in both a and b"""
    substrings = []
    substrings_a = substring_split(a, n)
    substrings_b = substring_split(b, n)
    for substring_a in substrings_a:
        for substring_b in substrings_b:
            if substring_a == substring_b and not substring_a in substrings:
                substrings.append(substring_a)
    return substrings