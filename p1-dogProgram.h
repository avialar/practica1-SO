#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h> // para mkdir

#define ARCHIVO "dataDogs.dat"
#define PRIMOS "primos.dat"
#define SIZE_LINEA 1024
#define SIZE_GRANDE 32
#define SIZE_PEQUENO 16
#define TEXT_EDITOR "/usr/bin/emacs"
#define FIRST_SIZE 11

typedef unsigned int uint;
typedef unsigned long ulong; // 32 bits 

typedef struct dogType_s{
	char nombre[32];
	char tipo[32];
	ulong edad;
	char raza[16];
	ulong estatura;
	double peso;
	char sexo;
} dogType;

typedef struct tabla_s{
	ulong* id;
	char** nombres;
	ulong size;
	ulong numero_de_datos;
	ulong last_key;
} tabla;

typedef struct primos_s{
	ulong* primos;
	ulong cur;
	ulong size;
} sprimos;

void menu();
void ingresar();
void ver();
void borrar();
void buscar();
void salir();
ulong hash(ulong key); // In : key ; Out : id
ulong new_hash(); // Out : key
void ir_en_linea(FILE* archivo, ulong linea);
void sizemasmas();
