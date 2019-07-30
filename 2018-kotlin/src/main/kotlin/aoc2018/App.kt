package aoc2018

import aoc2018.days.*

fun usage() {
    println("usage: aoc2018 DAY PART [INPUT FILE]")
}

fun main(args: Array<String>) {

    val solvers = mapOf(
        1 to arrayOf(::day1_1, ::day1_2),
        2 to arrayOf(::day2_1, ::day2_2),
        3 to arrayOf(::day3_1, ::day3_2)
    )

    if (args.size >= 2) {
        val (day, part) = args.take(2).map { it.toInt() }

        solvers.get(day)?.getOrNull(part-1)?.let { fn ->
            val input = if (args.size == 3) {
                args[2]
            } else {
                null
            }

            fn(input)
        } 
        ?: println("Day $day.$part not implemented.")
    }
    else {
        usage()
        println(args[0])
    }

}
