#include <stdlib.h>
#include <stdio.h>

#include "p1-dogProgram.h"

#define ESTRUCTURAS 11
#define NOMBRES 1717

int main(){
	FILE *datos_a, *nombre_a;
	ulong i, j, n, key = 11;
	int r;
	dogType tmp = {"", "", 0, "", 0, 0, 'm'};
	char nl = '\n', *s;
	
	datos_a = fopen(ARCHIVO,"w");
	nombre_a = fopen("nombresMascotas.txt", "r");
	
	for(i = 0; i < ESTRUCTURAS; i++, rewind(nombre_a)){
		n = rand() % NOMBRES;
		ir_en_linea(nombre_a, n);
		s = fgets(tmp.nombre, SIZE_GRANDE * sizeof(char), nombre_a);
		//error
		for(j = 0; j < SIZE_GRANDE; j++){
			if(tmp.nombre[j] == '\n'){
				tmp.nombre[j] = 0;
			}
		}
		if(i > 0){
			key = i;
		}
		r = fwrite(&key, sizeof(ulong), 1, datos_a);
		//error
		r = fwrite(&tmp, sizeof(dogType), 1, datos_a);
		//error
		r = fwrite(&nl, sizeof(char), 1, datos_a);
		//error
	}
	
	return EXIT_SUCCESS;
}

void ir_en_linea(FILE* archivo, ulong linea){
	ulong i = 0;
	char* s;
	char buffer[SIZE_LINEA];
	char p = '\n';
	while (i < linea){
		s = fgets(buffer, SIZE_LINEA * sizeof(char), archivo);
		if(s == NULL){
			fwrite(&p, sizeof(char), 1, archivo);
			i++;
		} else {
			i++;
		}
	}
	if(linea > 0){
		for(i = 0; buffer[i] != 0 && buffer[i] != '\n'; i++);
		if(buffer[i] == 0){
			fwrite(&p, sizeof(char), 1, archivo);
		}
	}
}
