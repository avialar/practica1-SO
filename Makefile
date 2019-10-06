CC = gcc
CFLAGS = -g -Wall -std=c99
all: p1-dogProgram

p1-dogProgram: p1-dogProgram.o
	$(CC) -o $@ $<

p1-dogProgram.o: p1-dogProgram.c p1-dogProgram.h
	$(CC) -c $< -Wall

clean:
	rm p1-dogProgram *.o

.PHONY : clean p1-dogProgram
