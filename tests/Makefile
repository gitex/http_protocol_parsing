CC = gcc
CC_FLAGS = -std=c99 -pedantic -Wall -Wextra

FILES.h := $(wildcard *.h)
TESTS = test_vector

.PHONY: run clean

all: $(TESTS)

test_vector: test_ptr_vector.c $(FILES.h) ../ptr_vector.c
	$(CC) -g3 -O0 $(CC_FLAGS) $^ -o $@

run: $(TESTS)
	@for t in $(TESTS); do ./$$t; done

clean:
	-rm $(TESTS) 2>/dev/null

