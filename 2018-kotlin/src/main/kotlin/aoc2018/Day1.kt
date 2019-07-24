package aoc2018.days

import aoc2018.foundation.parseInput

fun getInput(filename: String?): MutableList<Int> {
    val inputs = mutableListOf<Int>()
    val appendInt: (String) -> Unit = { inputs.add(it.toInt()) }
    parseInput(filename, appendInt)
    return inputs
}

fun day1_1(filename: String?) {
    println(getInput(filename).sum())
}

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
