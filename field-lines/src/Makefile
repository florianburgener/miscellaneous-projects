TARGET = program
LIBS = -lm
CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -Ofast

ifeq ($(OS),Windows_NT)
	LIBS += -lmingw32 -lSDL2main -lSDL2
else
	LIBS += -lSDL2
    CFLAGS += -fsanitize=address -fsanitize=leak
endif

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) ${CFLAGS} $(LIBS) -o $@

clean:
	rm -f *.o ${TARGET}*
