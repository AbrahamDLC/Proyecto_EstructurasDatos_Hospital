#ifndef ARBOL_H
#define ARBOL_H
#include <stddef.h>

/* ÁRBOL BINARIO DE BÚSQUEDA usado como ÍNDICE: cada nodo guarda una clave (texto) y el id del
   registro al que apunta. El árbol no guarda datos del paciente/doctor, solo sabe dónde buscarlos.
   Orden: por clave y, si hay claves iguales, por id (así se permiten apellidos repetidos). */
#define MAX_CLAVE 100

typedef enum{
    ARBOL_OK=1,
    ARBOL_DUPLICADO=500,
    ARBOL_NO_ENCONTRADO,
    ARBOL_CLAVE_INVALIDA,
    ARBOL_ERROR_MEMORIA
}errorArbol;

typedef struct nodoArbol{
    char clave[MAX_CLAVE];
    int id;
    struct nodoArbol* izquierda;
    struct nodoArbol* derecha;
}nodoArbol;

typedef struct{
    nodoArbol* raiz;
    int cantidad;
}arbolIndice;

int iniciarArbol(arbolIndice* a);
int insertarArbol(arbolIndice* a, const char* clave, int id);
int eliminarArbol(arbolIndice* a, const char* clave, int id);
nodoArbol* buscarArbol(const arbolIndice* a, const char* clave);  
int buscarTodosArbol(const arbolIndice* a, const char* clave, int ids[], int capacidad, int* n);
int alturaArbol(const arbolIndice* a);

/* Recorridos: llenan arreglo[] (máx. capacidad) y dejan en *n cuántos nodos guardaron.
   inorden = orden alfabético por clave. */
void preordenArbol(const arbolIndice* a, const nodoArbol* arreglo[], int capacidad, int* n);
void inordenArbol(const arbolIndice* a, const nodoArbol* arreglo[], int capacidad, int* n);
void postordenArbol(const arbolIndice* a, const nodoArbol* arreglo[], int capacidad, int* n);

void liberarArbol(arbolIndice* a);

#endif
