#ifndef ARBOL_PACIENTES_H
#define ARBOL_PACIENTES_H
#include "arbol.h"
#include "pacientes.h"

/* Índice de pacientes por "apellidoPaterno apellidoMaterno nombre" (en minúsculas), apunta al id. */
int claveIndicePaciente(const paciente* p, char* destino, size_t tam);
int indexarPaciente(arbolIndice* a, const paciente* p);
int indexarPacientes(arbolIndice* a, const listaPacientes* lista);   /* recorre la lista y los inserta todos */

#endif
