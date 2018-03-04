package aoc9

import java.lang.Exception

class AsciiException(override var message:String): Exception(message)

fun <R> String?.whenNotNullNorBlank(block: (String) -> R): R? {
    return this?.let { receiver ->
        if (receiver.isNotBlank()) {
            block(receiver)
        } else null
    }
}

// Convert a string to a list of ASCII values
// Doesn't need to be a class, but hey: language features!
class Asciifier(val bytes: String) {
    companion object AsciiTable {
        val asciiTable = mapOf (
            ',' to 44,
            '0' to 48, '1' to 49,
            '2' to 50, '3' to 51,
            '4' to 52, '5' to 53,
            '6' to 54, '7' to 55,
            '8' to 56, '9' to 57
        )
        fun bytesToList(bytes: String): List<Int> {
            return bytes.map { asciiTable.getOrElse(it) { throw AsciiException("$it not in Ascii table") } }
        }
    }

}

class Hash(val listSize: Int, var current: Int, var skipSize: Int) {
    var list: MutableList<Int>
    constructor (listSize: Int): this(listSize, 0, 0)
    init {
        list = (0..(listSize-1)).toMutableList()
    }

    fun singleRound(lengths: List<Int>) {
        // default to list size of 256 if not specified
        lengths.forEach { length ->
            // amount we wrap around if we do
            val wrapAround = current + length - listSize
            // end of the non-wrap-around section to reverse
            val rangeEnd = minOf(current + length - 1, listSize - 1)
    
            // section to be reversed
            var toReverse = list.slice(current..rangeEnd)
            // add the rest of the section if we wrapped around
            if (wrapAround > 0) {
                toReverse += list.slice(0..wrapAround-1)
            }
    
            // reverse the section and overwrite the corresponding items in the original
            toReverse.asReversed().let { reversed ->
                (current..rangeEnd).forEach { i ->
                    list[i] = reversed[i-current]
                }
                if (wrapAround > 0) {
                    (0..wrapAround-1).forEach { i ->
                        list[i] = reversed[length-wrapAround+i]
                    }
                }
            }
            current = (current + length + skipSize) % listSize
            skipSize++
        }
    }

    fun reset() {
        list = (0..(listSize-1)).toMutableList()
        current = 0
        skipSize = 0
    }
}

fun main(args: Array<String>) {
    readLine().whenNotNullNorBlank { input ->
        val listSize = 
            if (args.isNotEmpty()) {
                args[0].toInt()
            } else {
                256
            }
            
        // part 1
        val lengths1 = input.split(",") .map { it.toInt() }
        val hash = Hash(listSize)
        hash.singleRound(lengths1)
        println(hash.list[0]*hash.list[1])

        hash.reset()

        val suffix = listOf(17, 31, 73, 47, 23)
        val lengths2 = Asciifier.bytesToList(input) + suffix
        println(lengths2)
        
        // part 2


    }
}

