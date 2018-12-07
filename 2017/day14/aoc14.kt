package aoc14

import kotlin.math.pow

fun <R> String?.whenNotNullNorBlank(block: (String) -> R): R? {
    return this?.let { receiver ->
        if (receiver.isNotBlank()) {
            block(receiver)
        } else null
    }
}

fun <T, R> List<T>.mapBlocks(blockSize: Int, f: (List<T>) -> R): List<R> {
    var results = listOf<R>()

    val fullBlocks = this.size / blockSize
    val blockRemainder = this.size % blockSize

    repeat(fullBlocks) { i ->
        results = results + f(this.slice((i*blockSize)..(i*blockSize + blockSize-1)))
    }

    if (blockRemainder != 0) {
        results = results + f(this.drop(this.size-blockRemainder))
    }

    return results
}

// count the number of bits set in an Int
fun Int.bitsSet(): Int {
    var thisCopy = this
    var bitCount = 0
    while (thisCopy != 0) {
        thisCopy = thisCopy and (thisCopy-1); // clear the least significant bit set
        bitCount++
    }
    return bitCount
}

// determine if the bit at index is set
fun Int.bitAt(index: Int): Boolean = ((this shr index) and 1) != 0

// passthrough if boolean is non-null; false if otherwise
fun Boolean?.falseIfNull(): Boolean = if (this != null) this else false

// Using Int because Kotlin support for Short/Byte operations is experimental at the moment,
//      and it allows non-generic class
// wordSize defaults to 8 because all inputs are ASCII values < 256
class KnotHash(val words: List<Int>, val wordSize: Int=8) {
    val mask = 2f.pow(wordSize).toInt()-1
    val size: Int 
        get() = words.size * wordSize

    // TODO: make format string wordSize dependent
    override fun toString() = words.map { "%02X".format(it) }.joinToString("")

    // count the number of bits set
    fun bitsSet() = words.map { (it and mask).bitsSet() } .sum()

    // check if the bit at a particular index is set
    fun bitAt(index: Int): Boolean? =
        // make sure the index is in range
        if (index >= 0 && index < wordSize*words.size) {
            // find the word, then the bit within the word
            words[index / wordSize].bitAt(wordSize - (index % wordSize) - 1)
        } else 
            null
}

class KnotHasher(val circleSize: Int, var current: Int, var skipSize: Int) {
    var circle: MutableList<Int>
    val suffix = listOf(17, 31, 73, 47, 23)

    constructor (circleSize: Int): this(circleSize, 0, 0)
    constructor (): this(256, 0, 0)
    init {
        circle = (0..(circleSize-1)).toMutableList()
    }

    // reset internal vars between hashes
    // TODO: make these temp/non-member vars?
    fun reset() {
        circle = (0..(circleSize-1)).toMutableList()
        current = 0
        skipSize = 0
    }

    fun singleRound(lengths: List<Int>) {
        // default to circle size of 256 if not specified
        lengths.forEach { length ->
            // amount we wrap around if we do
            val wrapAround = current + length - circleSize
            // end of the non-wrap-around section to reverse
            val rangeEnd = minOf(current + length - 1, circleSize - 1)
    
            // section to be reversed
            var toReverse = circle.slice(current..rangeEnd)
            // add the rest of the section if we wrapped around
            if (wrapAround > 0) {
                toReverse += circle.slice(0..wrapAround-1)
            }
    
            // reverse the section and overwrite the corresponding items in the original
            toReverse.asReversed().let { reversed ->
                (current..rangeEnd).forEach { i ->
                    circle[i] = reversed[i-current]
                }
                if (wrapAround > 0) {
                    (0..wrapAround-1).forEach { i ->
                        circle[i] = reversed[length-wrapAround+i]
                    }
                }
            }
            current = (current + length + skipSize) % circleSize
            skipSize++
        }
    }


    fun hash(lengths: List<Int>): KnotHash {
        // 64 rounds
        repeat(64) {
            singleRound(lengths)
        }
        // xor each block of 16 values
        val result = KnotHash(
            circle.mapBlocks(16) { 
                it.reduce { 
                    acc, x -> acc xor x 
                } 
            // hex string output
            }
        )
        reset()
        return result
    }

    fun hash(input: String) = hash(input.map { it.toInt() } + suffix)

}

fun groupOnes(hashes: List<KnotHash>): Map<Pair<Int, Int>, Int> {
    var groupIndex = 0
    var groups = mutableMapOf<Pair<Int, Int>, Int>()
    // for each row
    hashes.forEachIndexed { row, hash ->
        // for each column
        (0..(hash.size-1)).forEach { col ->
            // if this square is used
            if (hash.bitAt(col).falseIfNull()) {
                // is the west and/or north neighbor grouped?
                var westGroup = groups.get(Pair(row, col-1))
                var northGroup = groups.get(Pair(row-1, col))
                when {
                    // add to west group; merge north into west
                    westGroup != null && northGroup != null -> {
                        groups.put(Pair(row, col), westGroup)
                        groups.filterValues { it == northGroup} 
                              .mapValuesTo(groups) { westGroup }
                    }
                    // add to west group
                    westGroup != null -> groups.put(Pair(row, col), westGroup)
                    // add to north group
                    northGroup != null -> groups.put(Pair(row, col), northGroup)
                    // add to west group; merge north group into west
                    else -> groups.put(Pair(row, col), groupIndex++)
                }
            }
        }
    }
    return groups.toMap()
}

fun main(args: Array<String>) {
    val input = 
        if (args.isNotEmpty()) {
            args[0]
        } else {
            readLine()
        }
      
    input.whenNotNullNorBlank { baseInput ->
        val knotHasher = KnotHasher()
        val hashes = (0..127).map {
            knotHasher.hash("%s-%d".format(baseInput, it))
        }
        
        val usedBlocks = hashes.map { it.bitsSet() }.sum()
        println("$usedBlocks blocks used")

        val groups = groupOnes(hashes)
        val numGroups = groups.values.distinct().size
        println("$numGroups regions")
    }
}
