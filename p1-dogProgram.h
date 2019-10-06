typedef unsigned int uint;
typedef unsigned long ulong; // 32 bits 
#define ARCHIVO "dataDogs.dat"

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
	ulong* id;
	char** nombres;
	ulong size;
	ulong numero_de_datos;
	ulong last_key;
} tabla;

void menu();
void ingresar();
void ver();
void borrar();
void buscar();
void salir();
ulong hash(ulong key); // In : key ; Out : id
ulong new_hash(); // Out : key
void ir_en_linea(FILE* archivo, ulong linea);
