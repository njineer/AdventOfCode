import java.io.File
import kotlin.text.Regex

fun addToMap(str: String): Unit {
    println("Adding $str")
}

fun main(args: Array<String>) {
    if (args.isNotEmpty()) {
        val filename = args[0]
        val linePattern = Regex("(\\w+)\\s+\\(\\d+)\\)\\s*(->\\s+((\\w+),?\\s*)+)?")
        File(filename).forEachLine { line ->
            if (line matches linePattern) {
                print("Match")
            }
        }
    } else {
        System.err.println("Missing input file")
    }
}

