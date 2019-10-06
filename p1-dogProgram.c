#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "p1-dogProgram.h"

tabla hash_table;

int main(int argc, char* argv[]){
	//char *s, buffer[SIZE];
	//char *endptr;
	//FILE *archivo;
	
	hash_table.id = (ulong*) malloc(11 * sizeof(ulong));
	hash_table.nombres = (char**) malloc(11 * sizeof(char*));
	hash_table.size = 11;
	hash_table.numero_de_datos = 0;
	hash_table.last_key = 0;

	/* no funciona
	
	archivo = fopen(ARCHIVO, "r");
	if(archivo == 0){ // file doesn't exist
		printf("el archivo ya no existe\n");
		archivo = fopen(ARCHIVO, "w");
		fclose(archivo);
	} else { // file exists
		for(uint i = 0; s != 0; s = fgets(buffer, sizeof(ulong), archivo), i++){ // reading the file
			if(i > hash_table.size){
				hash_table.size++; // hacer esto pero con un numero primo
			}
			
			hash_table.id[i] = (ulong) strtol(s, &endptr, 10);
			//error
			printf("key : %ld\n", hash_table.id[i]);
			if(hash_table.id[i] != 0){ // hay un dato
				s = fgets(buffer, 32*sizeof(char), archivo); // nombre de la mascota
				hash_table.nombres[i] = (char*) malloc(32 * sizeof(char));
				for(uint j = 0; j < 32; j++){ // copy string
					hash_table.nombres[i][j] = buffer[j];
				}
				printf("nombre : %s\n", s);
			}
			s = fgets(buffer, SIZE, archivo); // nueva linea
		}
		
		fclose(archivo);
	}

	*/
	
	menu();
	return EXIT_SUCCESS;
}

void menu(){
	char p;
	while (1) {
		p = 0;
		printf("1. Ingresar registro\n2. Ver registro\n3. Borrar registro\n4. Buscar\n5. Salir\n");
		while(p < '1' || p > '5'){
			p = getchar();
		}
		switch(p){ // check si p es 1, 2, 3, 4 o 5
		case '1':
			ingresar();
			break;
		case '2':
			ver();
			break;
		case '3':
			borrar();
			break;
		case '4':
			buscar();
			break;
		case '5':
			salir();
			break;
		default:
			perror("getchar");
			exit(EXIT_FAILURE);
		}
		printf("cualquier tecla\n"); getchar(); getchar();
	}
}

void ingresar(){
	int r;
	dogType* new = (dogType*) malloc(sizeof(dogType));
	printf("Cual es el nombre de la mascota?\n");
	r = scanf("%s", new->nombre);
	if (r == 0){
		perror("scanf");
		free(new);
		exit(EXIT_FAILURE);
	}
	printf("Cual es su tipo?\n");
	r = scanf("%s", new->tipo);
	if (r == 0){
		perror("scanf");
		free(new);
		exit(EXIT_FAILURE);
	}
	printf("Cual es su edad?\n");
	r = scanf("%lu", &new->edad);
	if (r == 0){
		perror("scanf");
		free(new);
		exit(EXIT_FAILURE);
	}
	printf("Cual es su raza?\n");
	r = scanf("%s", new->raza);
	if (r == 0){
		perror("scanf");
		free(new);
		exit(EXIT_FAILURE);
	}
	printf("Cual es su estatura (en cm)?\n");
	r = scanf("%lu", &new->estatura);
	if (r == 0){
		perror("scanf");
		free(new);
		exit(EXIT_FAILURE);
	}
	printf("Cual es su peso (en Kg)?\n");
	r = scanf("%lf", &new->peso);
	if (r == 0){
		perror("scanf");
		free(new);
		exit(EXIT_FAILURE);
	}
	printf("Cual es su sexo?\n");
	getchar();
	r = scanf("%c", &new->sexo);
	if (r == 0){
		perror("scanf");
		free(new);
		exit(EXIT_FAILURE);
	}

	// printf("\nMascota :\n%s %s %lu %s %lu %lf %c\n", new->nombre, new->tipo, new->edad, new->raza, new->estatura, new->peso, new->sexo);

	ulong key = new_hash();
	ulong id = hash(key);
	if(hash_table.id[id] == key){
		printf("Key : %lu\n", key);
	} else {
		fprintf(stderr, "Error in new_hash\n");
		free(new);
		exit(EXIT_FAILURE);
	}
	
	/*
	  Escribir en un archivo
	 */
	FILE *archivo = fopen(ARCHIVO, "r+");
	if (archivo == 0){
		perror("fopen");
		free(new);
		exit(EXIT_FAILURE);
	}
	ir_en_linea(archivo, id);
	r = fwrite(&key, sizeof(ulong), 1, archivo);
	if (r == 0){
		perror("fwrite");
		free(new);
		fclose(archivo);
		exit(EXIT_FAILURE);
	}
	fwrite(new->nombre, 32 * sizeof(char), 1, archivo);
	fwrite(new->tipo, 32 * sizeof(char), 1, archivo);
	fwrite(&new->edad, sizeof(ulong), 1, archivo);
	fwrite(new->raza, 16 * sizeof(char), 1, archivo);
	fwrite(&new->estatura, sizeof(ulong), 1, archivo);
	fwrite(&new->peso, sizeof(double), 1, archivo);
	fwrite(&new->sexo, sizeof(char), 1, archivo);
	char c = '\n';
	fwrite(&c, sizeof(char), 1, archivo);
	fclose(archivo);
	
	free(new);
}

void ver(){
	printf("ver\n");
}

void borrar(){
	printf("borrar\n");
}

void buscar(){
	printf("buscar\n");
}

void salir(){
	exit(EXIT_SUCCESS);
}


// returns id ; hash_table[id] == key dice si existe
ulong hash(ulong key){
	ulong id = key % hash_table.size, id1 = id;
	while (hash_table.id[id] != key){
		id = (id+1) % hash_table.size;
		if (id == id1){ // no hay el id en la tabla
			return id;
		}
	}
	// id[id] == key
	return id;
}

ulong new_hash(){
	if(hash_table.size == hash_table.numero_de_datos){
		hash_table.size++; // hacer con numeros primos
	}
	hash_table.numero_de_datos++;
	hash_table.last_key++;
	ulong key = hash_table.last_key;
	ulong id = key % hash_table.size;
	while (hash_table.id[id] != 0){
		id = (id+1) % hash_table.size;
	}
	
	hash_table.id[id] = key; 
	return key;
}

void ir_en_linea(FILE* archivo, ulong linea){
	ulong i = 0;
	char* s;
	char buffer[SIZE];
	while (i < linea){
		s = fgets(buffer, SIZE, archivo);
		if(s == NULL){
			char p = '\n';
			fwrite(&p, sizeof(char), 1, archivo);
			i++;
		} else { 
			i++;
		}
	}
}
