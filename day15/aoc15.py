#!/usr/bin/env python3

import sys


def generator(start, factor, divisor, limit=40000000):
    value = start
    for _ in range(limit):
        value = (value * factor) % divisor
        yield value
    raise StopIteration

def judgeGenerators(aStart, bStart, *, aFactor=16807, bFactor=48271, divisor=2147483647, genLimit=40000000):
    matching = 0
    aGen = generator(aStart, aFactor, divisor, limit=genLimit)
    bGen = generator(bStart, bFactor, divisor, limit=genLimit)
    #comparator = lambda (a,b): (a & 0xFFFF) == (b & 0xFFFF)
    #return sum(map(comparator, zip(aGen, bGen)))
    i=0
    for a,b in zip(aGen, bGen):
        #print("a={}\tb={}".format(a, b))
        aMask, bMask = map(lambda x: x & 0xFFFF, (a,b))
        if aMask == bMask:
            matching += 1

        if i % 1000 == 0:
            print("{}/{}".format(i,genLimit), end="\r")
        i+=1
    return matching


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("missing a start value")
    else:
        matching = judgeGenerators(int(sys.argv[1]), int(sys.argv[2]))
        print("\n".format(matching))
