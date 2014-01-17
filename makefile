# Makefile for KBS Project
CC=gcc
CFLAGS=-lm -Wall

all: kbs.c
	$(CC) $(CFLAGS) -o kbs kbs.c
	./kbs