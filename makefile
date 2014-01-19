# Makefile for KBS Project
CC=gcc
CFLAGS=-lm -Wall

all: kbs.c
	$(CC) $(CFLAGS) -o kbs kbs.c

test: all
	./kbs -v -f Files/E_013_Testdatei.csv

bsp: all
	./kbs -v -f Files/E_013_Beispieldatei.csv