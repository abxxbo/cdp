CC := gcc
CFLAGS := -Wall -Wextra -pedantic -O2 -g \
					-lblkid

BUILD := bin

.PHONY: cdp

cdp: src/cdp.c
	mkdir -p $(BUILD)
	$(CC) $^ $(CFLAGS) -o $(BUILD)/cdp

clean:
	rm $(BUILD)

install: $(BUILD)/cdp
	sudo cp $^ /usr/local/bin/cdp
