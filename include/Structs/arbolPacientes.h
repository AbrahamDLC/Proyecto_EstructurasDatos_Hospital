#ifndef ARBOL_PACIENTE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_LONG 100

typedef struct nodoPalabra{
    char cadena[MAX_LONG];
    struct nodoPalabra *izquierda;//rama de la izquierda
    struct nodoPalabra *derecha; //rama de derecha
}nodoPalabra;

nodoPalabra* raiz(char palabra[MAX_LONG]);
int agregar(nodoPalabra *nodo, char *palabra);
nodoPalabra* buscar(nodoPalabra *nodo, char *cadena);
void preorden(nodoPalabra *nodo, nodoPalabra *arreglo[],int *indice);
void inorden(nodoPalabra *nodo, nodoPalabra *arreglo[], int *indice);
void postorden(nodoPalabra *nodo, nodoPalabra *arreglo[], int *indice);


#endif