package aoc2018.days

import aoc2018.foundation.parseInput
import aoc2018.foundation.parseInts


fun day3_1(filename: String?) {
    val used = mutableMapOf<Pair<Int, Int>, Int>()

    parseInput(filename) { line ->
        val (_, x0, y0, width, height) = parseInts(line)
        for (x in x0 until x0+width) {
            for (y in y0 until y0+height) {
                val square = Pair(x,y)
                used.put(square, used.getOrDefault(square, 0) + 1)
            }
        }
    }

    val overlap = used
        .filterValues { it >= 2 }
        .size
    println(overlap)
}

fun day3_2(filename: String?) {

}
