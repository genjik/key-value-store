CC := gcc
CFLAGS := -Wall -std=c99 -g

LIB_SOURCES := lib/src/allocator.o lib/src/rbt.o
SOURCES := src/main.o

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

main: $(LIB_SOURCES) $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -rf main src/*.o lib/src/*.o
