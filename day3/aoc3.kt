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

            // corner values of this layer
            val distanceFromCenterOfSide = (0..3).map { lowerRight - (sideLength-1)*it - (sideLength/2) } // center value of each side
                                                 .map { abs(n - it) }                                     // distance of n from each side's center value
                                                 .min()!!                                                 // min distance (only null if n == 1)

            // Any number in this square is sideLength/2 away from [1] in X or Y direction.
            // In the other X or Y direction, use n's distance from the center of n's side of this square.
            sideLength/2 + distanceFromCenterOfSide
        }
    }
}

data class Coord (val x: Int, val y: Int)
operator fun Coord.plus(other: Coord): Coord = Coord(x + other.x, y + other.y)

fun MutableMap<Coord, Int>.get(x: Int, y: Int): Int = getOrDefault(Coord(x,y), 0)

enum class Direction { RIGHT, UP, LEFT, DOWN }

fun aoc3_part2(n: Int): Int? {
    var result: Int?
    if (n <= 0) {
        result = null
    } else {
        var spiral = mutableMapOf(Coord(0,0) to 1) 
        var cur = 1 // (0,0)
        var x = 0
        var y = 0
        var x_max = x + 1
        var y_max = y + 1
        var dir = Direction.RIGHT

        while (cur <= n) {
            //println("[cur($cur), x($x), y($y), x_max($x_max), y_max($y_max)]")

            when (dir) {
                Direction.RIGHT -> {
                    if (x < x_max) {
                        ++x
                    } else {
                        dir = Direction.UP
                        ++y
                    }
                }
                Direction.UP -> {
                    if (y < y_max) {
                        ++y
                    } else {
                        dir = Direction.LEFT
                        --x
                    }
                }
                Direction.LEFT -> {
                    if (x > -x_max) {
                        --x
                    } else {
                        dir = Direction.DOWN
                        --y
                    }
                }
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

            cur = when (dir) {
                Direction.RIGHT -> arrayOf(Coord(0,1), Coord(-1,1), Coord(-1,0), Coord(1,1))
                Direction.UP -> arrayOf(Coord(-1,-1), Coord(-1,0), Coord(-1,1), Coord(0,-1))
                Direction.LEFT -> arrayOf(Coord(-1,-1), Coord(0,-1), Coord(1,-1), Coord(1,0))
                Direction.DOWN -> arrayOf(Coord(0,1), Coord(1,-1), Coord(1,0), Coord(1,1))
            }.map { it + Coord(x,y) }
             .map { (x,y) -> spiral.get(x,y) }
             .sum()
            
            spiral.put(Coord(x,y), cur)
            //println("{new: ($x, $y) = $cur}")

            //println("$dir -> ${Coord(x,y)}, cur=$cur")
        }
        result = cur
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

