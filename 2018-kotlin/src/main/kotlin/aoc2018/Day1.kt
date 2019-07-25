package aoc2018.days

import aoc2018.foundation.parseInput

// convert each line of input to int and append to list
fun getInput(filename: String?): MutableList<Int> {
    val inputs = mutableListOf<Int>()
    val appendInt: (String) -> Unit = { inputs.add(it.toInt()) }
    parseInput(filename, appendInt)
    return inputs
}

// sum the input list
fun day1_1(filename: String?) {
    println(getInput(filename).sum())
}

// accumulate the input list until we see a repeated sum
fun day1_2(filename: String?) {
    val seen = mutableSetOf<Int>()
    val inputs = getInput(filename)
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
