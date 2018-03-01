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

    //println(charStream)

    charStream.forEach { chr ->
        if (!cancel) {

            if (inGarbage && chr != cancellation && chr != garbageEnd) {
                garbage++
            }
        
            when (chr) {
                groupStart -> {
                    if (!inGarbage) 
                        group++
                }
                groupEnd -> 
                    if (!inGarbage) 
                        score += group--
                garbageStart -> 
                    inGarbage = true
                garbageEnd ->
                    inGarbage = false
                cancellation ->
                    cancel = true
            }

        } else {
            cancel = false
        }
        //println("chr($chr), inGarbage($inGarbage), group($group), cancel($cancel), score($score)")
    }

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
