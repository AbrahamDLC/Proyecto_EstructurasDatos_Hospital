#include <stdlib.h>
#include "pila.h"

int iniciarPila(pilaOperaciones* p){
    p->tope=NULL;
    p->cantidad=0;
    return PILA_OK;
}

bool pilaVacia(const pilaOperaciones* p){
    return p==NULL || p->tope==NULL;
}

int apilar(pilaOperaciones* p, operacion op){
    if(p==NULL||(op.tipo!=OP_ALTA_PACIENTE&&op.tipo!=OP_BAJA_PACIENTE)||op.idPaciente<=0){
        return PILA_OPERACION_INVALIDA;
    }
    nodoPila* nuevo=malloc(sizeof(nodoPila));
    if(nuevo==NULL){return ERROR_MEMORIA;}
    nuevo->dato=op;
    nuevo->siguienteNodo=p->tope;
    p->tope=nuevo;
    p->cantidad++;
    return PILA_OK;
}

int desapilar(pilaOperaciones* p, operacion* op){
    if(pilaVacia(p)){return PILA_VACIA;}
    nodoPila* sale=p->tope;
    if(op!=NULL){*op=sale->dato;}
    p->tope=sale->siguienteNodo;
    free(sale);
    p->cantidad--;
    return PILA_OK;
}

int verTope(const pilaOperaciones* p, operacion* op){
    if(pilaVacia(p)){return PILA_VACIA;}
    if(op!=NULL){*op=p->tope->dato;}
    return PILA_OK;
}

void liberarPila(pilaOperaciones* p){
    if(p==NULL){return;}
    while(desapilar(p,NULL)==PILA_OK){}
}

int deshacerUltima(pilaOperaciones* p, listaPacientes* lista){
    operacion op;
    if(desapilar(p,&op)!=PILA_OK){return PILA_VACIA;}
    if(op.tipo==OP_BAJA_PACIENTE){return reactivarPaciente(lista,op.idPaciente);}
    return bajaDePaciente(lista,op.idPaciente);
}
