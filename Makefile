CC = gcc
CSCOPE = cscope
CFLAGS += -Wall -Werror
UNAME_S := $(shell uname -s)

BADGERTRAPOBJS := src/badger-trap.o\

.PHONY: all
all: badger-trap

badger-trap: $(BADGERTRAPOBJS)
	$(CC) $(CFLAGS) $(BADGERTRAPOBJS) -o $@ 

src/%.o: src/%.c src/*.h
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm -rf src/*.o badger-trap
