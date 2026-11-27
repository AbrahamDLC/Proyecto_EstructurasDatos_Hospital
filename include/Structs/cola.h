#ifndef COLA_H
#define COLA_H
#include <stdbool.h>
#include "erroresGrales.h"

typedef enum{
    COLA_OK=1,
    COLA_VACIA=300,
    COLA_DUPLICADO
}errorCola;

typedef struct nodoCola{
    int idPaciente;
    struct nodoCola* siguienteNodo;
}nodoCola;

typedef struct{
    nodoCola* frente;
    nodoCola* final;
    int cantidad;
}colaEspera;

int iniciarCola(colaEspera* c);
int encolar(colaEspera* c, int idPaciente);       
int desencolar(colaEspera* c, int* idPaciente);   // COLA_OK o COLA_VACIA
int verFrente(const colaEspera* c, int* idPaciente);
bool estaEnCola(const colaEspera* c, int idPaciente);
bool colaVacia(const colaEspera* c);
void liberarCola(colaEspera* c);

#endif
