CFLAGS = -Wall -Wextra -pedantic -std=c99
CFILES = $(shell find src/ -name "*.c")
CC = gcc

bin/ircy: $(CFILES)
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) $^ -o $@ -lpthread

.PHONY: clean
clean:
	rm -f bin/ircy src/*.o
