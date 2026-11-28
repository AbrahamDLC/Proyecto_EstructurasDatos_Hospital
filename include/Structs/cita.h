#ifndef CITA_H
#define CITA_H
#include <stdbool.h>
#include "erroresGrales.h"
#include "fecha.h"
#include "pacientes.h"
#include "doctor.h"

typedef enum{
    CITA_PROGRAMADA=1,
    CITA_ATENDIDA,
    CITA_CANCELADA
}estadoCita;

typedef enum{
    CITA_VALIDA=1,
    ERROR_CITA_FECHA_INVALIDA=600,
    ERROR_CITA_HORA_INVALIDA,
    ERROR_CITA_PACIENTE_NO_ENCONTRADO,   
    ERROR_CITA_DOCTOR_NO_ENCONTRADO,    
    ERROR_CITA_CONFLICTO_DOCTOR,         
    ERROR_CITA_CONFLICTO_PACIENTE,       
    ERROR_CITA_NO_ENCONTRADA,
    ERROR_CITA_ESTADO_INVALIDO           /* solo se puede cancelar/atender una cita PROGRAMADA */
}errorCita;

typedef struct cita{
    int id;
    int idPaciente;
    int idDoctor;
    fecha fechaCita;
    int hora;      
    int minuto;    
    estadoCita estado;
}cita;

typedef struct nodoCita{
    cita dato;
    struct nodoCita* siguienteNodo;
}nodoCita;

typedef struct{
    nodoCita* cabeza;
    nodoCita* cola;
    int cantidad;
    int sigId;
}listaCitas;

int iniciarListaCitas(listaCitas* lista);
void liberarListaCitas(listaCitas* lista);
int registrarCita(listaCitas* lista, listaPacientes* pacientes, listaDoctor* doctores, cita nueva, int* idCita);
int cancelarCita(listaCitas* lista, listaDoctor* doctores, int idCita);
int atenderCita(listaCitas* lista, listaDoctor* doctores, int idCita);
cita* buscarCitaID(listaCitas* lista, int idCita);
int citasDePaciente(const listaCitas* lista, int idPaciente, int ids[], int capacidad, int* n);
int citasDeDoctor(const listaCitas* lista, int idDoctor, int ids[], int capacidad, int* n);

#endif
