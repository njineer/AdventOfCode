#!/usr/bin/env python3

import fileinput

history = set()
freq = 0
history.add(freq)

for line in fileinput.input():
    line_stripped = line.strip()
    if not line_stripped:
        continue
    freq += int(line_stripped)
    print(freq)
    if freq in history:
        print(freq)
        break
    else:
        history.add(freq)
