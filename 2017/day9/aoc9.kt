package aoc9

import java.io.File

fun <R> String?.whenNotNullNorBlank(block: (String) -> R): R? {
    return this?.let { receiver ->
        if (receiver.isNotBlank()) {
            block(receiver)
        } else null
    }
}


fun scoreCharStream(charStream: String): Pair<Int, Int> {
    // delimiters
    val groupStart = '{'
    val groupEnd = '}'
    val garbageStart = '<'
    val garbageEnd = '>'
    val cancellation = '!'

    var inGarbage = false
    var cancel = false
    var group = 0
    var score = 0
    var garbage = 0

    charStream.forEach { chr ->
        // if the last character wasn't a cancellation
        if (!cancel) {
            // count garbage if we are in garbage and 
            //     this isn't a cancellation or the end of the garbage
            if (inGarbage && chr != cancellation && chr != garbageEnd) {
                garbage++
            }
            when (chr) {
                groupStart -> {
                    if (!inGarbage) 
                        // track how deep in the hierarchy we are
                        group++
                }
                groupEnd -> 
                    if (!inGarbage) 
                        // score based on the hierarchy depth and 'pop' back a level
                        score += group--
                garbageStart -> 
                    inGarbage = true
                garbageEnd ->
                    inGarbage = false
                cancellation ->
                    cancel = true
            }

        } else {
            // this char is canceled
            cancel = false
        }
    }
    //return the total score and theamount of garbage
    return Pair(score, garbage)
}

fun main(args: Array<String>) {
    var charStream = ""
    if (args.isNotEmpty()) {
        File(args[0]).forEachLine {
            charStream = charStream + it
        }
    } else {
        while(true) {
            readLine().whenNotNullNorBlank {
                charStream = charStream + it
            }
            ?: break
        }
    }

    val (score, garbage) = scoreCharStream(charStream)
    println("score = $score, garbage = $garbage")
}
