#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include <stdbool.h>

typedef enum{
    DOCTOR_VALIDO=1,
    ERROR_DR_NOENCONTRADO,
    ERROR_ID_DR_INVALIDO,

}errorDoctor;



typedef struct doctor{
    char nombre[15];
    char apellidoP[15];
    char appellidoM[15];
    char cedula[9];
    bool activo;

}doctor;

