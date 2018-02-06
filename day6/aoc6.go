package main

import (
    "fmt"
    "bufio"
    "os"
    "strings"
    "strconv"
)

func printMemory(s []int) {
    fmt.Print("[")
    for _, x := range s[:len(s)-1] {
        fmt.Printf("%v,", x)
    }
    fmt.Printf("%v]\n", s[len(s)-1])
}

func sliceToString(sInt []int) string {
    sStr := make([]string, len(sInt))
    for i, x := range sInt {
        sStr[i] = strconv.Itoa(x)
    }
    return strings.Join(sStr, ",")
}

func findMaxIndex(a []int) int {
    var maxIndex, maxValue = 0, 0

    for i, v := range a {
        if v > maxValue {
            maxValue = v
            maxIndex = i
        }
    }
    return maxIndex
}

func reallocate(memory []int) {
    // find index of max number of blocks
    maxIndex := findMaxIndex(memory)
    // rellocate those blocks
    alloc := memory[maxIndex]
    // reset previously-max bank
    memory[maxIndex] = 0

    for i := maxIndex+1; alloc > 0; i, alloc = i+1, alloc-1 {
        memory[i%len(memory)]++
    }
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
    fmt.Printf("reallocations: %v\n", aoc6(memory))
    fmt.Printf("cycles: %v\n", aoc6_2(memory))
}

func aoc6 (memory []int) int {
    // history of bank configurations seen
    history := make(map[string]struct{})


    memoryStr := sliceToString(memory)
    reallocations := 0

    for _, seen := history[memoryStr]; !seen; {
        history[memoryStr] = struct{}{}
        reallocate(memory)
        memoryStr = sliceToString(memory)
        _, seen = history[memoryStr]
        reallocations++
    }
    return reallocations
}

func aoc6_2 (memory []int) int {
    // history of bank configurations seen
    history := make(map[string]int)


    memoryStr := sliceToString(memory)
    reallocations := 0

    for _, seen := history[memoryStr]; !seen; {
        history[memoryStr] = reallocations
        reallocate(memory)
        memoryStr = sliceToString(memory)
        _, seen = history[memoryStr]
        reallocations++
    }
    return reallocations-history[memoryStr]
}
