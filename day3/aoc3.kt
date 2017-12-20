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

class Coord (val x: Int, val y: Int) {
    fun plus(other: Coord): Coord = Coord(x + other.x, y + other.y)
}

fun aoc3_part2(n: Int): Int? {
    return when {
        n <= 0 -> null
        n == 1 -> 0
        else -> {
           var spiral = mutableMapOf(Pair(0,0) to 1) 
           /*
            x = y = 1
            right until x
            up until y
            left until -x
            down until -y
            right until x + 1
            ...
           */
           0
        }
    }
}

fun main(args: Array<String>) {
    if (args.isNotEmpty()) {
        val n = args[0].toInt()
        aoc3(n)?.let { println(it) }
    }
}

