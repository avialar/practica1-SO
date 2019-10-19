#include "p1-dogProgram.h"

tabla hash_table;
sprimos primos;

int main(int argc, char* argv[]){
	int r;
	uint i;
	dogType buffer;
	ulong key = 0, s = 1, n;
	FILE *archivo;

	archivo = fopen(PRIMOS, "r");
	if(archivo == NULL){
		fprintf(stderr, "Error : No se puede leer %s\n", PRIMOS);
		return(EXIT_FAILURE);
	}

	// contar cuantos numeros hay
	
	r = fseek(archivo, 0, SEEK_END);
	if(r == -1){
		perror("fseek");
		return(EXIT_FAILURE);
	}

	primos.size = ftell(archivo) / sizeof(ulong);

	// leer los primos
	
	r = fseek(archivo, 0, SEEK_SET);
	//error

	primos.primos = (ulong *) malloc(sizeof(ulong) * primos.size);

	for(i = 0; i < primos.size; i++){
		r = fread(primos.primos+i, sizeof(ulong), 1, archivo);
	}
	
	fclose(archivo);

	
	// hash table

	hash_table.numero_de_datos = 0;
	hash_table.last_key = 0;
	
	archivo = fopen(ARCHIVO, "r");
	if(archivo == 0){ // file doesn't exist
		printf("el archivo ya no existe\n");
		archivo = fopen(ARCHIVO, "w");
		fclose(archivo);

		for(i=0; primos.primos[i] < FIRST_SIZE; i++);
		primos.cur = i;
		hash_table.id = (ulong*) calloc(primos.primos[primos.cur], sizeof(ulong));
		hash_table.nombres = (char**) calloc(primos.primos[primos.cur], sizeof(char*));
		hash_table.size = primos.primos[primos.cur];
	} else { // file exists
		// contar cuantos datos hay
		r = fseek(archivo, 0, SEEK_END);
		if(r == -1){
			perror("fseek");
			return(EXIT_FAILURE);
		}

		n = ftell(archivo) / (sizeof(ulong) + sizeof(dogType));
		for(i=0; i < primos.size && primos.primos[i] < n; i++);
		if(i == primos.size){
			fprintf(stderr, "Error : hay %lu datos pero el primo maximo que tenemos es %lu\n", n, primos.primos[i-1]);
			return(EXIT_FAILURE);
		}
		primos.cur = i;
		
		hash_table.id = (ulong*) calloc(primos.primos[primos.cur], sizeof(ulong));
		hash_table.nombres = (char**) calloc(primos.primos[primos.cur], sizeof(char*));
		hash_table.size = primos.primos[primos.cur];

		r = fseek(archivo, 0, SEEK_SET);
		//error
		
		// ingresar datos
		
		s = fread(&key, sizeof(ulong), 1, archivo);
		for(i = 0; s != 0; i++){ // reading the file
			if(i >= hash_table.size){
				sizemasmas();
			}
			hash_table.id[i] = key;
			
			if(hash_table.id[i] != 0){ // hay un dato
				hash_table.numero_de_datos++;
				if (key > hash_table.last_key){
					hash_table.last_key = key;
				}
				s = fread(&buffer, sizeof(dogType), 1, archivo); // nombre de la mascota
				hash_table.nombres[i] = (char*) malloc(32 * sizeof(char));
				for(uint j = 0; j < SIZE_GRANDE; j++){ // copy string
					hash_table.nombres[i][j] = buffer.nombre[j];
				}
			} else {
				s = fread(&buffer, sizeof(dogType), 1, archivo);
			}
			s = fread(&key, sizeof(ulong), 1, archivo);
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
	int r, buffersize, wstatus;
	ulong key, id, key2;
	dogType *mascota;
	FILE *archivo;
	pid_t pid;
	char l, command1[] = TEXT_EDITOR,
		command2[46] = "",
		buffer[211] = ""; // 32 + 32 + 20 + 16 + (20 + 13 = 33) + 8 (malo/femenino) + (22 + 5*7 = 22 + 35 = 57)
	/*
	  Nombre : 10
	  Tipo : 7
	  Edad : 7
	  Raza : 7
	  Estatura : 12
	  Peso : 7
	  Sexo : 7
	*/
	
	
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
	r = fread(&key2, sizeof(ulong), 1, archivo);
	if (r == 0){
		perror("fread");
		free(mascota);
		fclose(archivo);
		exit(EXIT_FAILURE);
	}
	if(key != key2){
		printf("/!\\ key (%ld) != key2 (%ld)\n", key, key2);
		return;
	}
	r = fread(mascota, sizeof(dogType), 1, archivo);
	if (r == 0){
		perror("fread");
		free(mascota);
		fclose(archivo);
		exit(EXIT_FAILURE);
	}
	fclose(archivo);
	printf("Nombre : %s\nTipo : %s\nEdad : %ld\nRaza : %s\nEstatura : %ld\nPeso : %.12lf\nSexo : %c\n", mascota->nombre, mascota->tipo, mascota->edad, mascota->raza, mascota->estatura, mascota->peso, mascota->sexo);

	//existe la historia clinica?
	
	r = sprintf(command2, "historias_clinicas/%lu_hc.txt", key); // 19 + 20 + 7
	if (r < 0){
		perror("sprintf");
		free(mascota);
		exit(EXIT_FAILURE);
	}
	
	archivo = fopen(command2, "r");

	if(archivo == NULL){ // tenemos que crearla
		
		archivo = fopen(command2, "w");
		if(archivo == NULL){
			perror("fopen");
			exit(EXIT_FAILURE);
		}
		char malo[] = "malo", femenino[] = "femenino", otro[] = "otro", *sexo;
		if(mascota->sexo == 'm'){
			sexo = malo;
		} else if(mascota->sexo == 'f'){
			sexo = femenino;
		} else{
			sexo = otro;
		}
		buffersize = sprintf(buffer, "Nombre : %s\nTipo : %s\nEdad : %ld\nRaza : %s\nEstatura : %ld\nPeso : %.12lf\nSexo : %s\n", mascota->nombre, mascota->tipo, mascota->edad, mascota->raza, mascota->estatura, mascota->peso, sexo);
		r = 0;
		while(r < buffersize){
			r += fwrite(&buffer[r], sizeof(char), buffersize, archivo);
		}
	}
	fclose(archivo);
	// existe
	
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
		if(pid == 0){ // hijo
			char env1[32], env2[32], env3[32], env4[64];
			
			char
				/*
				 *caml    = getenv("CAML_LD_LIBRARY_PATH"),
				 *dbus    = getenv("DBUS_SESSION_BUS_ADDRESS"),
				 */
				*display = getenv("DISPLAY"), // para X11
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
				 */
				//*pwd     = getenv("PWD"), // para abrir 
				/*
				 *shell   = getenv("SHELL"),
				 *shlvl   = getenv("SHLVL"),
				 */
				*term    = getenv("TERM"), // para consola
			  /*
			   *tmux0   = getenv("TMUX"),
			   *tmux1   = getenv("TMUX_PANE"),
			   *user    = getenv("USER"),
			   *winid   = getenv("WINDOWID"),
			   */
				*xauth   = getenv("XAUTHORITY"); // para X11
				/*
			   *xdg0    = getenv("XDG_RUNTIME_DIR"),
			   *xdg1    = getenv("XDG_SEAT"),
			   *xdg2    = getenv("XDG_SESSION_CLASS"),
			   *xdg3    = getenv("XDG_SESSION_ID"),
			   *xdg4    = getenv("XDG_SESSION_TYPE"),
			   *xdg5    = getenv("XDG_VTNR"),
			   *manp    = getenv("MANPATH");
			   */
			
			sprintf(env1, "TERM=%s", term);
			//sprintf(env2, "PWD=%s", pwd);
			sprintf(env3, "DISPLAY=%s", display);
			sprintf(env4, "XAUTHORITY=%s", xauth); // !!! > 32
			char* argv[3], *envp[] = {env1, env2, env3, env4, 0};
			argv[0] = command1; argv[1] = command2; argv[2] = 0;
			execve(command1, argv, envp); // xdg-open archivo.txt
			exit(EXIT_SUCCESS);
		} else { //padre
			while(wait(&wstatus) != pid);
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
			return 0;
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
	int r = 1;
	ulong tmp;
	dogType buffer;
	while (r != 0 && i < linea){
		r = fread(&tmp, sizeof(ulong), 1, archivo);
		r = fread(&buffer, sizeof(dogType), 1, archivo);
		i++;
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

	primos.cur++;
	
	hash_table.size = primos.primos[primos.cur];
	if(primos.cur % ( primos.size / 100) == 0){
		printf("%lu%% prev size : %lu\nnew  size : %lu\n", (ulong) (primos.cur * 1.0 / primos.size * 100), tmp.size, hash_table.size);
	}
	hash_table.id = (ulong*) calloc(hash_table.size, sizeof(ulong));
	//error
	hash_table.nombres = (char**) calloc(hash_table.size, sizeof(char*));
	//error
	//printf("prev-id = %u, prev-nombres = %u\n new-id = %u,  new-nombres = %u\n", tmp.id, tmp.nombres, hash_table.id, hash_table.nombres);
	for(i = 0; i < tmp.size; i++){
		hash_table.id[i] = tmp.id[i];
		if(tmp.id[i] != 0){
			hash_table.nombres[i] = tmp.nombres[i];
			//printf("%lu - %s\n", hash_table.id[i], hash_table.nombres[i]);
		} else {
			//printf("%lu - NULL\n", hash_table.id[i]);
		}
	}
	free(tmp.id); free(tmp.nombres);
}
