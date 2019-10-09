#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>

#include "p1-dogProgram.h"

tabla hash_table;

int main(int argc, char* argv[]){
	char buffer[SIZE_LINEA];
	ulong key = 0, s = 1, zero=0;
	FILE *archivo;
	
	hash_table.id = (ulong*) calloc(10000000, sizeof(ulong));
	hash_table.nombres = (char**) calloc(10000000, sizeof(char*));
	hash_table.size = 10000000;
	hash_table.numero_de_datos = 0;
	hash_table.last_key = 0;
	
	archivo = fopen(ARCHIVO, "r");
	if(archivo == 0){ // file doesn't exist
		printf("el archivo ya no existe\n");
		archivo = fopen(ARCHIVO, "w");
		fwrite(&zero, sizeof(ulong), 1, archivo);
		fclose(archivo);
	} else { // file exists
		s = fread(&key, sizeof(ulong), 1, archivo);
		for(uint i = 0; s != 0; s = fread(&key, sizeof(ulong), 1, archivo), i++){ // reading the file
			if(i > hash_table.size){
				sizemasmas();
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
			}
			fgets(buffer, SIZE_LINEA, archivo); // nueva linea
		}
		
		fclose(archivo);
	}

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
		printf("Presione Enter.\n"); getchar(); getchar(); 
	}
}

void ingresar(){
	int r;
	dogType* new = (dogType*) malloc(sizeof(dogType));
	ulong key, id;
	FILE *archivo;
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
	fclose(archivo);
	
	free(new);
}

void ver(){
	int r;
	ulong key, id;
	dogType *mascota;
	FILE *archivo;
	char l;
	pid_t pid;
		
	printf("Hay %ld numeros presentes.\nCual es la clave de la mascota?\n", hash_table.numero_de_datos);
	r = scanf("%ld", &key);
	if (r == 0){
		perror("scanf");
		exit(EXIT_FAILURE);
	}
	id = hash(key);
	if(hash_table.id[id] == 0){
		return;
	}
	mascota = (dogType*) malloc(sizeof(dogType));
	archivo = fopen(ARCHIVO, "r");
	ir_en_linea(archivo, id);
	r = fread(&key, sizeof(ulong), 1, archivo);
	if (r == 0){
		perror("fread");
		free(mascota);
		fclose(archivo);
		exit(EXIT_FAILURE);
	}
	r = fread(mascota, sizeof(dogType), 1, archivo);
	if (r == 0){
		perror("fread");
		free(mascota);
		fclose(archivo);
		exit(EXIT_FAILURE);
	}
	fclose(archivo);
	printf("Nombre : %s\nTipo : %s\nEdad : %ld\nRaza : %s\nEstatura : %ld\nPeso : %lf\nSexo : %c\n", mascota->nombre, mascota->tipo, mascota->edad, mascota->raza, mascota->estatura, mascota->peso, mascota->sexo);

	//existe la historia clinica?
	char command2[18];
	r = sprintf(command2, "%lu_hc.txt", key);
	archivo = fopen(command2, "r");

	if(archivo == NULL){ // tenemos que crearla
		
	}
	fclose(archivo);
	printf("Quiere abrir la historia clinica de %s? [S/N]\n", mascota->nombre);
	l = 0;
	while(l != 'S' && l != 's' && l != 'N' && l != 'n'){ // [S/N]
		r = scanf("%c", &l);
		if (r == 0){
			perror("scanf");
			free(mascota);
			exit(EXIT_FAILURE);
		}
	}
	if(l == 'S' || l == 's'){ // abrir historia clinica
		pid = fork();
		if(pid == -1){
			perror("can't fork");
		}
		if(pid == 0){
			char command1[] = "/usr/bin/xdg-open";
			if (r < 0){
				perror("sprintf");
				free(mascota);
				exit(EXIT_FAILURE);
			}
			char env1[32], env2[32];
			
			char
				/*
				 *caml    = getenv("CAML_LD_LIBRARY_PATH"),
				 *dbus    = getenv("DBUS_SESSION_BUS_ADDRESS"),
				 */
				*display = getenv("DISPLAY"), // para Window Manager
				/*
				 *editor  = getenv("EDITOR"),
				 *home    = getenv("HOME"),
				 *invocid = getenv("INVOCATION_ID"),
				 *journ   = getenv("JOURNAL_STREAM"),
				 *lang    = getenv("LANG"),
				 *less    = getenv("LESSOPEN"),
				 *logn    = getenv("LOGNAME"),
				 *mail    = getenv("MAIL"),
				 *moz     = getenv("MOZ_PLUGIN_PATH"),
				 *ocaml   = getenv("OCAML_TOPLEVEL_PATH"),
				 *oldpwd  = getenv("OLDPWD"),
				 *opam    = getenv("OPAM_SWITCH_PREFIX "),
				 *path    = getenv("PATH"),
				 *prompt  = getenv("PROMPT"),
				 *pwd     = getenv("PWD"),
				 *shell   = getenv("SHELL"),
				 *shlvl   = getenv("SHLVL"),
				 */
				*term    = getenv("TERM"); // para consola
			  /*
			   *tmux0   = getenv("TMUX"),
			   *tmux1   = getenv("TMUX_PANE"),
			   *user    = getenv("USER"),
			   *winid   = getenv("WINDOWID"),
			   *xauth   = getenv("XAUTHORITY"),
			   *xdg0    = getenv("XDG_RUNTIME_DIR"),
			   *xdg1    = getenv("XDG_SEAT"),
			   *xdg2    = getenv("XDG_SESSION_CLASS"),
			   *xdg3    = getenv("XDG_SESSION_ID"),
			   *xdg4    = getenv("XDG_SESSION_TYPE"),
			   *xdg5    = getenv("XDG_VTNR"),
			   *manp    = getenv("MANPATH");
			   */
			
			sprintf(env1, "TERM=%s", term);
			sprintf(env2, "DISPLAY=%s", display);
			
			char* argv[3], *envp[] = {env1, env2, 0};
			argv[0] = command1; argv[1] = command2; argv[2] = 0;
			execve(command1, argv, envp); // xdg-open archivo.txt
			exit(EXIT_SUCCESS);
		}
	}
	free(mascota);
}

void borrar(){
	int r;
	ulong key, id;
	FILE *archivo;
	printf("Hay %ld numeros presentes.\nCual es la clave de la mascota?\n", hash_table.numero_de_datos);
	r = scanf("%ld", &key);
	if (r == 0){
		perror("scanf");
		exit(EXIT_FAILURE);
	}
	id = hash(key);
	if(hash_table.id[id] == 0){
		return;
	}
	archivo = fopen(ARCHIVO, "r+");
	ir_en_linea(archivo, id);
	hash_table.id[id] = 0;
	free(hash_table.nombres[id]);
	key = 0;
	r = fwrite(&key, sizeof(ulong), 1, archivo);
	if (r == 0){
		perror("fwrite");
		fclose(archivo);
		exit(EXIT_FAILURE);
	}
	fclose(archivo);
}

void buscar(){
	int r;
	ulong i, j;
	char buffer[SIZE_GRANDE];
	printf("Cual es el nombre de la mascota?\n");
	r = scanf("%s", buffer);
	if (r == 0){
		perror("scanf");
		exit(EXIT_FAILURE);
	}
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
	return id;
}

ulong new_hash(char* nombre){
	if(hash_table.size == hash_table.numero_de_datos){
		sizemasmas(); // hacer con numeros primos
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
}


/*
hash = datos1
tmp = datos1
hash = datos2
datos1 -> datos2
free datos1
 */
void sizemasmas(){
	uint i;
	tabla tmp;
	tmp.size = hash_table.size;
	tmp.id = hash_table.id;
	tmp.nombres = hash_table.nombres;
	
	hash_table.size += 100;
	hash_table.id = (ulong*) calloc(hash_table.size, sizeof(ulong));
	//error
	hash_table.nombres = (char**) calloc(hash_table.size, sizeof(char*));
	//error
	for(i = 0; i < tmp.size; i++){
		hash_table.id[i] = tmp.id[i];
		hash_table.nombres[i] = tmp.nombres[i];
	}
	free(tmp.id); free(tmp.nombres);
}
