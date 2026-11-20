#ifndef ARBOL_BINARIO
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
void preorden(nodoPalabra *nodo);
void inorden(nodoPalabra *nodo);
void postorden(nodoPalabra *nodo);
















#endif