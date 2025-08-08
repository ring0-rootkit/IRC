CFLAGS = -Wall -Wextra -pedantic -std=c99 -Iinclude
CFILES = $(shell find src/ -name "*.c")
CC = gcc

bin/ircy: $(CFILES)
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) $^ -o $@ -lpthread

.PHONY: clean
clean:
	rm -f bin/ircy src/*.o

.PHONY: install
install: bin/ircy
	cp bin/ircy /usr/local/bin/

.PHONY: debug
debug: CFLAGS += -g -DDEBUG
debug: bin/ircy
