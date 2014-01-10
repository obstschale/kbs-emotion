# Makefile for KBS Project
CC=gcc
CFLAGS=-I

all: kbs.c
	$(CC) -o kbs kbs.c
	./kbs