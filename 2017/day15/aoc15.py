#!/usr/bin/env pypy3

import sys
import argparse
from itertools import islice
from collections import namedtuple

GenParam = namedtuple('GeneratorParameters', ('start', 'factor', 'divisor', 'multiple'))

def generator(start, factor, divisor, multiple=1):
    value = start
    while True:
        value = (value * factor) % divisor
        if value % multiple == 0:
            yield value

def comparator(pair):
    a,b = pair
    return (a & 0xFFFF) == (b & 0xFFFF)

def judgeGenerators(aGenParam, bGenParam, limit=40000000):
    #matching = 0
    aGen = generator(*aGenParam)
    bGen = generator(*bGenParam)
    return sum(map(comparator, islice(zip(aGen, bGen), limit)))
    #i=0
    #for a,b in zip(aGen, bGen):
    #    #print("a={}\tb={}".format(a, b))
    #    aMask, bMask = map(lambda x: x & 0xFFFF, (a,b))
    #    if aMask == bMask:
    #        matching += 1

    #    if i % 1000 == 0:
    #        print("{}/{}".format(i,genLimit), end="\r")
    #    i+=1
    #return matching

def parseArgs():
    ap = argparse.ArgumentParser("AoC Day15: Generator Judgement")
    ap.add_argument("a", type=int, help="Start value for generator A")
    ap.add_argument("b", type=int, help="Start value for generator B")
    ap.add_argument("--a-mult", type=int, default=1, help="Filter generator A to multiples of this value")
    ap.add_argument("--b-mult", type=int, default=1, help="Filter generator B to multiples of this value")
    ap.add_argument("-l", "--limit", type=int, default=40000000, help="Number of pairs to consider.")
    return ap.parse_args()

if __name__ == "__main__":
    args = parseArgs()

    aFactor = 16807
    bFactor = 48271
    divisor = 2147483647

    aGenParam = GenParam(args.a, aFactor, divisor, args.a_mult)
    bGenParam = GenParam(args.b, bFactor, divisor, args.b_mult)
    matching = judgeGenerators(aGenParam, bGenParam, limit=args.limit)
    print(matching)
