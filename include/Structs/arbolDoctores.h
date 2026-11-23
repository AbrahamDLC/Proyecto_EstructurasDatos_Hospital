#ifndef ARBOL_DOCTOR
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_LONG 100

typedef struct nodoPalabraDR{
    char cadena[MAX_LONG];
    struct nodoPalabra *izquierda;//rama de la izquierda
    struct nodoPalabra *derecha; //rama de derecha
}nodoPalabraDR;

nodoPalabraDR* raiz(char palabra[MAX_LONG]);
int agregar(nodoPalabraDR *nodo, char *palabra);
nodoPalabra* buscar(nodoPalabraDR *nodo, char *cadena);
void preorden(nodoPalabraDR *nodo, nodoPalabraDR *arreglo[],int *indice);
void inorden(nodoPalabraDR *nodo, nodoPalabraDR *arreglo[], int *indice);
void postorden(nodoPalabraDR *nodo, nodoPalabraDR *arreglo[], int *indice);


#endif