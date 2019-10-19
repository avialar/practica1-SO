CC = gcc
CFLAGS = -g -Wall
LIBS = #-lncurses

all: p1-dogProgram datos.generador primos.generador

p1-dogProgram: p1-dogProgram.o
	$(CC) -o $@ $< $(LIBS)

datos.generador: datos.generador.o
	$(CC) -o $@ $< $(LIBS)

primos.generador: primos.generador.o
	$(CC) -o $@ $<

p1-dogProgram.o: p1-dogProgram.c p1-dogProgram.h
	$(CC) -c $< $(CFLAGS)

datos.generador.o: datos.generador.c p1-dogProgram.h
	$(CC) -c $< $(CFLAGS)

primos.generador.o: primos.generador.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm -r p1-dogProgram datos.generador primos.generador *.o *.dat historias_clinicas

.PHONY : clean p1-dogProgram
