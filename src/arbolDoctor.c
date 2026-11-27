#include <stdio.h>
#include <ctype.h>
#include "arbolDoctores.h"

int claveIndiceDoctor(const doctor* d, char* destino, size_t tam){
    if(d==NULL||destino==NULL||tam==0){return ARBOL_CLAVE_INVALIDA;}
    int w=snprintf(destino,tam,"%s %s %s",d->apellidoP,d->appellidoM,d->nombre);
    if(w<0||(size_t)w>=tam){return ARBOL_CLAVE_INVALIDA;}
    for(char* c=destino;*c;c++){*c=(char)tolower((unsigned char)*c);}
    return ARBOL_OK;
}

int indexarDoctor(arbolIndice* a, const doctor* d){
    char clave[MAX_CLAVE];
    int r=claveIndiceDoctor(d,clave,sizeof(clave));
    if(r!=ARBOL_OK){return r;}
    return insertarArbol(a,clave,d->idDoctor);
}

int indexarDoctores(arbolIndice* a, const listaDoctor* lista){
    if(a==NULL||lista==NULL){return ARBOL_CLAVE_INVALIDA;}
    for(nodoDoctor* n=lista->cabeza;n!=NULL;n=n->siguienteNodo){
        int r=indexarDoctor(a,&n->dato);
        if(r!=ARBOL_OK&&r!=ARBOL_DUPLICADO){return r;}
    }
    return ARBOL_OK;
}
