#include "arbolPacientes.h"

nodoPalabra* raiz(char palabra[MAX_LONG]){
    if(palabra==NULL) return NULL;
    nodoPalabra* nuevo = malloc(sizeof(nodoPalabra));
    if(nuevo==NULL || palabra[0]=='\0') return NULL;

    strcpy(nuevo->cadena, palabra);

    nuevo->izquierda =NULL;
    nuevo->derecha=NULL;
    return nuevo;
}

int agregar(nodoPalabra* nodo, char *palabra){
    if(strcmp(palabra, nodo->cadena)>0){
        if(nodo->derecha==NULL){
            nodo->derecha=raiz(palabra);
            return 1;
        }else{
            agregar(nodo->derecha, palabra);
        }
    }else if(strcmp(palabra,nodo->cadena<0)){
        if(nodo->izquierda==NULL){
            nodo->izquierda=raiz(palabra);
            return 1;
        }else{
            agregar(nodo->izquierda, palabra);
        }
    }else{
        return 0;
    }

}