# Makefile for KBS Project
CC=gcc
CFLAGS=-lm -Wall
ALL=-a
VERBOSE=-v

all: kbs.c
	$(CC) $(CFLAGS) -o kbs kbs.c

test: all
	./kbs $(ALL) $(VERBOSE) -f Files/E_013_Testdatei.csv

bsp: all
	./kbs $(ALL) $(VERBOSE) -f Files/E_013_Beispieldatei.csv