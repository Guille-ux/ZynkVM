# this is going to be my Makefile
# this is under License GPLv3 or lastest
# Copyright (c) 2025 Guillermo Leira Temes

CC = gcc
BDIR = build
FANSI = -DANSI
STANDALONE = -DSTANDALONE -nostdlib -ffreestanding
CFLAGS = -Wall
SRC = src

all: ansi

ansi: CFLAGS += FANSI
ansi:
	

alone: CFLAGS += STANDALONE
alone:
	
compile:
	gcc -o $(BDIR)/main $(SRC)/main.c

clean: 
	rm -rf $(BDIR)/*
debug: clean
debug: compile
debug: 
	gdb $(BDIR)/main run