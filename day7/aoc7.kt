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

class Tower(var parents: MutableMap<String, String?>, var weights: MutableMap<String, Int?>) {
    fun updateParent(name: String, parent: String) {
        parents.plusAssign(name to parent)
    }
    fun updateWeight(name: String, weight: Int) {
        weights.plusAssign(name to weight)
    }

    fun parseInput(line: String): Unit {
        val linePattern = Regex("(\\w+)\\s+\\((\\d+)\\)\\s*(->\\s+(\\w+),\\s+(\\w+),\\s+(\\w+)\\s*)?")
        linePattern.matchEntire(line)?.let { match ->
            match.groupValues.filter { it.isNotEmpty() }.let { groups ->
                return when (groups.size) {
                    3 -> updateWeight(match.groupValues[1], match.groupValues[2].toInt())
                    7 -> {
                        updateWeight(match.groupValues[1], match.groupValues[2].toInt())
                        match.groupValues.slice(4..6).forEach {
                            updateParent(it,match.groupValues[1])
                        }
                    }
                    else -> throw InputException("Expected 3 or 7 regex groups;" +  
                                      "found ${match.groupValues.size} in '$line'")
                }
            }
        } ?: throw InputException("No regex match for line: $line")
    }

    fun findBase(): String {
        var program = parents.toList().first().first
        while (true) {
            parents.get(program)?.let {
                program = it
            }?: break
        }
        return program
    }
}


fun main(args: Array<String>) {
    val tower = Tower(mutableMapOf<String, String?>(), mutableMapOf<String, Int?>())
    if (args.isNotEmpty()) {
        val filename = args[0]
        File(filename).forEachLine { line ->
            if (line.isNotEmpty()) {
                tower.parseInput(line)
            }
        }
    } else {
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

