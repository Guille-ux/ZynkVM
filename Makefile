# this is going to be my Makefile
# this is under License GPLv3 or lastest
# Copyright (c) 2025 Guillermo Leira Temes

CC = gcc
BDIR = build
FANSI = -DANSI
STANDALONE = -DSTANDALONE -nostdlib -ffreestanding
CFLAGS = -Wall

all: ansi

ansi: CFLAGS += FANSI
ansi:
	

alone: CFLAGS += STANDALONE
alone:
	


clean: 
	rm -rf $(BDIR)/*