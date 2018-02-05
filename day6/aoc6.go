package main

import (
    "fmt"
    "os"
    "bufio"
)

func main() {

    var scanner *bufio.Scanner

    if args := os.Args[1:]; len(args) > 0 {
        file, err := os.Open(args[0])
        if err != nil {
            fmt.Printf("Error opening file: %v", args[0])
            os.Exit(2)
        }
        defer file.Close()

        scanner = bufio.NewScanner(file)
    } else {
        scanner = bufio.NewScanner(os.Stdin)
    }
    scanner.Scan()
    initAlloc = scanner.Text()

    fmt.Println("Advent of Code: Day 6")
    fmt.Printf("%v\n", layout)
}
