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

data class Program(val name: String, val weight: Int) {
    constructor (name: String, weight: Int, subs: List<String>): this(name, weight)
}

fun parseLine(line: String): Program {
    val linePattern = Regex("(\\w+)\\s+\\((\\d+)\\)\\s*(->\\s+(\\w+),\\s+(\\w+),\\s+(\\w+)\\s*)?")
    linePattern.matchEntire(line)?.let { match ->
        match.groupValues.filter { it.isNotEmpty() }.let { groups ->
            return when (groups.size) {
                3 -> {println("3: $line")
                Program(match.groupValues[1], 
                             match.groupValues[2].toInt())
                            }
                7 -> {println("7: $line")
                    Program(match.groupValues[1], 
                             match.groupValues[2].toInt(), 
                             match.groupValues.slice(4..6))
                    }
                else -> throw InputException("Expected 3 or 7 regex groups;" +  
                                  "found ${match.groupValues.size} in '$line'")
            }
        }
    } ?: throw InputException("No regex match for line: $line")
}


fun main(args: Array<String>) {
    val tower = mutableMapOf<String, Program>()
    if (args.isNotEmpty()) {
        val filename = args[0]
        val linePattern = Regex("(\\w+)\\s+\\((\\d+)\\)\\s*(->\\s+(\\w+),\\s+(\\w+),\\s+(\\w+)\\s*)?")
        File(filename).forEachLine { line ->
            parseLine(line).let {
                tower.put(it.name, it)
            }
        }
    } else {
        while(true) {
            readLine().whenNotNullNorBlank { line ->
                parseLine(line).let {
                    tower.put(it.name, it)
                }
            } 
            ?: break
        }
    }
    println(tower)
}

