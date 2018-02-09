package main

import (
    "fmt"
    "bufio"
    "os"
    "strings"
    "strconv"
)

// debug - print the memory banks
func printMemory(s []int) {
    fmt.Print("[")
    for _, x := range s[:len(s)-1] {
        fmt.Printf("%v,", x)
    }
    fmt.Printf("%v]\n", s[len(s)-1])
}

// join a slice of ints into a comma-separated string
func sliceToString(sInt []int) string {
    sStr := make([]string, len(sInt))
    for i, x := range sInt {
        sStr[i] = strconv.Itoa(x)
    }
    return strings.Join(sStr, ",")
}

// find the index of the largest value in a slice of ints
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
    // those block will be reallocated
    alloc := memory[maxIndex]
    // reset previously-max bank
    memory[maxIndex] = 0

    // reallocate blocks
    for i := maxIndex+1; alloc > 0; i, alloc = i+1, alloc-1 {
        memory[i%len(memory)]++
    }
}

func aoc6 (memory []int) int {
    // set of bank configurations we have seen
    history := make(map[string]struct{})

    // stringify the current memory layout
    memoryStr := sliceToString(memory)
    reallocations := 0

    for _, seen := history[memoryStr]; !seen; {
        // add this memory layout to the history
        history[memoryStr] = struct{}{}
        // reallocate the largest bank
        reallocate(memory)
        // stringify the current memory layout and check if it's new
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
        // add this memory layout to the history, along with when it was seen
        history[memoryStr] = reallocations
        // reallocate the largest bank
        reallocate(memory)
        // stringify the current memory layout and check if it's new
        memoryStr = sliceToString(memory)
        _, seen = history[memoryStr]

        reallocations++
    }
    // number of reallocations - cycle we first saw this layout
    return reallocations-history[memoryStr]
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

