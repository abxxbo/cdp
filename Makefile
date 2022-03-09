CC := gcc
CFLAGS := -Wall -Wextra -pedantic -O2 -g

BUILD := bin/

.PHONY: cdp

cdp: cdp.c
	mkdir -p $(BUILD)
	$(CC) $^ $(CFLAGS) -o $(BUILD)/cdp

install: $(BUILD)/cdp
	sudo cp $^ /usr/local/bin/cdp
