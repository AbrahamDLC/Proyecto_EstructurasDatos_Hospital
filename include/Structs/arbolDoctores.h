#ifndef ARBOL_DOCTORES_H
#define ARBOL_DOCTORES_H
#include "arbol.h"
#include "doctor.h"

/* Índice de doctores por "apellidoP appellidoM nombre" (en minúsculas), apunta al idDoctor. */
int claveIndiceDoctor(const doctor* d, char* destino, size_t tam);
int indexarDoctor(arbolIndice* a, const doctor* d);
int indexarDoctores(arbolIndice* a, const listaDoctor* lista);

#endif
