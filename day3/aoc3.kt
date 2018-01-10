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

//fun MutableMap<Coord, Int>.get(x: Int, y: Int): Int = getOrDefault(Coord(x,y), 0)
fun MutableMap<Coord, Int>.get(x: Int, y: Int): Int {
    val result = getOrDefault(Coord(x,y), 0)
    println("\tgetOr0: ($x, $y) = $result")
    return result
}

fun aoc3_part2(n: Int): Int? {
    return when {
        n <= 0 -> null
        n == 1 -> 2
        else -> {
            var spiral = mutableMapOf(Coord(0,0) to 1) 
            var cur = 1 // (0,0)
            var x = 0
            var y = 0
            var x_max = x + 1
            var y_max = y + 1

            while (cur < n) {
                println("--- cur($cur), x($x), y($y), x_max($x_max), y_max($y_max)")
                // right
                while (x++ < x_max) {
                    val tmp = arrayOf(Coord(0,1), Coord(-1,1), Coord(-1,0), Coord(1,1)).map { it + Coord(x,y) } 
                                                                                       .map { (x,y) -> spiral.get(x,y) }
                    cur = tmp.sum()
                    spiral.put(Coord(x,y), cur)
                    println("{new: ($x, $y) = $cur}")
                    println("R-> ${Coord(x,y)}, cur=$tmp, sum=$cur")
                }
                --x
                // up
                while (y++ < y_max) {
                    val tmp = arrayOf(Coord(-1,-1), Coord(-1,0), Coord(-1,1), Coord(0,-1)).map { it + Coord(x,y) } 
                                                                                          .map { (x,y) -> spiral.get(x,y) }
                    cur = tmp.sum()
                    spiral.put(Coord(x,y), cur)
                    println("{new: ($x, $y) = $cur}")
                    println("U-> ${Coord(x,y)}, cur=$tmp, sum=$cur")
                }
                --y
                // left
                while (x-- > -x_max) {
                    val tmp = arrayOf(Coord(-1,-1), Coord(0,-1), Coord(1,-1), Coord(1,0)).map { it + Coord(x,y) } 
                                                                                         .map { (x,y) -> spiral.get(x,y) }
                    cur = tmp.sum()
                    spiral.put(Coord(x,y), cur)
                    println("{new: ($x, $y) = $cur}")
                    println("L-> ${Coord(x,y)}, cur=$tmp, sum=$cur")
                }
                ++x
                // down
                while (y-- > -y_max) {
                    val tmp = arrayOf(Coord(0,1), Coord(1,-1), Coord(1,0), Coord(1,1)).map { it + Coord(x,y) } 
                                                                                      .map { (x,y) -> spiral.get(x,y) }
                    cur = tmp.sum()
                    spiral.put(Coord(x,y), cur)
                    println("{new: ($x, $y) = $cur}")
                    println("D-> ${Coord(x,y)}, cur=$tmp, sum=$cur")
                }
                ++y
                ++x_max
                ++y_max
                if (x_max > 20 || y_max > 20)
                    break
            }
            cur
        }
    }
}

fun main(args: Array<String>) {
    if (args.isNotEmpty()) {
        val n = args[0].toInt()
        aoc3(n)?.let { println(it) }
        aoc3_part2(n)?.let { println(it) }
    }
}

