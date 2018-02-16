import java.io.File
import java.lang.Exception
import kotlin.text.Regex

class InputException(override var message:String): Exception(message)

data class Program(val name: String, var weight: Int?, var subs: MutableList<String>, var base: Program?) {
    constructor (name: String, weight: Int, subs: MutableList<String>): this(name, weight, subs, null)
    constructor (name: String, weight: Int): this(name, weight, mutableListOf<String>(), null)
    constructor (name: String): this(name, null, mutableListOf<String>(), null)
    fun update(other: Program) {
        if (weight == null) { weight = other.weight }
        if (subs.isEmpty()) { subs = other.subs }
    }
}


fun <R> String?.whenNotNullNorBlank(block: (String) -> R): R? {
    return this?.let { receiver ->
        if (receiver.isNotBlank()) {
            block(receiver)
        } else null
    }
}


fun MutableMap<String, Program>.add(prog: Program): Program {
    /*
    is prog in tower?
    if yes
        already seen as sub
        update with weight, subs
    elif no
        add to tower
        add subs to tower
    */
    var found = true
    val towerProg = getOrPut(prog.name) { found = false; prog }
    println("found = $found")
    if (found) {
       towerProg.update(prog)
       println("found ${prog.name}; updating")
    } else {
        println("new addition: ${prog.name}")
        towerProg.subs.forEach {
            println("\tadding sub $it")
            val sub = add(Program(it))
            sub.base = prog
            println("\tupdated base to ${sub.base}")
        }
    }
    return prog
}


fun parseLine(line: String): Program {
    val linePattern = Regex("(\\w+)\\s+\\((\\d+)\\)\\s*(->\\s+(\\w+),\\s+(\\w+),\\s+(\\w+)\\s*)?")
    linePattern.matchEntire(line)?.let { match ->
        match.groupValues.filter { it.isNotEmpty() }.let { groups ->
            return when (groups.size) {
                3 -> Program(match.groupValues[1], 
                             match.groupValues[2].toInt())
                7 -> Program(match.groupValues[1], 
                         match.groupValues[2].toInt(), 
                         match.groupValues.slice(4..6).toMutableList())
                else -> throw InputException("Expected 3 or 7 regex groups;" +  
                                  "found ${match.groupValues.size} in '$line'")
            }
        }
    } ?: throw InputException("No regex match for line: $line")
}

//fun findTowerBase(tower: MutableMap<String, Program>): Program {
//}


fun main(args: Array<String>) {
    val tower = mutableMapOf<String, Program>()
    if (args.isNotEmpty()) {
        val filename = args[0]
        File(filename).forEachLine { line ->
            parseLine(line).let {
                println(it)
                tower.add(it)
            }
        }
    } else {
        while(true) {
            readLine().whenNotNullNorBlank { line ->
                parseLine(line).let {
                    println(it)
                    tower.add(it)
                }
            } 
            ?: break
        }
    }
    println(tower)
}

