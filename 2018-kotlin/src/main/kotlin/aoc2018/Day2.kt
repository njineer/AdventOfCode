package aoc2018.days

import aoc2018.foundation.parseInput

fun countTwosAndThrees(id: String): Pair<Boolean, Boolean> {
    val counts = id
        .groupingBy { it }
        .eachCount()
        .values
        .filter { it == 2 || it == 3 }

    return Pair(counts.contains(2), counts.contains(3))
}

fun day2_1(filename: String?) {
    val counts = mutableMapOf(2 to 0, 3 to 0)
    val updateCounts: (String) -> Unit = { 
        val (two, three) = countTwosAndThrees(it)
        if (two) {
            counts.put(2, counts.getValue(2) + 1)
        }
        if (three) {
            counts.put(3, counts.getValue(3) + 1)
        }
    }
    parseInput(filename, updateCounts)

    println(counts.getValue(2)*counts.getValue(3))
}

fun day2_2(filename: String?) {
    val inputs = mutableListOf<String>()
    val append: (String) -> Unit = { inputs.add(it) }
    parseInput(filename, append)

    run earlyExit@ {
        inputs.forEachIndexed { i, s1 ->
            inputs.drop(i).forEach { s2 ->
                val diffs = s1
                    .zip(s2)
                    .mapIndexed { i, (c1, c2) -> Pair(i, c1 != c2) }
                    .filter { (_, ne) -> ne }
                    .map { (i, _) -> i }
                if (diffs.size == 1) {
                    val common = s1
                        .filterIndexed { i, _ -> i != diffs.single() }
                        // .joinToString()
                   println(common)
                   return@earlyExit
                }
            }
        }
    }
}
