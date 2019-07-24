package aoc2018.foundation

import java.io.File

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
