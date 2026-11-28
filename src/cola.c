#include <stdlib.h>
#include "cola.h"

int iniciarCola(colaEspera* c){
    c->frente=NULL;
    c->final=NULL;
    c->cantidad=0;
    return COLA_OK;
}

bool colaVacia(const colaEspera* c){
    return c==NULL || c->frente==NULL;
}

bool estaEnCola(const colaEspera* c, int idPaciente){
    if(c==NULL){return false;}
    for(nodoCola* n=c->frente;n!=NULL;n=n->siguienteNodo){
        if(n->idPaciente==idPaciente){return true;}
    }
    return false;
}

int encolar(colaEspera* c, int idPaciente){
    if(c==NULL||idPaciente<=0){return ERROR_ID_INVALIDO;}
    if(estaEnCola(c,idPaciente)){return COLA_DUPLICADO;}

    nodoCola* nuevo=malloc(sizeof(nodoCola));
    if(nuevo==NULL){return ERROR_MEMORIA;}
    nuevo->idPaciente=idPaciente;
    nuevo->siguienteNodo=NULL;

    if(c->frente==NULL){
        c->frente=nuevo;
    }else{
        c->final->siguienteNodo=nuevo;
    }
    c->final=nuevo;
    c->cantidad++;
    return COLA_OK;
}

int desencolar(colaEspera* c, int* idPaciente){
    if(colaVacia(c)){return COLA_VACIA;}
    nodoCola* sale=c->frente;
    if(idPaciente!=NULL){*idPaciente=sale->idPaciente;}
    c->frente=sale->siguienteNodo;
    if(c->frente==NULL){c->final=NULL;}
    free(sale);
    c->cantidad--;
    return COLA_OK;
}

int verFrente(const colaEspera* c, int* idPaciente){
    if(colaVacia(c)){return COLA_VACIA;}
    if(idPaciente!=NULL){*idPaciente=c->frente->idPaciente;}
    return COLA_OK;
}

void liberarCola(colaEspera* c){
    if(c==NULL){return;}
    int id;
    while(desencolar(c,&id)==COLA_OK){}
}
