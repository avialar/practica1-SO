#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "p1-dogProgram.h"

tabla hash_table;

void test_hash(){
	printf("Hash table :\nsize : %ld\nnumero de datos : %ld\nlast key : %ld\n", hash_table.size, hash_table.numero_de_datos, hash_table.last_key);
	for (ulong i = 0; i < hash_table.size; i++){
		printf("id[%ld] : key = %ld, nombre = \"%s\"\n", i, hash_table.id[i], hash_table.nombres[i]);
	}
}

int main(int argc, char* argv[]){
	char buffer[SIZE_LINEA];
	ulong key = 0, s = 1, zero=0;
	FILE *archivo;
	
	hash_table.id = (ulong*) calloc(11, sizeof(ulong));
	hash_table.nombres = (char**) calloc(11, sizeof(char*));
	hash_table.size = 11;
	hash_table.numero_de_datos = 0;
	hash_table.last_key = 0;
	
	archivo = fopen(ARCHIVO, "r");
	if(archivo == 0){ // file doesn't exist
		printf("el archivo ya no existe\n");
		archivo = fopen(ARCHIVO, "w");
		fwrite(&zero, sizeof(ulong), 1, archivo);
		fclose(archivo);
	} else { // file exists
		for(uint i = 0; s != 0; s = fread(&key, sizeof(ulong), 1, archivo), i++){ // reading the file
			if(i > hash_table.size){
				hash_table.size++; // hacer esto pero con un numero primo
			}
						
			hash_table.id[i] = key;
			
			if(hash_table.id[i] != 0){ // hay un dato
				hash_table.numero_de_datos++;
				if (key > hash_table.last_key){
					hash_table.last_key = key;
				}
				s = fread(buffer, sizeof(char), 32, archivo); // nombre de la mascota
				hash_table.nombres[i] = (char*) malloc(32 * sizeof(char));
				for(uint j = 0; j < SIZE_GRANDE; j++){ // copy string
					hash_table.nombres[i][j] = buffer[j];
				}
				printf("nombre : %s\n", buffer);
			}
			fgets(buffer, SIZE_LINEA, archivo); // nueva linea
		}
		
		fclose(archivo);
	}

	test_hash();
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
	ulong key, id;
	FILE *archivo;
	char c;
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

	key = new_hash(new->nombre);
	id = hash(key);
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
	archivo = fopen(ARCHIVO, "r+");
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
	fwrite(new, sizeof(dogType), 1, archivo);
	/*
	c = '\n';
	fwrite(&c, sizeof(char), 1, archivo);
	*/
	fclose(archivo);
	
	free(new);
}

void ver(){
	int r;
	ulong key, id;
	dogType *mascota;
	FILE *archivo;
	printf("Hay %ld numeros presentes.\nCual es la clave de la mascota?\n", hash_table.numero_de_datos);
	r = scanf("%ld", &key);
	//error
	id = hash(key);
	if(hash_table.id[id] == 0){
		return;
	}
	mascota = (dogType*) malloc(sizeof(dogType));
	archivo = fopen(ARCHIVO, "r");
	ir_en_linea(archivo, id);
	r = fread(&key, sizeof(ulong), 1, archivo);
	r = fread(mascota, sizeof(dogType), 1, archivo);
	//error
	fclose(archivo);
	printf("Nombre : %s\nTipo : %s\nEdad : %ld\nRaza : %s\nEstatura : %ld\nPeso : %lf\nSexo : %c\n", mascota->nombre, mascota->tipo, mascota->edad, mascota->raza, mascota->estatura, mascota->peso, mascota->sexo);
	free(mascota);
}

void borrar(){
	int r;
	ulong key, id;
	FILE *archivo;
	printf("Hay %ld numeros presentes.\nCual es la clave de la mascota?\n", hash_table.numero_de_datos);
	r = scanf("%ld", &key);
	//error
	id = hash(key);
	if(hash_table.id[id] == 0){
		return;
	}
	archivo = fopen(ARCHIVO, "r+");
	ir_en_linea(archivo, id);
	key = 0;
	r = fwrite(&key, sizeof(ulong), 1, archivo);
	//error
	fclose(archivo);
}

void buscar(){
	int r, s;
	ulong i, j;
	char buffer[SIZE_GRANDE];
	printf("Cual es el nombre de la mascota?\n");
	r = scanf("%s", buffer);
	//error
	for (i = 0; i < hash_table.size; i++){
		if(hash_table.id[i] != 0){
			for(j = 0; j < SIZE_GRANDE && buffer[j] != 0 && hash_table.nombres[i][j] != 0 && buffer[j] == hash_table.nombres[i][j]; j++){
			}
			if(buffer[j] == '\0'){
				printf("key : %ld - nombre : %s\n", hash_table.id[i], hash_table.nombres[i]);
			}
		}
	}
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

ulong new_hash(char* nombre){
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
	hash_table.nombres[id] = (char*) malloc(SIZE_GRANDE * sizeof(char));
	for(uint j = 0; j < SIZE_GRANDE; j++){ // copy string
		hash_table.nombres[id][j] = nombre[j];
	}
	return key;
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
