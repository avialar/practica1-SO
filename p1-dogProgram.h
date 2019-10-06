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
	ulong id;
	char** nombres;
} tabla;

void menu();
void ingresar();
void ver();
void borrar();
void buscar();
void salir();
ulong hash(); // In : key, f_key ; Out : id
