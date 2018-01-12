#!/usr/bin/env python3

import sys
from collections import Counter

def aoc4(filename):
    valid = 0
    with open(filename) as f:
        for line in f:
            if line.strip():
                words = line.strip().split()
                if Counter(words).most_common(1)[0][1] == 1:
                    valid +=1
    return valid


def aoc4_2(filename):
    valid = 0
    with open(filename) as f:
        for line in f:
            if line.strip():
                # counters are not hashable, so check for duplicates manually
                counters = []
                for word in line.strip().split():
                    c = Counter(word)
                    if c in counters:
                        break
                    else:
                        counters.append(c)
                else:
                    valid +=1
    return valid




if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Missing input file.", file=sys.stderr)
    elif len(sys.argv) > 2:
        print("Too many arugments.", file=sys.stderr)
    else:
        print(aoc4(sys.argv[1]))
        print(aoc4_2(sys.argv[1]))
