package main

import (
    "fmt"
    "bufio"
    "os"
    "strings"
    "strconv"
)

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
            fmt.Println("Invalid integer: %v", scanner.Text())
            return
        } else {
            jumps = append(jumps, jump)
        }
    }
    fmt.Println(len(jumps), " jumps")

    steps := 0
    for offset := 0; offset >= 0 && offset < len(jumps); {
        offset, jumps[offset] = offset + jumps[offset], jumps[offset] + 1
        steps++
        //fmt.Printf("Step %v: jumps[%v] = %v\n", steps, offset, jumps[offset]-1)
    }
    fmt.Printf("%v jumps\n", steps)
}
