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
        val listSize = 256
        var list = (0..255).toList()
        var skipSize = 0
        var current = 0
        input
            .split(",")
            .map { it.toInt() }
            .forEach { length ->
                
            }
    }
}

