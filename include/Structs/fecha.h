#ifndef FECHA_H
#define FECHA_H
#include <time.h>

typedef struct fecha
{
    int dia;
    int mes;
    int anio;
}fecha;

fecha fechaActual(void);
int validarFecha(int dia, int mes, int anio);
int calcularEdadPaciente(fecha fechaNacimiento, fecha fechaActual);

#endif