package aoc14

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

fun Int.bitsSet(): Int {
    var thisCopy = this
    var bitCount = 0
    while (thisCopy != 0) {
        thisCopy = thisCopy and (thisCopy-1); // clear the least significant bit set
        bitCount++
    }
    return bitCount
}

fun countBitsSet(hex: String) = hex.map { it.toString().toInt(16).bitsSet() }.sum()


class KnotHash(val words: List<Int>) {
    override fun toString() = words.map { "%02X".format(it) }.joinToString("")
    fun bitsSet() = words.map { (it and 0xFFFF).bitsSet() } .sum()
}

class KnotHasher(val circleSize: Int, var current: Int, var skipSize: Int) {
    var circle: MutableList<Int>
    val suffix = listOf(17, 31, 73, 47, 23)

    constructor (circleSize: Int): this(circleSize, 0, 0)
    constructor (): this(256, 0, 0)
    init {
        circle = (0..(circleSize-1)).toMutableList()
    }

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

fun main(args: Array<String>) {
    val input = 
        if (args.isNotEmpty()) {
            args[0]
        } else {
            readLine()
        }
      
    input.whenNotNullNorBlank { baseInput ->
        val knotHasher = KnotHasher()
        //val usedBlocks = (0..127).map {
        val usedBlocks = (0..127).map {
            knotHasher.hash("%s-%d".format(baseInput, it))
        }.map {
            it.bitsSet()
        }.sum()

        println("$usedBlocks blocks used")
    }
}
