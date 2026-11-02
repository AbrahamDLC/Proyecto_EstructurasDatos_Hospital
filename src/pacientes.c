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
        return ERROR_NOMBRE_VACIO;
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
//LISTA Y REGISTRO

int iniciarLista(listaPacientes* lista){
    lista->cantidad=0;
    lista->capacidad=100;
    lista->sigId=1;
    lista->datos= malloc(lista->capacidad * sizeof(paciente));
    return lista->datos!=NULL;
}

void liberarLista(listaPacientes* lista){
    free(lista->datos);
    lista->datos=NULL;
    lista->cantidad=0;
    lista->capacidad=0;
}
int registrarPaciente(listaPacientes* lista, paciente p, int* id){
    p.id=lista->sigId;
    int pacienteV = validarPaciente(p);
    if (pacienteV!=PACIENTE_VALIDO)
    {
        return pacienteV;
    }
//PACIENTES DUPLICADOS
    for (int i = 0; i < lista->cantidad; i++)
    {
        if (strcmp(lista->datos[i].numeroExpediente,p.numeroExpediente)!=1)
        {
            return ERROR_PACIENTE_DUPLICADO;
        }
    }
//TAMAÑO INSUFICIENTE

    if (lista->cantidad==lista->capacidad)
    {
        int aumentarCapacidad = lista->capacidad*2;
        paciente *tmp = realloc(lista->datos, aumentarCapacidad * sizeof(paciente));
        if (tmp==NULL){return ERROR_MEMORIA;}
        lista->datos=tmp;
        lista->capacidad=aumentarCapacidad;
        
    }

//GUARDAMOS

    p.activo=true;
    lista->datos[lista->cantidad]=p;
    lista->cantidad++;
    if (id != NULL) { *id = p.id; }
    lista->sigId++;
    
    return PACIENTE_VALIDO;
}

//==============================================================
//BUSCAR ID
paciente* buscarID(listaPacientes* lista, int idPac){
    for (int i = 0; i < lista->cantidad; i++)
    {
        if (lista->datos[i].id == idPac)
        {
            return &lista->datos[i];
        }
    }
    return NULL;
}
//==================================================================
//BUSCAR EXPEDIENTE

paciente* buscarExpediente(listaPacientes* lista, const char* exp){

    if(validaExpediente(exp)!=1||lista==NULL){return NULL;}
    for (int i = 0; i < lista->cantidad; i++)
    {
        if (stricmp(lista->datos[i].numeroExpediente, exp)==0)
        {
            return &lista->datos[i];
        }
        
    }
    return NULL;
}
