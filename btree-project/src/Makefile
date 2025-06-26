TARGET = program
LIBS = -lm -lssl -lcrypto
CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic
CFLAGS += -fsanitize=address -fsanitize=leak

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) $(LIBS) -o $@

# BEGIN : Tests

Unity.o: tests/Unity/unity.c tests/Unity/unity.h
	$(CC) $(CFLAGS) -c $< -o $@

BPTreeTests.o: tests/BPTreeTests.c
	$(CC) $(CFLAGS) -c $< -o $@

make_run_tests: Unity.o BPTreeTests.o Array.o BPTree.o
	$(CC) $^ $(CFLAGS) $(LIBS) -o tests_exec
	./tests_exec || true

run_tests: make_run_tests clean

# END : Tests

clean:
	rm -f *.o ${TARGET}* tests_exec
