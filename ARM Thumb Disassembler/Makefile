CC = g++
CFLAGS = -Wall -Wextra -std=c++11
TARGET = disassembler

all: $(TARGET)

$(TARGET): main.o disassembler.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o disassembler.o

main.o: main.cpp disassembler.h
	$(CC) $(CFLAGS) -c main.cpp

disassembler.o: disassembler.cpp disassembler.h
	$(CC) $(CFLAGS) -c disassembler.cpp

clean:
	rm -f *.o $(TARGET)

test: $(TARGET)
	@echo "Running tests..."
	@echo "Test 1: Simple instruction test"
	./$(TARGET) test_files/simple.bin -v
	@echo "Test 2: Output file test"
	./$(TARGET) test_files/simple.bin -o output.asm
	@echo "All tests completed."
