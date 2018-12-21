#!/usr/bin/env python3

import fileinput

history = set()
freq = 0
history.add(freq)

# get an integer from each line of the input
inputs = [int(x.strip()) for x in fileinput.input() if x.strip()]

iter_guard = 10000 # watchdog loop counter
done = False

# loop through the inputs, keeping a running sum until a repeated sum is found
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
