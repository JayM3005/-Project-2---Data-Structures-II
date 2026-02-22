# Compiler settings
CC = g++
CFLAGS = -Wall -std=c++11

# The default target
all: sim

# Link the object files to create the executable
sim: main.o PriorityQueue.o FIFOQueue.o
	$(CC) $(CFLAGS) main.o PriorityQueue.o FIFOQueue.o -o sim

# Compile the main file
main.o: main.cpp PriorityQueue.hpp FIFOQueue.hpp
	$(CC) $(CFLAGS) -c main.cpp

# Compile the Priority Queue
PriorityQueue.o: PriorityQueue.cpp PriorityQueue.hpp
	$(CC) $(CFLAGS) -c PriorityQueue.cpp

# Compile the FIFO Queue
FIFOQueue.o: FIFOQueue.cpp FIFOQueue.hpp
	$(CC) $(CFLAGS) -c FIFOQueue.cpp

# Clean up generated files
clean:
	rm -f *.o sim