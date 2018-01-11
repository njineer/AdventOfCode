#!/usr/bin/env python3

import sys

def aoc2 (filename):
    sum = 0
    with open(filename, 'r') as f:
        for line in f:
            if line.strip():
                values = [ int(x) for x in line.strip().split() ]
                diff = max(values) - min(values)
                sum += diff
    return sum


def aoc2_2 (filename):
    sum = 0
    with open(filename, 'r') as f:
        for line in f:
            if line.strip():
                values = [ int(x) for x in line.strip().split() ]
                for index, x in enumerate(values[:-1]):
                    for y in values[index+1:]:
                        greater = max(x,y)
                        lesser = min(x,y)
                        if greater % lesser == 0:
                            sum += greater // lesser
                            break
                    else:
                        continue # inner loop not broken
                    break # break outer loop if inner loop is broken
    return sum

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Missing input file.", file=sys.stderr)
    elif len(sys.argv) > 2:
        print("Too many arugments.", file=sys.stderr)
    else:
        print(aoc2(sys.argv[1]))
        print(aoc2_2(sys.argv[1]))
