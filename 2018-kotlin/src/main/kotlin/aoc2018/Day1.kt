package aoc2018.days

import aoc2018.foundation.parseInput

fun day1_1(filename: String?) {
    val inputs = mutableListOf<Int>()
    val appendInt: (String) -> Unit = { inputs.add(it.toInt()) }
    parseInput(filename, appendInt)
    println(inputs.sum())
}

fun day1_2(filename: String?) {
    println("day1.1")

}
