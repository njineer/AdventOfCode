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
            val distanceFromCenterOfSide = (0..3).map { lowerRight - (sideLength-1)*it - (sideLength/2)} // center value of each side
                                                 .map { abs(n - it) }                                    // distance of n from each side's center value
                                                 .min()!!                                                // min distance (only null if n == 1)

            sideLength/2 + distanceFromCenterOfSide
        }
    }
}

fun main(args: Array<String>) {
    if (args.isNotEmpty()) {
        val n = args[0].toInt()
        aoc3(n)?.let { println(it) }
    }
}

