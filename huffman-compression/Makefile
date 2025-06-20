CC := gcc
CFLAGS := -std=c99 -Wall -Wextra -fsanitize=address -fsanitize=leak -MD
LIBS := -lm

BIN_PATH := bin
SRC_PATH := src

TARGET_1 := $(BIN_PATH)/compress
TARGET_2 := $(BIN_PATH)/decompress

OBJ_1 := $(patsubst $(SRC_PATH)/%.c, $(BIN_PATH)/_compress/%.o, $(shell find $(SRC_PATH) -type f -name '*.c'))
OBJ_2 := $(patsubst $(SRC_PATH)/%.c, $(BIN_PATH)/_decompress/%.o, $(shell find $(SRC_PATH) -type f -name '*.c'))

default: all

$(TARGET_1): $(OBJ_1)
	$(CC) -o $@ $(OBJ_1) $(CFLAGS) $(LIBS)

$(TARGET_2): $(OBJ_2)
	$(CC) -o $@ $(OBJ_2) $(CFLAGS) $(LIBS)

$(BIN_PATH)/_compress/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(dir $@)
	$(CC) -o $@ $(CFLAGS) -DEXEC_COMPRESS -c $<

$(BIN_PATH)/_decompress/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(dir $@)
	$(CC) -o $@ $(CFLAGS) -DEXEC_DECOMPRESS -c $<

.PHONY: all
all: $(TARGET_1) $(TARGET_2)

.PHONY: clean
clean:
	rm -rf $(BIN_PATH)

-include $(OBJ:.o=.d)
