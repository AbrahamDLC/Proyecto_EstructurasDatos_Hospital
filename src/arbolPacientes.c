#include <stdio.h>
#include <ctype.h>
#include "arbolPacientes.h"

int claveIndicePaciente(const paciente* p, char* destino, size_t tam){
    if(p==NULL||destino==NULL||tam==0){return ARBOL_CLAVE_INVALIDA;}
    int w=snprintf(destino,tam,"%s %s %s",p->apellidoPaterno,p->apellidoMaterno,p->nombre);
    if(w<0||(size_t)w>=tam){return ARBOL_CLAVE_INVALIDA;}
    for(char* c=destino;*c;c++){*c=(char)tolower((unsigned char)*c);}   /* solo minúsculas ASCII */
    return ARBOL_OK;
}

int indexarPaciente(arbolIndice* a, const paciente* p){
    char clave[MAX_CLAVE];
    int r=claveIndicePaciente(p,clave,sizeof(clave));
    if(r!=ARBOL_OK){return r;}
    return insertarArbol(a,clave,p->id);
}

int indexarPacientes(arbolIndice* a, const listaPacientes* lista){
    if(a==NULL||lista==NULL){return ARBOL_CLAVE_INVALIDA;}
    for(nodoPaciente* n=lista->cabeza;n!=NULL;n=n->siguienteNodo){
        int r=indexarPaciente(a,&n->dato);
        if(r!=ARBOL_OK&&r!=ARBOL_DUPLICADO){return r;}
    }
    return ARBOL_OK;
}
