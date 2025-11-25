CC = gcc
RM = rm -f
CFLAGS = -Wall -O2
LDFLAGS = -lm

HOME := $(shell echo ~$(USER))
LOCAL_BIN_PATH := $(HOME)/.local/bin
LOCAL_MAN_PATH := $(HOME)/.local/share/man/man1
BIN_ORIGINAL := sysfetch
BIN_NEW := sysfetch
BUILD_DIR := bin
SRC_DIR := src
SRC := sysfetch.c

SRC_OBJ := $(SRC_DIR)/$(SRC)
OBJ := $(BUILD_DIR)/$(BIN_ORIGINAL)

MANPAGE := sysfetch.1

.PHONY: clean build link install-man all

all: clean build link install-man

$(BUILD_DIR):
	mkdir -p $@

build: $(BUILD_DIR) $(OBJ)

$(OBJ): $(SRC_OBJ)
	$(CC) $(CFLAGS) $< -o $@

link: $(OBJ)
	mkdir -p $(LOCAL_BIN_PATH)
	ln -sf $(PWD)/$(OBJ) $(LOCAL_BIN_PATH)/$(BIN_NEW)

install-man: $(MANPAGE)
	mkdir -p $(LOCAL_MAN_PATH)
	cp $(MANPAGE) $(LOCAL_MAN_PATH)/$(MANPAGE)

clean:
	$(RM) -r $(BUILD_DIR)

