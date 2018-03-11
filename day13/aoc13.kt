package aoc13

import java.lang.Exception
import java.io.File

class InputException(override var message:String): Exception(message)

fun <R> String?.whenNotNullNorBlank(block: (String) -> R): R? {
    return this?.let { receiver ->
        if (receiver.isNotBlank()) {
            block(receiver)
        } else null
    }
}

fun readInput(args: Array<String>): List<String> {
    var inputs = mutableListOf<String>()
    if (args.isNotEmpty()) {
        File(args[0]).forEachLine { line ->
            line.whenNotNullNorBlank {
                inputs.add(it.trim())
            }
        }
    } else {
        while(true) {
            readLine().whenNotNullNorBlank {
                inputs.add(it.trim())
            }
            ?: break
        }
    }
    return inputs.toList()
}

fun parseInput(inputs: List<String>): List<Int>? {
    val layers = mutableMapOf<Int, Int>()

    // match <layer depth>: <range>
    val regex = Regex("(\\d+)\\s*:\\s*(\\d+)")

    // match the regex for each line in the input and store the results
    inputs.forEach { input ->
        regex.matchEntire(input)?.let { match ->
            val (depth, range) = match.destructured.toList().map { it.toInt() }
            layers[depth] = range
        } ?: throw InputException("No regex match for instr: $input")
    }

    // fill in the 'missing' layers with range = 0
    return if (layers.isNotEmpty()) {
        (0..layers.keys.max()!!).map { layers.getOrDefault(it, 0) }
    } else {
        null
    }
}


fun main(args: Array<String>) {
    var firewall = parseInput(readInput(args))
    println(firewall)

}

