#include <time.h>
#include <sys/ioctl.h>

#include "p1-dogProgram.h"

#define ESTRUCTURAS 10000000
#define NOMBRES 1717

int main(){
	FILE *datos_a, *nombre_a;
	ulong i, j, n, key = 10000000, cols;
	int r;
	dogType tmp = {"", "", 0, "", 0, 0, 'm'};
	char *nombres[NOMBRES], buffer;
	clockid_t c = CLOCK_MONOTONIC_RAW;
	struct timespec t;
	clock_gettime(c, &t); srand(t.tv_sec);
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	cols = (w.ws_col - 8);

	setbuf(stdout, NULL);
	
	for(i = 0; i < NOMBRES; i++){
		nombres[i] = (char*) calloc(SIZE_GRANDE, sizeof(char));
	}
	
	datos_a = fopen(ARCHIVO,"w");
	nombre_a = fopen("nombresMascotas.txt", "r");
	if(datos_a == 0 || nombre_a == 0){ //error
		perror("fopen");
		exit(EXIT_FAILURE);
	}
	buffer = (char) fgetc(nombre_a);
	for(i = 0, j = 0; buffer != -1; buffer = (char) fgetc(nombre_a)){
		if(buffer == '\n'){
			i++;
			j = 0;
		} else {
			nombres[i][j] = buffer;
			j++;
		}
	}

	
	for(i = 1; i < (ESTRUCTURAS + 1); i++, rewind(nombre_a)){
		if(i % (ESTRUCTURAS / 10) == 0){
			double tmp = (i * 1.0) / ESTRUCTURAS;
			printf("\r<");
			for(j = 0; j < (tmp * cols); j += 1){
				printf("=");
			}
			for(; j < cols; j += 1){
				printf(" ");
			}
			printf("> %ld%%", (ulong) (tmp * 100));
		}
		n = rand() % NOMBRES;
		for(j = 0; nombres[n][j] != 0; j++){
			tmp.nombre[j] = nombres[n][j];
		}
		tmp.nombre[j] = 0;
		key = i;
		r = fwrite(&key, sizeof(ulong), 1, datos_a);
 		if(r == 0){
			perror("fwrite");
			exit(EXIT_FAILURE);
		}
		r = fwrite(&tmp, sizeof(dogType), 1, datos_a);
 		if(r == 0){
			perror("fwrite");
			exit(EXIT_FAILURE);
		}
		/*
		r = fwrite(&nl, sizeof(char), 1, datos_a);
		if(r == 0){
			perror("fwrite");
			exit(EXIT_FAILURE);
		}
		*/
	}

	printf("\n");
	return EXIT_SUCCESS;
}
