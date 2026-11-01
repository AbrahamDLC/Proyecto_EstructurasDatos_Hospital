#ifndef PACIENTE_H
#define PACIENTE_H
#include <stdbool.h>
#include "fecha.h"
typedef struct paciente
{
    int id;
    char nombre[25];
    char apellidoPaterno[20];
    char apellidoMaterno[20];
    char numeroExpediente[15];
    bool activo;
    fecha fechaNaciemiento;


}paciente;
//ESTO ES PORQUE LO PASAREMOS A FRONT WEY, MOSTRARÁ EL TIPO DE ERROR EXACTO
typedef enum{
    PACIENTE_VALIDO,
    ERROR_ID_INVALIDO,
    ERROR_NOMBRE_VACIO,
    ERROR_APELLIDO_VACIO,
    ERROR_EXPEDIENTE_INVALIDO,
    ERROR_FECHA_INVALIDA,
    ERROR_PACIENTE_DUPLICADO
} ErrorPaciente;


int registrarPaciente(paciente nuevo);
int validarPaciente(paciente pacienteN);
paciente* buscarID(int IDpaciente);
paciente* buscarExpediente(const char* expediente);
int bajaDePaciente(const char* IDpaciente);
void mostrarPaciente();
void listaPacientes();
#endif

//#ifndef verifica si ya existe una definicion con ese nombre y #define es para decirle al programa que si ya procesó el archivo una vez en algún momento, no lo vuelva a cargar

/* Los archivos .h le dicen a nuestro programa:
Existe un tipo de dato llamado Paciente.
 Existe una función llamada registrarPaciente (o cualquier otra)
Pero no explica qué hacen las funciones por dentro. Es como las clases abstractas en Java wey*/