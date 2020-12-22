#!/usr/bin/env python3

import re
import argparse
import logging
from collections import Counter

logging.basicConfig(level=logging.INFO,
                    format="%(asctime)s [%(levelname)s], %(message)s",
                    datefmt="%Y-%m-%d %H:%M:%S")


class PasswordEntry:
    input_regex = re.compile("(?P<lower>\d+)-(?P<upper>\d+)\s*(?P<token>\w+):\s*(?P<password>\w+)")

    def __init__(self, lower_bound: int, upper_bound: int, token: str, password: str):
        self.lower_bound = lower_bound
        self.upper_bound = upper_bound
        self.token = token
        self.password = password

    def __str__(self):
        return f"<PasswordEntry({self.lower_bound}-{self.upper_bound} {self.token}: {self.password})>"

    __repr__ = __str__

    @classmethod
    def from_raw(cls, raw_input):
        match = cls.input_regex.match(raw_input)
        return cls(int(match.group("lower")),
                   int(match.group("upper")),
                   match.group("token"),
                   match.group("password")) if match else None

    @property
    def is_valid(self) -> bool:
        # counter = Counter(self.password)
        # return self.lower_bound <= counter[self.token] <= self.upper_bound
        return self.lower_bound <= Counter(self.password)[self.token] <= self.upper_bound


def parse_args() -> argparse.Namespace:
    ap = argparse.ArgumentParser(description="Advent of Code 2020: Day 2")
    # ap.add_argument("-p", "--part", choices=(1, 2), default=1, type=int, help="Part 1 or part 2")
    ap.add_argument("input", help="Puzzle input file")
    return ap.parse_args()


def main():
    args = parse_args()
    valid_count = 0
    with open(args.input, 'r') as f:
        for line in map(lambda string: string.strip(), f.readlines()):
            if PasswordEntry.from_raw(line).is_valid:
                valid_count += 1

    logging.info("%d valid entries", valid_count)


main()
