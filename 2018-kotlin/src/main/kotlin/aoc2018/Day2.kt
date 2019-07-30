package aoc2018.days

import aoc2018.foundation.parseInput

// count the frequency of each char in id
// return whether any char has a frequency of 2 or 3
fun countTwosAndThrees(id: String): Pair<Boolean, Boolean> {
    val counts = id
        .groupingBy { it }
        .eachCount()
        .values
        .filter { it == 2 || it == 3 }

    return Pair(counts.contains(2), counts.contains(3))
}

// count the number of inputs that have chars with frequencies of 2 or 3
// return the multiplied counts
fun day2_1(filename: String?) {
    val counts = mutableMapOf(2 to 0, 3 to 0)
    parseInput(filename) {
        val (two, three) = countTwosAndThrees(it)
        if (two) {
            counts.put(2, counts.getValue(2) + 1)
        }
        if (three) {
            counts.put(3, counts.getValue(3) + 1)
        }
    }

    println(counts.getValue(2)*counts.getValue(3))
}

fun day2_2(filename: String?) {
    val inputs = mutableListOf<String>()
    parseInput(filename) { inputs.add(it) }

    // compare each input
    inputs.forEachIndexed { i, s1 ->
        inputs.drop(i).forEach { s2 ->
            // find the indices of differing characters
            val diffs = s1
                .zip(s2)
                .mapIndexed { i, (c1, c2) -> Pair(i, c1 != c2) }
                .filter { (_, ne) -> ne }
                .map { (i, _) -> i }
            // result will have only one diff
            if (diffs.size == 1) {
                // print the matching characters and exit
                val common = s1.filterIndexed { i, _ -> i != diffs.single() }
                println(common)
                return
            }
        }
    }
}
