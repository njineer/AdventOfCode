#!/usr/bin/env python3

import sys
from collections import Counter

def aoc4(filename):
    valid = 0
    with open(filename) as f:
        for line in f:
            if line.strip():
                passphrases = line.split()
                if Counter(passphrases).most_common(1)[0][1] == 1:
                    valid +=1
    return valid


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Missing input file.", file=sys.stderr)
    elif len(sys.argv) > 2:
        print("Too many arugments.", file=sys.stderr)
    else:
        print(aoc4(sys.argv[1]))
