import java.io.File
import kotlin.text.Regex
import java.lang.Exception

class InputException(override var message:String): Exception(message)
//data class Register(val name: String, val value: Int)
//data class RegComponents(
//    val name: String, 
//    val incDec: String, 
//    val amount: Int, 
//    val compareReg: String, 
//    val compareFunc: String, 
//    val compareVal: Int
//)

infix fun Int.eq(other: Int): Boolean = this.compareTo(other) == 0
infix fun Int.ne(other: Int): Boolean = this.compareTo(other) != 0
infix fun Int.le(other: Int): Boolean = this.compareTo(other) <= 0
infix fun Int.lt(other: Int): Boolean = this.compareTo(other) < 0
infix fun Int.gt(other: Int): Boolean = this.compareTo(other) > 0
infix fun Int.ge(other: Int): Boolean = this.compareTo(other) >= 0

val comparators = mapOf(
    "==" to Int::eq,
    "!=" to Int::ne,
    "<"  to Int::lt,
    "<=" to Int::le,
    ">"  to Int::gt,
    ">=" to Int::ge
)


fun <R> String?.whenNotNullNorBlank(block: (String) -> R): R? {
    return this?.let { receiver ->
        if (receiver.isNotBlank()) {
            block(receiver)
        } else null
    }
}

fun getInput(args: Array<String>): List<String> {
    var inputs = mutableListOf<String>()

    // read the input from a file
    if (args.isNotEmpty()) {
        val filename = args[0]
        File(filename).forEachLine { line ->
            if (line.isNotEmpty()) {
                inputs.add(line.trim())
            }
        }
    } 
    // read the input from stdin
    else {
        while(true) {
            readLine().whenNotNullNorBlank { line ->
                if (line.isNotEmpty()) {
                    inputs.add(line.trim())
                }
            } 
            ?: break
        }
    }
    return inputs.toList()
}

fun parseInputs(inputs: List<String>): List<List<String>> {
    var instructions = mutableListOf<List<String>>()
    val regex = Regex("(\\w+)\\s+(inc|dec)\\s+([\\-\\+\\d]+)\\s+if\\s+(\\w+)\\s+(.+)\\s+(\\d+)")

    inputs.forEach { instr ->
        regex.matchEntire(instr)?.let {
            instructions.add(it.groupValues.drop(1)) 
            println(it.groupValues.drop(1))
        } ?: throw InputException("No regex match for instr: $instr")
        
    }
    return instructions.toList()
}



fun main(args: Array<String>) {
    parseInputs(getInput(args))
}

