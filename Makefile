CC = gcc
RM = rm -f
CFLAGS = -Wall -O2
LDFLAGS = -lm

HOME := $(shell echo ~$(USER))
LOCAL_BIN_PATH := $(HOME)/.local/bin
BIN_ORIGINAL := sysfetch
BIN_NEW := sfetch
BUILD_DIR := bin

SRC := sysfetch.c
OBJ := $(BUILD_DIR)/$(BIN_ORIGINAL)

.PHONY: clean build link all

all: clean build link

$(BUILD_DIR):
	mkdir -p $@

build: $(BUILD_DIR) $(OBJ)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) $< -o $@

link: $(OBJ)
	mkdir -p $(LOCAL_BIN_PATH)
	ln -sf $(PWD)/$(OBJ) $(LOCAL_BIN_PATH)/$(BIN_NEW)

clean:
	$(RM) -r $(BUILD_DIR)

