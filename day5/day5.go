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
    for offset := 0; offset >= 0 && offset < len(jumps); {
        newOffset := offset + jumps[offset]
        jumps[offset]++
        offset = newOffset
        steps++
    }
    return steps
}

func jumpsUntilExit2(jumps []int) int {
    steps := 0
    for offset := 0; offset >= 0 && offset < len(jumps); {
        //fmt.Printf("Step %v: jumps[%v] = %v -> ", steps, offset, jumps[offset])
        newOffset := offset + jumps[offset]
        if jumps[offset] >= 3 {
            jumps[offset]--
        } else {
            jumps[offset]++
        }
        //fmt.Printf("jumps[%v] = %v; next = %v\n", offset, jumps[offset], newOffset)
        offset = newOffset
        steps++
    }
    return steps
}

func main() {
    var scanner* bufio.Scanner
    if len(os.Args[1:]) >= 1 {
        f, err := os.Open(os.Args[1])
        if err != nil {
            panic(err)
        }
        scanner = bufio.NewScanner(f)
    } else {
        scanner = bufio.NewScanner(os.Stdin)
    }
    jumps := make([]int, 0)
    for scanner.Scan() {
        if jump, err := strconv.Atoi(strings.TrimSpace(scanner.Text())); err != nil {
            //fmt.Println("Invalid integer: %v", scanner.Text())
            return
        } else {
            jumps = append(jumps, jump)
        }
    }

    jumps2 := make([]int, len(jumps))
    copy(jumps2, jumps)
    fmt.Printf("%v jumps\n", jumpsUntilExit(jumps))
    fmt.Printf("%v jumps\n", jumpsUntilExit2(jumps2))
}
