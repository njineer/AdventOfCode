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

fun parseInput(inputs: List<String>): Map<Int, Int>? {
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
    return layers.toMap()
}

// send a packet starting at time=0
//      return getting-caught-severity
fun sendPacket(firewall: Map<Int, Int>) = 
    firewall.map { (layer, range) ->
        // when starting at t=0, the packet will reach layer i at t=i
        // if the scanner is at [0] at that time: caught
        if (layer % (2*(range-1)) == 0) {
            range * layer
        } else {
            0
        }
    }.sum()

// brute-force sending at packet at each delay value until not caught
//      assuming there is a solution; infinite otherwise
fun delayPacket(firewall: Map<Int, Int>): Int {
    var delay = 0
    while(
        firewall.filter { (layer, range) -> 
            (layer + delay) % (2*(range-1)) == 0
        }.isNotEmpty()
    ) delay++

    return delay
}


fun main(args: Array<String>) {
    parseInput(readInput(args))?.let { firewall ->
        //println(firewall)
        println("Severity: ${sendPacket(firewall)}")
        println("Delay to not get caught: ${delayPacket(firewall)}")
    }
}

