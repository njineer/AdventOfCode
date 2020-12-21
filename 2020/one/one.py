#!/usr/bin/env python3
import sys
import logging

logging.basicConfig(level=logging.INFO,
                    format="%(asctime)s [%(levelname)s], %(message)s",
                    datefmt="%Y-%m-%d %H:%M:%S")


def multiply_if_sum(input_list: list, target_sum: int = 2020):
    for i, x in enumerate(input_list[:-1]):
        for y in input_list[i + 1:]:
            logging.debug("%d + %d => %d", x, y, x+y)
            if x + y == target_sum:
                return x * y
    return None


def main():
    if len(sys.argv) < 1:
        print("Missing input file")
    else:
        with open(sys.argv[1], 'r') as f:
            puzzle_input = list(map(int, f.readlines()))
        result = multiply_if_sum(puzzle_input)
        if result is not None:
            print(result)
        else:
            print("Nothing")


main()
