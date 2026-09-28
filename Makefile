CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -O2
TARGET = huffman
OBJECTS = main.o huffman.o huffman_tree.o priority_queue.o bitio.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

main.o: main.c huffman.h
	$(CC) $(CFLAGS) -c main.c

huffman.o: huffman.c huffman.h huffman_tree.h bitio.h
	$(CC) $(CFLAGS) -c huffman.c

huffman_tree.o: huffman_tree.c huffman_tree.h priority_queue.h
	$(CC) $(CFLAGS) -c huffman_tree.c

priority_queue.o: priority_queue.c priority_queue.h huffman_tree.h
	$(CC) $(CFLAGS) -c priority_queue.c

bitio.o: bitio.c bitio.h
	$(CC) $(CFLAGS) -c bitio.c

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean