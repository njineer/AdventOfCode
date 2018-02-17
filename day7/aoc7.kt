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

data class Program(
    val name: String, 
    var weight: Int?, 
    var parent: Program?,
    var subs: List<Program>,
    var balanced: Boolean?,
    var fullWeight: Int?
) 
{
    constructor (name: String) : this(name, null, null, listOf<Program>(), null, null)
    constructor (name: String, weight: Int) : this(name, weight, null, listOf<Program>(), null, null)
    constructor (name: String, weight: Int, subs: List<Program>) : this(name, weight, null, subs, null, null)
    // fullWeight property?
}

class Tower() {
    var programs = mutableMapOf<String, Program>()

    // set or update a program's parent/base
    fun updateParent(name: String, parent: Program): Program { 
        val p = programs.getOrPut(name) { Program(name) }    
        p.parent = parent
        return p
    }

    // set or update a program's weight
    fun updateWeight(name: String, weight: Int): Program {
        val p = programs.getOrPut(name) { Program(name) }    
        p.weight = weight
        return p
    }

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
                    updateWeight(match.groupValues[1], match.groupValues[2].toInt()).run {
                        subs = match.groupValues.last().split(',').map{ updateParent(it.trim(), this) }
                    }
                // something bizarre; should never happen
                } else {
                    throw InputException("Regex match but fewer than 3 groups ?!?!?!?: $line")
                }
            }
        } ?: throw InputException("No regex match for line: $line")
    }

    // find the base of the tower
    fun findBase(): Program {
        // start with the first program
        var program = programs.toList().first().second
        // move down the tower until parent==null
        while (true) {
            program.parent?.let {
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
    val base = tower.findBase()
    println("Tower base: ${base.name}")
}

