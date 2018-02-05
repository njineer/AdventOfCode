package main

import (
    "fmt"
    "bufio"
    "os"
    "strings"
    "strconv"
)

func main() {

    var scanner *bufio.Scanner

    // read from file specified by cl arg
    if args := os.Args[1:]; len(args) > 0 {
        file, err := os.Open(args[0])
        if err != nil {
            fmt.Printf("Error opening file: %v\n", args[0])
            os.Exit(2)
        }
        defer file.Close()

        scanner = bufio.NewScanner(file)
    } else {
        // read from stdin
        scanner = bufio.NewScanner(os.Stdin)
    }

    // memory: fixed number of banks consisting of arbitrary number of blocks
    const numBanks = 16
    memory := make([]int, numBanks)

    // load initial memory allocation
    scanner.Scan()
    initAlloc := scanner.Text()
    for i, block := range strings.Fields(initAlloc) {
        if b, err := strconv.Atoi(block); err != nil {
            fmt.Print("Error in input. Cannnot convert '%v' to int\n", block)
        } else {
            memory[i] = b
        }
    }


    fmt.Println("Advent of Code: Day 6")
}
