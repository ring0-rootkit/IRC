CFLAGS = -Wall -Wextra -pedantic -std=c99 -Iinclude -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE
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

.PHONY: run
run: bin/ircy
	@echo "Usage: make run SERVER=<server> NICK=<nick> CHANNEL=<channel>"
	@echo "Example: make run SERVER=irc.libera.chat NICK=testuser CHANNEL=test"
ifdef SERVER
ifdef NICK  
ifdef CHANNEL
	./bin/ircy -s $(SERVER) -n $(NICK) -c $(CHANNEL)
else
	@echo "Error: CHANNEL parameter required"
endif
else
	@echo "Error: NICK parameter required" 
endif
else
	@echo "Error: SERVER parameter required"
endif
