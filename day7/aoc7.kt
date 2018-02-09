import java.io.File
import kotlin.text.Regex

fun addToMap(str: String): Unit {
    println("Adding $str")
}

fun backslashify(str: String): String = str.replace("\\", "\\\\")

fun main(args: Array<String>) {
    if (args.isNotEmpty()) {
        val filename = args[0]
        val linePattern = Regex(backslashify("(\w+)\s+\(\d+)\)\s*(->\s+((\w+),?\s*)+)?"))
        File(filename).forEachLine { line ->
            if (line matches linePattern) {
                print("Match")
            }
        }
        println(bs.replace("\\", "\\\\"))
    } else {
        System.err.println("Missing input file")
    }
}

