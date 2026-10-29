#ifndef PACIENTE_H
#define PACIENTE_H
typedef struct paciente
{
    int id;
    int edad;
    char nombre[25];
    char apellidoPaterno[20];
    char apellidoMaterno[20];



}paciente;

int registrarPaciente(paciente nuevo);
#endif

//#ifndef verifica si ya existe una definicion con ese nombre y #define es para decirle al programa que si ya procesó el archivo una vez en algún momento, no lo vuelva a cargar

/* Los archivos .h le dicen a nuestro programa:
Existe un tipo de dato llamado Paciente.
 Existe una función llamada registrarPaciente (o cualquier otra)
Pero no explica qué hacen las funciones por dentro. Es como las clases abstractas en Java wey*/