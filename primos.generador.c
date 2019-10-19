#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>

#define ARCHIVO "primos.dat"

typedef unsigned int uint;

/*
  Usamos la criba de Eratostenes ( https://lmgtfy.com/?q=criba+de+erat%C3%B3stenes&s=d&t=w )
 */
int main(int argc, char *argv[]){
	if(argc < 2 || argc > 3){
		printf("USAGE\n\t%s <numero maximo> [base]\n\tnumero > 0\n\t1 < base < 37\n\tPor defecto, la base es automatica (10 = 0xA = 0xa = 012)\nPARA LA PRACTICA\n\t%s 0xAA0000\n", argv[0], argv[0]); // usage
		return EXIT_FAILURE;
	}

	long n, b = 0;
	ulong i, j, max = sqrt(UINT_MAX), c = 0;
	bool *tabla;
	char *endptr;
	FILE *archivo;

	if(argc == 3){ // base
		b = strtol(argv[2], &endptr, 10);
		if(endptr != NULL && *endptr != '\0'){
			fprintf(stderr, "Error : La base debe ser un numero!\n");
			return EXIT_FAILURE;
		}
	}
	
	n = strtol(argv[1], &endptr, b);
	if(endptr != NULL && *endptr != '\0'){
		fprintf(stderr, "Error : el numero debe ser un numero!\n");
		return EXIT_FAILURE;
	}
	if(n < 1){
		fprintf(stderr, "Error : el numero no es valido\n%ld < 1\n", n);
		return EXIT_FAILURE;
	}

	// ahora sabemos que tenemos que trabajar
	archivo = fopen(ARCHIVO, "w");
	if(archivo == NULL){
		perror("fopen");
		return EXIT_FAILURE;
	}
	tabla = (bool *) calloc(sizeof(bool), n); // todo 0
	
	for(i = 2; i < n; i++){
		if(!tabla[i]){ // solo si es primo
			c++;
			fwrite(&i, sizeof(ulong), 1, archivo);
			for(j = i; i < max && (j * i) < n; j++){ // 2 * 2, 2 * 3, 2 * 4... no son primos
				tabla[i*j] = 1;
			}
		}
	}
	printf("c = %lu\n", c);
	
	free(tabla);
	fclose(archivo);
	return EXIT_SUCCESS;
}
