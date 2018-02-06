package main

import (
    "fmt"
    "bufio"
    "os"
    "strings"
    "strconv"
)

func printMemory(s *[16]int) {
    fmt.Print("[")
    for _, x := range s[:len(s)-1] {
        fmt.Printf("%v,", x)
    }
    fmt.Printf("%v]\n", s[len(s)-1])
}

func findMaxIndex(a *[16]int) int {
    var maxIndex, maxValue = 0, 0

    for i, v := range a {
        if v > maxValue {
            maxValue = v
            maxIndex = i
        }
    }
    fmt.Printf("Max = %v @ %v\n", maxValue, maxIndex)
    return maxIndex
}

func reallocate(memory *[16]int) {
    fmt.Print("Before reallocation: ")
    printMemory(memory)
    // find index of max number of blocks
    maxIndex := findMaxIndex(memory)
    // rellocate those blocks
    alloc := memory[maxIndex]
    // reset previously-max bank
    memory[maxIndex] = 0

    for i := maxIndex+1; alloc > 0; i, alloc = i+1, alloc-1 {
        memory[i%16]++
    }
    fmt.Print("After reallocation:  ")
    printMemory(memory)
}

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
    var memory [numBanks]int
    
    // history of bank configurations seen
    history := make(map[[16]int]struct{})

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

    reallocations := 0
    watchdog := 1000
    for _, seen := history[memory]; !seen && watchdog > 0; {
        fmt.Printf("Reallocation %v: ", reallocations)
        printMemory(&memory)
        history[memory] = struct{}{}
        reallocate(&memory)
        reallocations++
        watchdog--
    }

    fmt.Println("Advent of Code: Day 6")
    fmt.Printf("reallocations: %v\n", reallocations)
}
