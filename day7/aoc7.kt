import java.io.File
import kotlin.text.Regex

fun addToMap(str: String): Unit {
    println("Adding $str")
}

fun <R: Any> String?.whenNotNullNorBlank(block: (String) -> R): R? {
    return this?.let { r ->
        if (r.isNotBlank()) {
            block(r)
        } else null
    }
}

fun main(args: Array<String>) {
    if (args.isNotEmpty()) {
        val filename = args[0]
        val linePattern = Regex("(\\w+)\\s+\\((\\d+)\\)\\s*(->\\s+((\\w+),?\\s*)+)?")
        File(filename).forEachLine { line ->
            if (line matches linePattern) {
                println("Match")
            }
        }
    } else {
        while(true) {
            readLine().whenNotNullNorBlank {

            } 
            ?: break
        }
    }
}

