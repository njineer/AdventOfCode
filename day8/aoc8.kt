package aoc8

import java.io.File
import kotlin.text.Regex
import java.lang.Exception

class InputException(override var message:String): Exception(message)

fun eq(a: Int, b: Int): Boolean = a == b
fun ne(a: Int, b: Int): Boolean = a != b
fun le(a: Int, b: Int): Boolean = a <= b
fun lt(a: Int, b: Int): Boolean = a < b
fun gt(a: Int, b: Int): Boolean = a > b
fun ge(a: Int, b: Int): Boolean = a >= b

val comparators = mapOf (
    "=="  to ::eq,
    "!="  to ::ne,
    "<"   to ::lt,
    "<="  to ::le,
    ">"   to ::gt,
    ">="  to ::ge
)

val operators = mapOf<String, Int.(Int)->Int> (
    "inc" to Int::plus,
    "dec" to Int::minus
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
    val regex = Regex("(\\w+)\\s+(inc|dec)\\s+([\\-\\+\\d]+)\\s+if\\s+(\\w+)\\s+(.+)\\s+([\\-\\+\\d]+)")

    inputs.forEach { instr ->
        regex.matchEntire(instr)?.let {
            instructions.add(it.groupValues.drop(1)) 
            //println(it.groupValues.drop(1))
        } ?: throw InputException("No regex match for instr: $instr")
        
    }
    return instructions.toList()
}

fun processInstructions(instructions: List<List<String>>): Map<String, Int> {
    var registers = mutableMapOf<String, Int>()
    instructions.forEach { instr ->
        val reg = instr[0]
        val op = operators.getOrElse(instr[1].toLowerCase()) { throw InputException("Expected 'inc' or 'dec'; found ${instr[1]}") }
        val amount = instr[2].toInt()
        val compareReg = instr[3]
        val compareOp = comparators.getOrElse(instr[4].toLowerCase()) { throw InputException("Expected a comparator; found ${instr[4]}") }
        val compareVal = instr[5].toInt()

        //println("reg: $reg\nop: $op\namount: $amount\ncompareReg:$compareReg\ncompareOp: $compareOp\ncompareVal: $compareVal")
       
        val compareRegVal = registers.getOrDefault(compareReg, 0)
        if (compareOp(compareRegVal, compareVal)) {
            registers.getOrDefault(reg, 0).let { 
                registers[reg] = it.op(amount)
            }
        }
    }
    return registers.toMap()
}


fun main(args: Array<String>) {
    val registers = processInstructions(parseInputs(getInput(args)))
    //registers.forEach { println(it) }
    val maxRegister = registers.maxBy { (_, value) ->  value }
    println("Max value: $maxRegister")
}

