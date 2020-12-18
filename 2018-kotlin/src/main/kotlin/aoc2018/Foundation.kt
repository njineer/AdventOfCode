package aoc2018.foundation

import java.io.File
import kotlin.text.Regex


val numRegex = Regex("(-?\\d+)")

fun <R> String?.whenNotNullNorBlank(fn: (String) -> R): R? {
    return this?.let { receiver ->
        if (receiver.isNotBlank()) {
            fn(receiver)
        } else null
    }
}


fun parseInput(filename: String?, fn: (String) -> Unit) {
    // read the input from a file
    if (!filename.isNullOrEmpty()) {
        File(filename).forEachLine { line ->
            if (line.isNotEmpty()) {
                fn(line)
            }
        }
    } 
    // read the input from stdin
    else {
        while(true) {
            readLine().whenNotNullNorBlank { line ->
                if (line.isNotEmpty()) {
                    fn(line)
                }
            } 
            ?: break
        }
    }
}


fun parseInts(line: String): List<Int> {
    return numRegex
        .findAll(line)
        .map { it.value.toInt() }
        .toList()
}


fun <R: Any> mapInput(filename: String?, transform: (String) -> R): List<R> {
    val ml = mutableListOf<R>()
    parseInput(filename) {
        ml.add(transform(it))
    }
    return ml.toList()
}

fun collectInput(filename: String?): List<String> {
    val ml = mutableListOf<String>()
    parseInput(filename) {
        ml.add(it)
    }
    return ml.toList()
}

fun parseSingle(filename: String?): String {
    return collectInput(filename).single()
}
