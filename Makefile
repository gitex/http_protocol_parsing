export LANG = en_US.UTF-8

CC = gcc CC_FLAGS = -std=c99 -pedantic -Wall -Wextra -Werror -Wfloat-equal -Wformat=2 \
           -Wshadow -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wpointer-arith \
           -Wuninitialized -Wunused -Wwrite-strings

TARGET = main
FILES.c = ptr_vector.c http_request.c http_header.c main.c
FILES.h = ptr_vector.h http_request.h http_header.h

all: clean dev

dev: $(FILES.c) $(FILES.h)
	 gcc -o $(TARGET) $(FILES.c)

debug: $(FILES.c) $(FILES.h)
	 gcc -g -O0 -o $(TARGET) $(FILES.c)

run: dev
	./$(TARGET)

clean:
	-rm $(TARGET) 2>/dev/null

gdb: debug
	gdb ./$(TARGET)
