CC          = gcc
RM          = rm -f
CFLAGS      = -Wall -O2
LDFLAGS     = -lm

PREFIX     ?= $(HOME)/.local
BIN_DIR    := $(PREFIX)/bin
MAN_DIR    := $(PREFIX)/share/man/man1
CONFIG_DIR := $(HOME)/.config

BIN_NAME   := sysfetch
CONF_NAME  := sysfetch.conf
BUILD_DIR  := bin
CFG_DIR    := cfg
SRC_DIR    := src
SRC        := sysfetch.c

SRC_OBJ    := $(SRC_DIR)/$(SRC)
OBJ        := $(BUILD_DIR)/$(BIN_NAME)
CONF_OBJ   := $(CFG_DIR)/$(CONF_NAME)

MANPAGE    := sysfetch.1

.PHONY: all build clean config install uninstall

all: clean build config

$(BUILD_DIR):
	mkdir -p $@

build: $(BUILD_DIR) $(OBJ)
config: $(CONF_OBJ)
	mkdir -p $(CONFIG_DIR)/sysfetch
	cp $(CONF_OBJ) $(CONFIG_DIR)/sysfetch

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

