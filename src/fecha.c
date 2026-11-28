#include "fecha.h"

static int esFechaCalendarioValida(int dia, int mes, int anio){
    if(anio < 1900) return 0;
    if(mes < 1 || mes > 12) return 0;
    if(dia < 1) return 0;
    int bisiesto = ((anio % 400 == 0) || (anio % 4 == 0 && anio % 100 != 0));
    int dias;
    switch(mes){
        case 2:  dias = bisiesto ? 29 : 28; break;
        case 4:  case 6:  case 9:  case 11: dias = 30; break;
        default: dias = 31;
    }
    return dia <= dias;
}

static fecha hacerFecha(int d, int m, int a){
    fecha f;
    f.dia  = d;
    f.mes  = m;
    f.anio = a;
    return f;
}

fecha fechaActual(void){
    time_t t = time(NULL);
    struct tm *lt = localtime(&t);
    fecha hoy;
    hoy.dia  = lt->tm_mday;
    hoy.mes  = lt->tm_mon + 1;
    hoy.anio = lt->tm_year + 1900;
    return hoy;
}

int minutosActuales(void){
    time_t t = time(NULL);
    struct tm *lt = localtime(&t);
    return lt->tm_hour * 60 + lt->tm_min;
}

int compararFechas(fecha a, fecha b){
    if(a.anio != b.anio) return a.anio - b.anio;
    if(a.mes  != b.mes)  return a.mes  - b.mes;
    return a.dia - b.dia;
}

/* Nacimiento: fecha pasada u hoy */
int validarFecha(int dia, int mes, int anio){
    if(!esFechaCalendarioValida(dia, mes, anio)) return 0;
    return compararFechas(hacerFecha(dia, mes, anio), fechaActual()) <= 0;
}

int validarFechaCita(int dia, int mes, int anio){
    if(!esFechaCalendarioValida(dia, mes, anio)) return 0;
    return compararFechas(hacerFecha(dia, mes, anio), fechaActual()) >= 0;
}

int calcularEdadPaciente(fecha fechaNacimiento, fecha fechaHoy){
    if(!esFechaCalendarioValida(fechaNacimiento.dia,
                                 fechaNacimiento.mes,
                                 fechaNacimiento.anio)) return -1;
    if(!esFechaCalendarioValida(fechaHoy.dia,
                                 fechaHoy.mes,
                                 fechaHoy.anio)) return -1;

    int edad = fechaHoy.anio - fechaNacimiento.anio;
    if(fechaHoy.mes < fechaNacimiento.mes ||
       (fechaHoy.mes == fechaNacimiento.mes && fechaHoy.dia < fechaNacimiento.dia)){
        edad--;
    }
    return (edad < 0) ? -1 : edad;
}