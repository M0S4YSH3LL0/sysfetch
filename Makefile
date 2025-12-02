CC         = gcc
RM         = rm -f
CFLAGS     = -Wall -O2
LDFLAGS    = -lm

PREFIX    ?= $(HOME)/.local
BIN_DIR   := $(PREFIX)/bin
MAN_DIR   := $(PREFIX)/share/man/man1

BIN_NAME  := sysfetch
BUILD_DIR := bin
SRC_DIR   := src
SRC       := sysfetch.c

SRC_OBJ   := $(SRC_DIR)/$(SRC)
OBJ       := $(BUILD_DIR)/$(BIN_NAME)

MANPAGE   := sysfetch.1

.PHONY: all build clean install uninstall

all: clean build

$(BUILD_DIR):
	mkdir -p $@

build: $(BUILD_DIR) $(OBJ)

$(OBJ): $(SRC_OBJ)
	$(CC) $(CFLAGS) $< -o $@

install: $(OBJ) $(MANPAGE)
	install -d $(DESTDIR)$(BIN_DIR)
	install -m 755 $(OBJ) $(DESTDIR)$(BIN_DIR)/$(BIN_NAME)
	install -d $(DESTDIR)$(MAN_DIR)
	install -m 644 $(MANPAGE) $(DESTDIR)$(MAN_DIR)/$(MANPAGE)

uninstall:
	$(RM) $(DESTDIR)$(BIN_DIR)/$(BIN_NAME)
	$(RM) $(DESTDIR)$(MAN_DIR)/$(MANPAGE)

clean:
	$(RM) -r $(BUILD_DIR)

