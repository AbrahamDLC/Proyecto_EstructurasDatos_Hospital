#ifndef PILA_H
#define PILA_H
#include <stdbool.h>
#include "erroresGrales.h"
#include "pacientes.h"

// PILA: bitácora de operaciones sobre pacientes para poder deshacer la última
typedef enum{
    PILA_OK=1,
    PILA_VACIA=200,
    PILA_OPERACION_INVALIDA
}errorPila;

typedef enum{
    OP_ALTA_PACIENTE=1,
    OP_BAJA_PACIENTE
}tipoOperacion;

typedef struct{
    tipoOperacion tipo;
    int idPaciente;
}operacion;

typedef struct nodoPila{
    operacion dato;
    struct nodoPila* siguienteNodo;
}nodoPila;

typedef struct{
    nodoPila* tope;
    int cantidad;
}pilaOperaciones;

int iniciarPila(pilaOperaciones* p);
int apilar(pilaOperaciones* p, operacion op);
int desapilar(pilaOperaciones* p, operacion* op);
int verTope(const pilaOperaciones* p, operacion* op);
bool pilaVacia(const pilaOperaciones* p);
void liberarPila(pilaOperaciones* p);

// Saca la última operación y la revierte: deshacer BAJA -> reactivar, deshacer Devuelve PILA_VACIA o el código de pacientes (PACIENTE_VALIDO, ID_NO_ENCONTRADO)
int deshacerUltima(pilaOperaciones* p, listaPacientes* lista);

#endif
