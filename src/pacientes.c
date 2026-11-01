#include <stdio.h>
#include <string.h>
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

    return PACIENTE_VALIDO;

    
}