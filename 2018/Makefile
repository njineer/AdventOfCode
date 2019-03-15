#CC=g++
CC=clang++
CPPFLAGS=-std=c++17

TARGET1=part1
TARGET2=part2
TARGETS=$(TARGET1) $(TARGET2)


.PHONY: all clean run1 run2 ex1 ex2 help

all: $(TARGETS)

clean:
	-@rm $(TARGETS) *.o 2>/dev/null || true

help:
	-@echo "Options: all clean run1 run2 help"

run1: $(TARGET1)
	cat input | ./$(TARGET1)

run2: $(TARGET2)
	cat input | ./$(TARGET2)

ex1: $(TARGET1)
	cat example_input | ./$(TARGET1)

ex2: $(TARGET2)
	cat example_input | ./$(TARGET2)

$(TARGET1): $(TARGET1).o
	$(CC) $(CPPFLAGS) $^ -o $@

$(TARGET2): $(TARGET2).o
	$(CC) $(CPPFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CPPFLAGS) $^ -c
