CFLAGS = -Wall -Wextra -pedantic -lm -std=c99
CFILES = $(shell find src/ -name "*.c")
CC = gcc

bin/ircy: $(CFILES)
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f bin/ircy src/*.o
