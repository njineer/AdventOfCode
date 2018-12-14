TARGET=day2
CC=g++
CPPFLAGS=-std=c++11

$(TARGET): day2_1.o
	$(CC) $(CPPFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CPPFLAGS) $^ -c

.PHONY: clean
clean:
	@-rm $(TARGET) *.o
