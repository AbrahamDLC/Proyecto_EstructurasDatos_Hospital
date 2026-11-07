#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pacientes.h"
#include "validaciones.h"


int validarPaciente(paciente p){

    if (p.id<=0)
    {
        return ERROR_ID_INVALIDO;
    }
//=====================================
    if (validaStrings(p.nombre)!=1)
    {
        return ERROR_NOMBRE_INVALIDO;
    }
//=====================================
    if (validaStrings(p.apellidoPaterno)!=1)
    {
        return ERROR_APELLIDO_VACIO;
    }
//========================================
    if (validaStrings(p.apellidoMaterno)!=1)
      {
          return ERROR_APELLIDO_VACIO;
      }    
//========================================    
    if (validaExpediente(p.numeroExpediente)!=1)
    {
        return ERROR_EXPEDIENTE_INVALIDO;
    }
    if(!validarFecha(p.fechaNaciemiento.dia,p.fechaNaciemiento.mes,p.fechaNaciemiento.anio)){
        return ERROR_FECHA_INVALIDA;
    }

    return PACIENTE_VALIDO;
    
}

//==============================================================
//LINKED LIST
int iniciarLista(listaPacientes* lista){
    lista->cabeza = NULL;
    lista->cola = NULL;
    lista->cantidad = 0;
    lista->sigId = 1;
    return PACIENTE_VALIDO;
}

void liberarLista(listaPacientes* lista){
    nodoPaciente* actual = lista->cabeza;
    while (actual != NULL) {
        nodoPaciente* sig = actual->siguienteNodo;
        free(actual);
        actual = sig;
    }
    lista->cabeza = NULL;
    lista->cola = NULL;
    lista->cantidad = 0;
}

int registrarPaciente(listaPacientes* lista, paciente p, int* id){
    p.id = lista->sigId;
    int pacienteV = validarPaciente(p);
    if (pacienteV != PACIENTE_VALIDO) { return pacienteV; }

//PACIENTES DUPLICADOS
    for (nodoPaciente* n = lista->cabeza; n != NULL; n = n->siguienteNodo) {
        if (strcmp(n->dato.numeroExpediente, p.numeroExpediente) == 0) {
            return ERROR_PACIENTE_DUPLICADO;
        }
    }

//GUARDAMOS
    p.activo = true;
    nodoPaciente* nuevo = crearNodo(p);
    if (nuevo == NULL) { return ERROR_MEMORIA; }

    if (lista->cabeza == NULL) {
        lista->cabeza = nuevo;               
    } else {
        lista->cola->siguienteNodo = nuevo;  
    }
    lista->cola = nuevo;                     

    lista->cantidad++;
    lista->sigId++;
    if (id != NULL) { *id = p.id; }
    return PACIENTE_VALIDO;
}

//BUSCAR ID
paciente* buscarID(listaPacientes* lista, int idPac){
    if (lista == NULL) { return NULL; }
    for (nodoPaciente* n = lista->cabeza; n != NULL; n = n->siguienteNodo) {
        if (n->dato.id == idPac) { return &n->dato; }
    }
    return NULL;
}

//BUSCAR EXPEDIENTE
paciente* buscarExpediente(listaPacientes* lista, const char* exp){
    if (lista == NULL || validaExpediente(exp) != 1) { return NULL; }
    for (nodoPaciente* n = lista->cabeza; n != NULL; n = n->siguienteNodo) {
        if (strcmp(n->dato.numeroExpediente, exp) == 0) { return &n->dato; }
    }
    return NULL;
}