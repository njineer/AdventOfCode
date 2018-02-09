import kotlin.math.abs as abs

fun aoc3(n: Int): Int? {
    return when {
        n <= 0 -> null
        n == 1 -> 0
        else -> {
            // find side length of the square n is on (via nearest greater odd square)
            var sideLength = 1
            var lowerRight = sideLength * sideLength // larget value in current square
            while (lowerRight < n) {
                sideLength += 2
                lowerRight = sideLength*sideLength
            }

            // find out how far n is from the center value of the side it is on
            val distanceFromCenterOfSide = (0..3).map { lowerRight - (sideLength-1)*it - (sideLength/2) } // center value of each side
                                                 .map { abs(n - it) }                                     // distance of n from each side's center value
                                                 .min()!!                                                 // min distance (only null if n == 1)

            // Any number in this square is sideLength/2 away from [1] in X or Y direction.
            // In the other X or Y direction, use n's distance from the center of n's side of this square.
            sideLength/2 + distanceFromCenterOfSide
        }
    }
}

// (x,y) coordinates in the spiral
data class Coord (val x: Int, val y: Int)
operator fun Coord.plus(other: Coord): Coord = Coord(x + other.x, y + other.y)

// get the value at a coord (or 0 if it isn't populate yet) from the spiral with coord's (x,y)
fun MutableMap<Coord, Int>.get(x: Int, y: Int): Int = getOrDefault(Coord(x,y), 0)

// movement direction as spiral is built
enum class Direction { RIGHT, UP, LEFT, DOWN }

fun aoc3_part2(n: Int): Int? {
    var result: Int?
    if (n <= 0) {
        result = null
    } else {
        var spiral = mutableMapOf(Coord(0,0) to 1) 
        var cur = 1 // (0,0)
        // current x,y when moving about the spiral
        var x = 0
        var y = 0
        // max x,y for the current level/layer of the spiral
        var x_max = x + 1
        var y_max = y + 1

        // start by moving right from (0,0) 
        var dir = Direction.RIGHT

        // move counter-clockwise until we find or exceed n
        while (cur <= n) {
            //println("[cur($cur), x($x), y($y), x_max($x_max), y_max($y_max)]")

            when (dir) {
                // move right until we hit max x for this layer, then go up
                Direction.RIGHT -> {
                    if (x < x_max) {
                        ++x
                    } else {
                        dir = Direction.UP
                        ++y
                    }
                }
                // move up until we hit max y for this layer, then go left
                Direction.UP -> {
                    if (y < y_max) {
                        ++y
                    } else {
                        dir = Direction.LEFT
                        --x
                    }
                }
                // move left until we hit min x for this layer, then go down
                Direction.LEFT -> {
                    if (x > -x_max) {
                        --x
                    } else {
                        dir = Direction.DOWN
                        --y
                    }
                }
                // move down until we hit min y for this layer, then go right (and continue into the next layer)
                Direction.DOWN -> {
                    if (y > -y_max) {
                        --y
                    } else {
                        dir = Direction.RIGHT
                        ++x
                        ++x_max
                        ++y_max
                    }
                }
            }

            // get the sum of the adjacent values in the spiral
            cur = when (dir) {
                // the relative coords that are populated when moving in each direction
                Direction.RIGHT -> arrayOf(Coord(0,1), Coord(-1,1), Coord(-1,0), Coord(1,1))
                Direction.UP -> arrayOf(Coord(-1,-1), Coord(-1,0), Coord(-1,1), Coord(0,-1))
                Direction.LEFT -> arrayOf(Coord(-1,-1), Coord(0,-1), Coord(1,-1), Coord(1,0))
                Direction.DOWN -> arrayOf(Coord(0,1), Coord(1,-1), Coord(1,0), Coord(1,1))
            }.map { it + Coord(x,y) }           // absolute coords
             .map { (x,y) -> spiral.get(x,y) }  // get the values at these coords
             .sum()                             // sum -> new value
            
            // place newly calculated value
            spiral.put(Coord(x,y), cur)

            //println("{new: ($x, $y) = $cur}")
            //println("$dir -> ${Coord(x,y)}, cur=$cur")
        }
        result = cur // found n or the next largest value
    }
    return result
}

fun main(args: Array<String>) {
    if (args.isNotEmpty()) {
        val n = args[0].toInt()
        aoc3(n)?.let { println(it) }
        aoc3_part2(n)?.let { println(it) }
    }
}

