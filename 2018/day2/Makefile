CC=g++
CPPFLAGS=-std=c++11

TARGET1=day2_1
TARGET2=day2_2
TARGETS=$(TARGET1) $(TARGET2)

$(TARGET1): day2_2.o
	$(CC) $(CPPFLAGS) $^ -o $@

$(TARGET2): day2_1.o
	$(CC) $(CPPFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CPPFLAGS) $^ -c

.PHONY: clean
clean:
	-@rm $(TARGETS) *.o 2>/dev/null || true
