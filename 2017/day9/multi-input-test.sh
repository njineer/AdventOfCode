#!/bin/bash
while IFS='' read -r line || [[ -n "$line" ]]; do
    echo $line
    echo $line | ./aoc9.sh
done < "$1"
