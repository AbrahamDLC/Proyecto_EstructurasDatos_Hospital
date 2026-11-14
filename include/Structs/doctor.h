#ifndef DOCTORES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "especialidades.h"
#include "erroresGrales.h"
#include "validaciones.h"


typedef enum{
    DOCTOR_VALIDO=1,
    ERROR_DR_NOENCONTRADO,
    ERROR_ID_DR_INVALIDO,
    ERROR_CEDULA_INVALIDA

}errorDoctor;



typedef struct doctor{
    int idDoctor;
    int idEspecialidad;
    int pacientesAsignados;
    char nombre[15];
    char apellidoP[15];
    char appellidoM[15];
    char cedula[9];
    bool activo;

}doctor;


typedef struct nodoDoctor{
    doctor dato;
    struct nodoDoctor* siguienteNodo;
}nodoDoctor;


typedef struct{
    nodoDoctor* cabeza;
    nodoDoctor* cola;
    int cantidad;
    int sigId;
}listaDoctor;

//MÉTODOS
int validarDr(doctor p);
int registrarDr(listaDoctor* lista, doctor d, int* drID);
int iniciarListaDr(listaDoctor* lista);
void liberarListaDr(listaDoctor* lista);
struct nodoDoctor* crearNodoDr(doctor d);
doctor* buscarDr_ID(listaDoctor* lista, int id);
doctor* buscarDr_Cedula(listaDoctor* lista, const char* cedula);


#endif