import java.io.File
import java.lang.Exception
import kotlin.text.Regex

class InputException(override var message:String): Exception(message)

fun <R> String?.whenNotNullNorBlank(block: (String) -> R): R? {
    return this?.let { receiver ->
        if (receiver.isNotBlank()) {
            block(receiver)
        } else null
    }
}

class Tower() {
    var parents = mutableMapOf<String, String?>()
    var weights = mutableMapOf<String, Int?>()
    var fullWeights = mutableMapOf<String, Int?>()
    var balance = mutableMapOf<String, Boolean>()

    // set or update a program's parent/base
    fun updateParent(name: String, parent: String) = parents.plusAssign(name to parent)

    // set or update a program's weight
    fun updateWeight(name: String, weight: Int) = weights.plusAssign(name to weight)

    // set or update a program's full weight (including it's sub-towers
    fun updateFullWeight(name: String, weight: Int) = fullWeights.plusAssign(name to weight)

    // set or udpate whether a program's disc is balanced


    fun parseInput(line: String): Unit {
        // Capture of subprograms is overwritten, so capture all in comma-separated list
        val linePattern = Regex("(\\w+)\\s+\\((\\d+)\\)\\s*(?:->\\s+((?:\\w+,?\\s*)+))?")
        // match the regex
        linePattern.matchEntire(line)?.let { match ->
            // filter the empty groups for the shorter matches
            match.groupValues.filter { it.isNotEmpty() }.let { groups ->
                // program + weight
                if(groups.size == 3) {
                    updateWeight(match.groupValues[1], match.groupValues[2].toInt())
                } 
                // program + weight + subs
                else if ( groups.size > 3) {
                    updateWeight(match.groupValues[1], match.groupValues[2].toInt())
                    match.groupValues.last().split(',').map{ it.trim() }.forEach { prog ->
                        updateParent(prog, match.groupValues[1])
                    }
                // something bizarre; should never happen
                } else {
                    throw InputException("Regex match but fewer than 3 groups ?!?!?!?: $line")
                }
            }
        } ?: throw InputException("No regex match for line: $line")
    }

    // find the base of the tower
    fun findBase(): String {
        // start with the first program
        var program = parents.toList().first().first
        // move down the tower until parent==null
        while (true) {
            parents.get(program)?.let {
                program = it
            }?: break
        }
        return program
    }
}


fun main(args: Array<String>) {
    val tower = Tower()

    // read the input from a file
    if (args.isNotEmpty()) {
        val filename = args[0]
        File(filename).forEachLine { line ->
            if (line.isNotEmpty()) {
                tower.parseInput(line)
            }
        }
    } 
    // read the input from stdin
    else {
        while(true) {
            readLine().whenNotNullNorBlank { line ->
                if (line.isNotEmpty()) {
                    tower.parseInput(line)
                }
            } 
            ?: break
        }
    }
    println("Tower base: ${tower.findBase()}")
}

