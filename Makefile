CC = gcc
RM = rm -f
CFLAGS = -Wall -O2
LDFLAGS = -lm

HOME := $(shell echo ~$(USER))
LOCAL_BIN_PATH=$(HOME)/.local/bin
BIN_ORIGINAL=sysfetch.o
BIN_NEW=sfetch
PWD := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

.PHONY: clean build link all

clean:
	$(RM) $(PWD)sysfetch.o

build:
	$(CC) $(CFLAGS) sysfetch.c -o $(PWD)sysfetch.o

link:
	ln -sf $(PWD)$(BIN_ORIGINAL) $(LOCAL_BIN_PATH)/$(BIN_NEW)

all: clean build link

