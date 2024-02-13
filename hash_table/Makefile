.PHONY: clean

CC := gcc
CFLAGS := -g3 -Wall -Wpedantic

all:driver
driver: src/hash.c src/driver.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	@rm driver
