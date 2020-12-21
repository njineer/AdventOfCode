#!/usr/bin/env python3
import logging
import argparse

logging.basicConfig(level=logging.INFO,
                    format="%(asctime)s [%(levelname)s], %(message)s",
                    datefmt="%Y-%m-%d %H:%M:%S")


def multiply_if_sum_2(input_list: list, target_sum: int = 2020):
    for i, x in enumerate(input_list[:-1]):
        for y in input_list[i + 1:]:
            logging.debug("%d + %d => %d", x, y, x+y)
            if x + y == target_sum:
                return x * y
    return None


def multiply_if_sum_3(input_list: list, target_sum: int = 2020):
    for i, x in enumerate(input_list[:-1]):
        for j, y in enumerate(input_list[i + 1:]):
            xy_sum = x+y
            for k, z in enumerate(input_list[j+1:]):
                logging.debug("%d + %d + %d => %d", x, y, z, x+y+z)
                if xy_sum + z == target_sum:
                    return x * y * z
    return None


def parse_args():
    ap = argparse.ArgumentParser(description="Advent of Code 2020: Day 1")
    ap.add_argument("-p", "--part", choices=(1, 2), default=1, type=int, help="Part 1 or part 2")
    ap.add_argument("input", help="Puzzle input file")
    return ap.parse_args()

def main():
    args = parse_args()
    with open(args.input, 'r') as f:
        puzzle_input = list(map(int, f.readlines()))
    func = multiply_if_sum_2 if args.part == 1 else multiply_if_sum_3
    result = func(puzzle_input)
    if result is not None:
        print(result)
    else:
        print("Nothing")


main()
