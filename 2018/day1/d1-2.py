#!/usr/bin/env python3

import fileinput

history = set()
freq = 0
history.add(freq)

inputs = [int(x.strip()) for x in fileinput.input() if x.strip()]

iter_guard = 10000
done = False

while iter_guard and not done:
    for val in inputs:
        freq += val
        if freq in history:
            print(freq)
            done = True
            break
        else:
            history.add(freq)
    iter_guard -=1
