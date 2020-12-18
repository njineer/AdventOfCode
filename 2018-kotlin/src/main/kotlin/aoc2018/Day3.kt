package aoc2018.days

import aoc2018.foundation.parseInput
import aoc2018.foundation.parseInts

data class Claim(val id: Int, val x: Int, val y: Int, val width: Int, val height: Int)

val claims = mutableListOf<Claim>()
val used = mutableMapOf<Pair<Int, Int>, Int>()

fun processGrid (filename: String?) {
    parseInput(filename) { line ->
        val (id, x0, y0, width, height) = parseInts(line)
        for (x in x0 until x0+width) {
            for (y in y0 until y0+height) {
                val square = Pair(x,y)
                used.put(square, used.getOrDefault(square, 0) + 1)
            }
        }
        claims += Claim(id, x0, y0, width, height)
    }
}

fun day3_1(filename: String?) {
    processGrid(filename)

    val overlap = used
        .filterValues { it >= 2 }
        .size
    println(overlap)
}

fun day3_2(filename: String?) {
    processGrid(filename)

    val noOverlap = claims.asSequence()
        .filter claimFilter@{ claim ->
            for (x in claim.x until claim.x+claim.width) {
                for (y in claim.y until claim.y+claim.height) {
                    if (used.getValue(Pair(x,y)) > 1) {
                        return@claimFilter false
                    }
                }
            }
            return@claimFilter true
        }
        .single()

    println(noOverlap.id)
}
