# Makefile for Proxy Lab 
#
# You may modify this file any way you like (except for the handin
# rule). You instructor will type "make" on your specific Makefile to
# build your proxy from sources.

VPATH = src:external

CC = gcc
CFLAGS = -g -Wall
LDFLAGS = -lpthread
BUILD := build
TARGET := $(BUILD)/proxy

# notdir removes the directory names and keeps the file names only
SRCS := $(notdir $(wildcard src/*.c external/*.c))
# addprefix add the build/ prefix to the files and converts *.c to *.o
OBJS := $(addprefix $(BUILD)/,$(SRCS:.c=.o))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# % is a wildcard so foo.o gets a matching foo.c
# -I[dir] is needed for the compiler to search for files
$(BUILD)/%.o: %.c | $(BUILD)
	$(CC) $(CFLAGS) -Isrc -Iexternal -c $< -o $@

$(BUILD):
	mkdir -p $@

# Creates a tarball in ../proxylab-handin.tar that you can then
# hand in. DO NOT MODIFY THIS!
handin:
	(make clean; cd ..; tar cvf $(USER)-proxylab-handin.tar proxylab-handout --exclude tiny --exclude nop-server.py --exclude proxy --exclude driver.sh --exclude port-for-user.pl --exclude free-port.sh --exclude ".*")

clean:
	rm -fr *~ *.o proxy core *.tar *.zip *.gzip *.bzip *.gz build

