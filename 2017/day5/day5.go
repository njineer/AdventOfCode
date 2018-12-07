package main

import (
    "fmt"
    "bufio"
    "os"
    "strings"
    "strconv"
)

func jumpsUntilExit(jumps []int) int {
    steps := 0
    // for each jump instruction
    for offset := 0; offset >= 0 && offset < len(jumps); {
        // calculate where the jump destination is
        newOffset := offset + jumps[offset]
        // increment this current jump value
        jumps[offset]++
        // make the jump
        offset = newOffset
        // count how many jumps
        steps++
    }
    return steps
}

func jumpsUntilExit2(jumps []int) int {
    steps := 0
    // for each jump instruction
    for offset := 0; offset >= 0 && offset < len(jumps); {
        // calculate where the jump destination is
        newOffset := offset + jumps[offset]
        // adjust this current jump value
        if jumps[offset] >= 3 {
            jumps[offset]--
        } else {
            jumps[offset]++
        }
        // make the jump
        offset = newOffset
        // count how many jumps
        steps++
    }
    return steps
}

func main() {
    var scanner* bufio.Scanner

    if len(os.Args[1:]) >= 1 {
        // read from file
        f, err := os.Open(os.Args[1])
        if err != nil {
            panic(err)
        }
        scanner = bufio.NewScanner(f)
    } else {
        // read from stdin
        scanner = bufio.NewScanner(os.Stdin)
    }

    jumps := make([]int, 0)

    // remove whitespace from each line, convert to int
    for scanner.Scan() {
        if jump, err := strconv.Atoi(strings.TrimSpace(scanner.Text())); err != nil {
            fmt.Println("Invalid integer: %v", scanner.Text())
            return
        } else {
            jumps = append(jumps, jump)
        }
    }

    // duplicate input for part-2
    jumps2 := make([]int, len(jumps))
    copy(jumps2, jumps)

    fmt.Printf("%v jumps\n", jumpsUntilExit(jumps))
    fmt.Printf("%v jumps\n", jumpsUntilExit2(jumps2))
}
