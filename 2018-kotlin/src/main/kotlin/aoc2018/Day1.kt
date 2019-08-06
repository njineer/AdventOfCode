package aoc2018.days

import aoc2018.foundation.mapInput

// sum the input list
fun day1_1(filename: String?) {
    val inputs = mapInput(filename) { it.toInt() }
    println(inputs.sum())
}

// accumulate the input list until we see a repeated sum
fun day1_2(filename: String?) {
    val seen = mutableSetOf<Int>()
    val inputs = mapInput(filename) { it.toInt() }
    var sum = 0

    while (true) {
        inputs.forEach { i ->
            sum += i
            if (seen.contains(sum)) {
                println(sum)
                return
            } else { 
                seen.add(sum)
            }
        }
    }
}
