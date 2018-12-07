package aoc11

import java.lang.Exception
import java.io.File
import kotlin.math.abs

class DirectionException(override var message:String): Exception(message)

data class Hexagon(var x: Int, var y: Int, var z: Int) {
    operator fun plus(other: Hexagon): Hexagon {
        return Hexagon(this.x + other.x, this.y + other.y, this.z + other.z)  
    }

    fun distance(other: Hexagon): Int {
        return ( abs(x - other.x) + abs(y - other.y) + abs(z - other.z) ) / 2
    }
}

// axial coordinate relative offsets of a neighbor hexagon
val neighbors = mapOf (
    "n"  to Hexagon( 0,  1, -1),
    "ne" to Hexagon( 1,  0, -1),
    "nw" to Hexagon(-1,  1,  0),
    "s"  to Hexagon( 0, -1,  1),
    "se" to Hexagon( 1, -1,  0),
    "sw" to Hexagon(-1,  0,  1)
)

fun <R> String?.whenNotNullNorBlank(block: (String) -> R): R? {
    return this?.let { receiver ->
        if (receiver.isNotBlank()) {
            block(receiver)
        } else null
    }
}

fun main(args: Array<String>) {
    var path = ""
    if (args.isNotEmpty()) {
        File(args[0]).forEachLine {
            path += it
        }
    } else {
        while(true) {
            readLine().whenNotNullNorBlank {
                path += it
            }
            ?: break
        }
    }

    var maxDistance = 0
    var origin = Hexagon(0,0,0)
    val distance = path
        .split(',')
        .map { neighbors.getOrElse(it) { throw DirectionException("Unknown direction: $it") } }
        .reduce { acc, step -> 
            acc.distance(origin).let {
                // keep track of the furthest distance from origin
                if (it > maxDistance)
                    maxDistance = it
            }
            acc + step 
        }
        .distance(origin) // distance of the destination to the origin

    println("Drect path istance: $distance")
    println("Max distance from origin: $maxDistance")
}

