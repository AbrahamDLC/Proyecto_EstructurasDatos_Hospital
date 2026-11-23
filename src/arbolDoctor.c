#include "arbolDoctores.h"

nodoPalabraDR* raiz(char palabra[MAX_LONG]){
    if(palabra==NULL) return NULL;
    nodoPalabraDR* nuevo = malloc(sizeof(nodoPalabraDR));
    if(nuevo==NULL || palabra[0]=='\0') return NULL;

    strcpy(nuevo->cadena, palabra);

    nuevo->izquierda =NULL;
    nuevo->derecha=NULL;
    return nuevo;
}

int agregar(nodoPalabraDR* nodo, char *palabra){
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

void preorden(nodoPalabraDR *nodo, nodoPalabraDR *arreglo[], int *indice){
    if (nodo!=NULL){
        arreglo[*indice]=nodo;
        (*indice)++;
        preorden(nodo->izquierda, arreglo, indice);
        preorden(nodo->derecha, arreglo, indice);
    }
}

void inorden(nodoPalabraDR *nodo, nodoPalabraDR *arreglo[], int *indice){
    if(nodo!=NULL){
        inorden(nodo->izquierda, arreglo, indice);
        arreglo[*indice]=nodo;
        (*indice)++;
        inorden(nodo->derecha,arreglo, indice);
    }
}

void postorden(nodoPalabraDR *nodo, nodoPalabraDR *arreglo[], int *indice){
    if (nodo!=NULL)
    {
        postorden(nodo->izquierda, arreglo, indice);
        postorden(nodo->derecha, arreglo, indice);
        arreglo[*indice]=nodo;
        (*indice)++;
    }
    
}

nodoPalabraDR* buscar(nodoPalabraDR *nodo, char *cadena){
    if(nodo==NULL)return NULL;
    if(strcmp(cadena, nodo->cadena)==0){
        return nodo;
    }else if(strcmp(cadena, nodo->cadena)>0){
        return buscar(nodo->derecha, cadena);
    }else{
        return buscar(nodo->izquierda, cadena);
    }
}
