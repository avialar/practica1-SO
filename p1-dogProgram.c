#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

#include "p1-dogProgram.h"

int main(int argc, char* argv[]){
	menu();
	return EXIT_SUCCESS;
}

void menu(){
	char p;
	int r = 0;
	while (1) {
		p = 0;
		printf("1. Ingresar registro\n2. Ver registro\n3. Borrar registro\n4. Buscar\n5. Salir\n");
		while(p < '1' || p > '5'){
			p = getchar();
		}
		switch(p){
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
		printf("cualquier tecla\n"); getchar();
	}
}

void ingresar(){
	printf("ingresar\n");
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
