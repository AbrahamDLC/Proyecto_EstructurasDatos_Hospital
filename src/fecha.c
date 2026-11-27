#include "fecha.h"

int validarFecha(int dia, int mes, int anio){
    int bisiesto;
    int dias;
    if ((anio % 400 == 0) || (anio % 4 == 0 && anio % 100 != 0))
    {
        bisiesto=1;
    }else{
        bisiesto=0;
    }
    
    if (dia<1||dia>31){return 0;}
    if (mes<1||mes>12){return 0;}
    if (anio>fechaActual().anio||anio<1900){return 0;}
    switch (mes)
    {
    case 2:
        if (bisiesto)
        {
            dias=29;
        }else{
            dias=28;
        }
        
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        dias=30;
        break;
    default:
        dias=31;
        break;
    }
    if (dia>dias){return 0;}
    
    
    return 1;
}

fecha fechaActual(){
    time_t fechaActual= time(NULL);
    struct tm *t=localtime(&fechaActual);
    fecha hoy;
    hoy.dia=t->tm_mday;
    hoy.mes=t->tm_mon+1;
    hoy.anio=t->tm_year+1900;
 
    return hoy;    
}

int calcularEdadPaciente(fecha fechaNacimiento, fecha fechaHoy){
    if (!validarFecha(fechaNacimiento.dia, fechaNacimiento.mes, fechaNacimiento.anio))
    {
        return -1;
    }

    if (!validarFecha(fechaHoy.dia,fechaHoy.mes, fechaHoy.anio))
    {
        return -1;
    }

    int edad = fechaHoy.anio-fechaNacimiento.anio;
    if (fechaHoy.mes < fechaNacimiento.mes ||
        (fechaHoy.mes == fechaNacimiento.mes && fechaHoy.dia < fechaNacimiento.dia))
     {
         edad--;
     }

     if (edad<0)
     {
        return -1;
     }
     
    
    return edad;

    int minutosActuales(void){
        time_t t = time(NULL);
        struct tm* m = localtime(&t);
        return m->tm_hour*60 + m->tm_min;
    }
    
    int compararFechas(fecha a, fecha b){
        if(a.anio != b.anio) return a.anio - b.anio;
        if(a.mes  != b.mes)  return a.mes  - b.mes;
        return a.dia - b.dia;
    }
    
    int validarFechaCita(int dia, int mes, int anio){
        // cita debe ser HOY o FUTURO
        fecha hoy = fechaActual();
        if(!validarFecha(dia,mes,anio)) return 0;
        fecha f = {dia,mes,anio};
        return compararFechas(f, hoy) >= 0;
    }
    
}

