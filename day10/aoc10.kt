package aoc9

fun <R> String?.whenNotNullNorBlank(block: (String) -> R): R? {
    return this?.let { receiver ->
        if (receiver.isNotBlank()) {
            block(receiver)
        } else null
    }
}

fun main(args: Array<String>) {
    readLine().whenNotNullNorBlank { input ->
        // default to list size of 256 if not specified
        val listSize = 
            if (args.isNotEmpty()) {
                args[0].toInt()
            } else {
                256
            }
        var list = (0..(listSize-1)).toMutableList()
        var skipSize = 0
        var current = 0
        input
            .split(",")
            .map { it.toInt() }
            .forEach { length ->
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
        println(list[0]*list[1])
    }
}

