import fileinput
from functools import reduce
from operator import add

# print the sum of the integer on each line of the input
print(reduce(add, [int(x.strip()) for x in fileinput.input() if x.strip()], 0))
