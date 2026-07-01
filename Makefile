export LANG = en_US.UTF-8

CC = gcc
CC_FLAGS = -std=c99 -pedantic -Wall -Wextra
CC_FLAGS_WARNINGS= -Werror -Wfloat-equal -Wformat=2 \
           -Wshadow -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith \
           -Wuninitialized -Wunused -Wwrite-strings

SRC_DIR = .
TESTS_DIR = tests

# FILES.c = ptr_vector.c http_request.c http_header.c main.c
# FILES.h = ptr_vector.h http_request.h http_header.h

MAIN.c = main.c
FILES.c := $(wildcard $(SRC_DIR)/*.c)
FILES.h := $(wildcard $(SRC_DIR)/*.h)
CORE_FILES.c := $(filter-out $(SRC_DIR)/$(MAIN.c), $(FILES.c))
TEST_FILES.c := $(wildcard $(TESTS_DIR)/*.c)

TARGET = main
TEST_TARGET = run_tests

.PHONY: all clean test

all: clean $(TARGET)

$(TARGET): $(FILES.c) $(FILES.h)
	 $(CC) $(CC_FLAGS) $(CC_FLAGS_WARNINGS) $(FILES.c) -o $(TARGET)

debug: $(FILES.c) $(FILES.h)
	 $(CC) $(CC_FLAGS) -g -O0 -o $(TARGET) $(FILES.c)

run: dev
	./$(TARGET)

$(TEST_TARGET): $(TEST_FILES.c) $(CORE_FILES.c) ./tests/utils.h
	$(CC) $(CC_FLAGS) -g -O0 $(TEST_FILES.c) $(CORE_FILES.c) -o $(TEST_TARGET)

test: $(TEST_TARGET)
	@echo "Running tests..."
	./$(TEST_TARGET)

clean:
	-rm $(TARGET) $(TEST_TARGET) 2>/dev/null

gdb: debug
	gdb ./$(TARGET)
