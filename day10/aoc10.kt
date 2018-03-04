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
                val overlap = current + length - listSize
                val rangeEnd = minOf(current + length - 1, listSize - 1)
                println("current ($current) length ($length), overlap ($overlap), rangeEnd ($rangeEnd)")

                var toReverse = list.slice(current..rangeEnd)
                if (overlap > 0) {
                    toReverse += list.slice(0..overlap-1)
                }

                toReverse.asReversed().let { reversed ->
                    (current..rangeEnd).forEach { i ->
                        list[i] = reversed[i-current]
                    }
                    if (overlap > 0) {
                        (0..overlap-1).forEach { i ->
                            list[i] = reversed[length-overlap+i]
                        }
                    }
                }
                current = (current + length + skipSize) % listSize
                skipSize++
            }
        println(list[0]*list[1])
    }
}

