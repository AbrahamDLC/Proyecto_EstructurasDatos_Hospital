#include <stdlib.h>
#include "cita.h"

int iniciarListaCitas(listaCitas* lista){
    lista->cabeza=NULL;
    lista->cola=NULL;
    lista->cantidad=0;
    lista->sigId=1;
    return CITA_VALIDA;
}

void liberarListaCitas(listaCitas* lista){
    nodoCita* actual=lista->cabeza;
    while(actual!=NULL){
        nodoCita* sig=actual->siguienteNodo;
        free(actual);
        actual=sig;
    }
    lista->cabeza=NULL;
    lista->cola=NULL;
    lista->cantidad=0;
}

static bool mismoHorario(const cita* a, const cita* b){
    return compararFechas(a->fechaCita,b->fechaCita)==0 && a->hora==b->hora && a->minuto==b->minuto;
}

int registrarCita(listaCitas* lista, listaPacientes* pacientes, listaDoctor* doctores, cita nueva, int* idCita){
    if(lista==NULL){return ERROR_CITA_NO_ENCONTRADA;}

    if(!validarFechaCita(nueva.fechaCita.dia,nueva.fechaCita.mes,nueva.fechaCita.anio)){
        return ERROR_CITA_FECHA_INVALIDA;
    }
    if(nueva.hora<0||nueva.hora>23||nueva.minuto<0||nueva.minuto>59){
        return ERROR_CITA_HORA_INVALIDA;
    }
    // si es hoy, la hora no puede haber pasado ya 
    if(compararFechas(nueva.fechaCita,fechaActual())==0 && nueva.hora*60+nueva.minuto<minutosActuales()){
        return ERROR_CITA_FECHA_INVALIDA;
    }

    paciente* pac=buscarID(pacientes,nueva.idPaciente);
    if(pac==NULL||!pac->activo){return ERROR_CITA_PACIENTE_NO_ENCONTRADO;}
    doctor* dr=buscarDr_ID(doctores,nueva.idDoctor);
    if(dr==NULL||!dr->activo){return ERROR_CITA_DOCTOR_NO_ENCONTRADO;}

    for(nodoCita* n=lista->cabeza;n!=NULL;n=n->siguienteNodo){
        if(n->dato.estado!=CITA_PROGRAMADA||!mismoHorario(&n->dato,&nueva)){continue;}
        if(n->dato.idDoctor==nueva.idDoctor){return ERROR_CITA_CONFLICTO_DOCTOR;}
        if(n->dato.idPaciente==nueva.idPaciente){return ERROR_CITA_CONFLICTO_PACIENTE;}
    }

    nueva.id=lista->sigId;
    nueva.estado=CITA_PROGRAMADA;
    nodoCita* nuevo=malloc(sizeof(nodoCita));
    if(nuevo==NULL){return ERROR_MEMORIA;}
    nuevo->dato=nueva;
    nuevo->siguienteNodo=NULL;

    if(lista->cabeza==NULL){
        lista->cabeza=nuevo;
    }else{
        lista->cola->siguienteNodo=nuevo;
    }
    lista->cola=nuevo;
    lista->cantidad++;
    lista->sigId++;
    dr->pacientesAsignados++;
    if(idCita!=NULL){*idCita=nueva.id;}
    return CITA_VALIDA;
}

cita* buscarCitaID(listaCitas* lista, int idCita){
    if(lista==NULL){return NULL;}
    for(nodoCita* n=lista->cabeza;n!=NULL;n=n->siguienteNodo){
        if(n->dato.id==idCita){return &n->dato;}
    }
    return NULL;
}

static int cerrarCita(listaCitas* lista, listaDoctor* doctores, int idCita, estadoCita nuevoEstado){
    cita* c=buscarCitaID(lista,idCita);
    if(c==NULL){return ERROR_CITA_NO_ENCONTRADA;}
    if(c->estado!=CITA_PROGRAMADA){return ERROR_CITA_ESTADO_INVALIDO;}
    c->estado=nuevoEstado;
    doctor* dr=buscarDr_ID(doctores,c->idDoctor);
    if(dr!=NULL&&dr->pacientesAsignados>0){dr->pacientesAsignados--;}
    return CITA_VALIDA;
}

int cancelarCita(listaCitas* lista, listaDoctor* doctores, int idCita){
    return cerrarCita(lista,doctores,idCita,CITA_CANCELADA);
}

int atenderCita(listaCitas* lista, listaDoctor* doctores, int idCita){
    return cerrarCita(lista,doctores,idCita,CITA_ATENDIDA);
}

int citasDePaciente(const listaCitas* lista, int idPaciente, int ids[], int capacidad, int* n){
    if(lista==NULL||ids==NULL||n==NULL){return ERROR_ID_INVALIDO;}
    *n=0;
    for(nodoCita* c=lista->cabeza;c!=NULL&&*n<capacidad;c=c->siguienteNodo){
        if(c->dato.idPaciente==idPaciente){ids[(*n)++]=c->dato.id;}
    }
    return CITA_VALIDA;
}

int citasDeDoctor(const listaCitas* lista, int idDoctor, int ids[], int capacidad, int* n){
    if(lista==NULL||ids==NULL||n==NULL){return ERROR_ID_INVALIDO;}
    *n=0;
    for(nodoCita* c=lista->cabeza;c!=NULL&&*n<capacidad;c=c->siguienteNodo){
        if(c->dato.idDoctor==idDoctor){ids[(*n)++]=c->dato.id;}
    }
    return CITA_VALIDA;
}
