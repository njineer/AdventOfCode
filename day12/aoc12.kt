package aoc12

import java.lang.Exception
import java.io.File
import java.util.LinkedList

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

fun parseInput(connections: List<String>): Map<Int, List<Int>> {
    val pipes = mutableMapOf<Int, List<Int>>()

    // Capture of connections is overwritten, so capture all in comma-separated list
    val regex = Regex("(\\d+)\\s*<->\\s*(?:((?:\\w+,?\\s*)+))")

    // match the regex for each line in the input and store the results
    connections.forEach { conn ->
        regex.matchEntire(conn)?.let { match ->
            val (program, others) = match.destructured
            pipes[program.toInt()] = others.split(',').map { it.trim().toInt() } 
        } ?: throw InputException("No regex match for instr: $conn")
    }
    return pipes.toMap()
}

fun findConnections(pipes: Map<Int, List<Int>>, origin: Int): List<Int> {
    var visited = mutableSetOf<Int>()
    var queue = LinkedList<Int>()
    queue.add(origin)
    while (queue.isNotEmpty()) {
        val cur = queue.poll()
        if (visited.contains(cur)) {
            continue
        } else {
            visited.add(cur)
            queue.addAll(pipes[cur]!!)
        }
    }
    return visited.toList()

}

fun main(args: Array<String>) {
    var pipes = parseInput(readInput(args))
    var groups = mutableListOf<List<Int>>()
    println("part 1: ${findConnections(pipes, 0).size} connections to 0")

    while (pipes.isNotEmpty()) {
        val origin = pipes.toList().first().first
        val conns = findConnections(pipes, origin)
        groups.add(conns)
        pipes -= conns
    }
    println("${groups.size} groups")
}
