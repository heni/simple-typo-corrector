#!/usr/bin/env python2.7
from __future__ import print_function
from __future__ import division
import itertools
import collections
import math
import os
import re

RE_WORD = re.compile(u"\w+", re.U)


def SplitToWords(ln):
    return [w.lower() for w in RE_WORD.findall(ln.strip().decode("utf-8"))]


def LoadTextFrequencies(unigrams, bigrams, filename):
    with open(filename) as textReader:
        words = itertools.chain.from_iterable(SplitToWords(ln) for ln in textReader)
        prevWord = None
        for w in words:
            if prevWord:
                bigrams[prevWord, w] += 1
            unigrams[w] += 1
            prevWord = w


def SaveFrequencies(unigrams, bigrams, statDir):
    sumFreqs = sum(unigrams.itervalues())
    with open(os.path.join(statDir, "unigrams.txt"), "w") as unigram_prn:
        for w, freq in unigrams.iteritems():
            if freq > 1:
                print(u"{0}\t{1:.8f}".format(w, math.log(freq/sumFreqs)).encode("utf-8"), file=unigram_prn)
    sumFreqs = sum(bigrams.itervalues())
    with open(os.path.join(statDir, "bigrams.txt"), "w") as bigram_prn:
        for (w1, w2), freq in bigrams.iteritems():
            if freq > 1:
                print(u"{0};{1}\t{2:.8f}".format(w1, w2, math.log(freq/sumFreqs)).encode("utf-8"), file=bigram_prn)


if __name__ == "__main__":
    DatasetDir = "dataset"
    bigrams = collections.defaultdict(int)
    unigrams = collections.defaultdict(int)
    for paperFile in os.listdir(DatasetDir):
        LoadTextFrequencies(unigrams, bigrams, os.path.join(DatasetDir, paperFile))
    SaveFrequencies(unigrams, bigrams, ".")
