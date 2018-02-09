fun main(args: Array<String>) {
    if (args.isNotEmpty()) {
        val filename = args[0]
        System.out.println(filename)
    } else {
        System.err.println("Missing input file")
    }
}

