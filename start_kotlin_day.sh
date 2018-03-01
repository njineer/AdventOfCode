if [ $# -eq 0 ]; then
    echo "Missing day number."
else
    echo "starting day $1"
    mkdir day$1
    cd day$1
    touch aoc$1.kt
    echo "sources = aoc$1.kt\n\naoc$1.jar: \$(sources)\n\t@kotlinc \$^ -include-runtime -d \$@\n" > Makefile
    echo "if [ -f aoc$1.jar ]; then\n\tjava -jar aoc$1.jar \$@\nelse\n\techo \"Missing .jar\"\nfi\n"
    cd -
fi
