CC = gcc
RM = rm -f
CFLAGS = -Wall -O2
LDFLAGS = -lm

LOCAL_BIN_PATH=/home/m0/.local/bin
BIN_ORIGINAL=sysfetch.o
BIN_NEW=sfetch
PWD := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
BUILD_DIR=$(PWD)bin/

.PHONY: clean build link all

clean:
	$(RM) $(BUILD_DIR)sysfetch.o

build:
	$(CC) $(CFLAGS) sysfetch.c -o $(BUILD_DIR)sysfetch.o

link:
	ln -sf $(BUILD_DIR)$(BIN_ORIGINAL) $(LOCAL_BIN_PATH)/$(BIN_NEW)

all: clean build link

