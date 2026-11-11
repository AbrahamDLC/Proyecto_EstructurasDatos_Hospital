#ifndef ESPECIALIDADES_H
#define ESPECIALIDADES_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef enum{
    ESPECIALIDAD_INVALIDA,
    ESPECIALIDAD_VALIDA=1,
    ESPECIALIDAD_DUPLICADA,
    ESPECIALIDAD_NO_ENCONTRADA,
    ERROR_MEMORIA
}errorEspecialidad;



typedef struct especialidad{
    int id;
    char especialidadN[20];
}especialidad;

typedef struct nodoEspecialidad{
    especialidad dato;
    struct nodoEspecialidad* siguienteNodo;
}nodoEspecialidad;

typedef struct{
    nodoEspecialidad* cabeza;
    nodoEspecialidad* cola;
    int cantidad;
    int sigId;

}listaEspecialidades;

//FUNCIONES
int registrarEspecialidad(listaEspecialidades* lista, especialidad e);
int iniciarEspecialidad(listaEspecialidades* lista);
void liberarEspecialidad(listaEspecialidades* lista);
especialidad* buscarIDEspecialidad(listaEspecialidades* lista, int idE);
especialidad* buscarNombreEspecialidad(listaEspecialidades* lista, const char* nombre);

#endif