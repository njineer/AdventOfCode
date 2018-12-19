CC=g++
CPPFLAGS=-std=c++17

TARGET1=day2_1
TARGET2=day2_2
TARGETS=$(TARGET1) $(TARGET2)


.PHONY: all clean run1 run2

all: $(TARGETS)

clean:
	-@rm $(TARGETS) *.o 2>/dev/null || true

run1: $(TARGET1)
	cat d2.in | ./$(TARGET1)

run2: $(TARGET2)
	cat d2.in | ./$(TARGET2)

$(TARGET1): $(TARGET1).o
	$(CC) $(CPPFLAGS) $^ -o $@

$(TARGET2): $(TARGET2).o
	$(CC) $(CPPFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CPPFLAGS) $^ -c
