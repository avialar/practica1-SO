#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/ioctl.h>
#include <sys/stat.h>  // para mkdir
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>  // para cualquier tecla
#include <unistd.h>

#define ARCHIVO "dataDogs.dat"
#define PRIMOS "primos.dat"
#define SIZE_LINEA 1024
#define SIZE_GRANDE 32
#define SIZE_PEQUENO 16
#define TEXT_EDITOR "/usr/bin/nedit"
#define FIRST_SIZE 11
#define ERROR(test, funcion) \
  if (test) {                \
    funcion;                 \
    salir(EXIT_FAILURE);     \
  }
#define PROGRESSION(a, b, c, d)            \
  if (a % (b / d) == 0) {                  \
    double tmp = (a * 1.0) / b;            \
    uint j;                                \
    printf("\r<");                         \
    for (j = 0; j < (tmp * c); j += 1) {   \
      printf("=");                         \
    }                                      \
    for (; j < c; j += 1) {                \
      printf(" ");                         \
    }                                      \
    printf("> %ld%%", (ulong)(tmp * 100)); \
  }

typedef unsigned int uint;
typedef unsigned long ulong;  // 32 bits

typedef struct dogType_s {
  char nombre[32];
  char tipo[32];
  ulong edad;
  char raza[16];
  ulong estatura;
  double peso;
  char sexo;
} dogType;

typedef struct tabla_s {
  ulong* id;  // 76MB
  //	char** nombres; // 1220 MB
  ulong size;
  ulong numero_de_datos;
  ulong last_key;
} tabla;

typedef struct primos_s {
  ulong* primos;  // 5MB
  ulong cur;
  ulong size;
} sprimos;

void menu();
void ingresar();
void ver();
void borrar();
void buscar(struct termios termios_p_raw,
            struct termios termios_p_def,
            char buf[]);
void salir(int exitcode);
ulong hash(ulong key);  // In : key ; Out : id
ulong new_hash();       // Out : key
void ir_en_linea(FILE* archivo, ulong linea);
void sizemasmas();
