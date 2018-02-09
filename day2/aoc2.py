#!/usr/bin/env python3

import sys

def aoc2 (filename):
    sum = 0
    with open(filename, 'r') as f:
        for line in f:
            if line.strip(): # remove whitespace-only or empty lines
                values = [ int(x) for x in line.strip().split() ] # convert to ints
                diff = max(values) - min(values)
                sum += diff
    return sum


def aoc2_2 (filename):
    sum = 0
    with open(filename, 'r') as f:
        for line in f:
            if line.strip(): # remove whitespace-only or empty lines
                values = [ int(x) for x in line.strip().split() ] # convert to ints
                # for each item...
                for index, x in enumerate(values[:-1]):
                    lineDone = False
                    # compare to all items later in the line
                    for y in values[index+1:]:
                        # find which value is larger to avoid result < 1
                        greater = max(x,y)
                        lesser = min(x,y)
                        if greater % lesser == 0: # divides evenly
                            sum += greater // lesser
                            lineDone = True
                    if lineDone:
                        # no need to keep going; assume 1 even division per line
                        break
    return sum

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Missing input file.", file=sys.stderr)
    elif len(sys.argv) > 2:
        print("Too many arugments.", file=sys.stderr)
    else:
        print(aoc2(sys.argv[1]))
        print(aoc2_2(sys.argv[1]))
