CC = gcc
RM = rm -f
CFLAGS = -Wall -O2
LDFLAGS = -lm

LOCAL_BIN_PATH=/home/m0/.local/bin
BIN_ORIGINAL=sysfetch
BIN_NEW=sfetch
PWD := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

.PHONY: clean build link all

clean:
	$(RM) sysfetch

build:
	$(CC) $(CFLAGS) sysfetch.c -o sysfetch

link:
	ln -sf $(PWD)$(BIN_ORIGINAL) $(LOCAL_BIN_PATH)/$(BIN_NEW)

all: clean build link

