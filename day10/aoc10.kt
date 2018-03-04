package aoc9

fun <R> String?.whenNotNullNorBlank(block: (String) -> R): R? {
    return this?.let { receiver ->
        if (receiver.isNotBlank()) {
            block(receiver)
        } else null
    }
}

class Hash(val listSize: Int, var current: Int, var skipSize: Int) {
    val list: MutableList<Int>
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
}

fun main(args: Array<String>) {
    readLine().whenNotNullNorBlank { input ->
        val lengths = input.split(",") .map { it.toInt() }
        val hash = Hash(256)
        hash.singleRound(lengths)
        println(hash.list[0]*hash.list[1])
    }
}

