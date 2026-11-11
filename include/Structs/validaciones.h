#ifndef VALIDACION_P
#define VALIDACION_P
#include <regex.h>
#include <stdbool.h>
#include "especialidades.h"

int validaStrings(const char* cadenaTxt);
int validaExpediente(const char* exped);
int validaLogin(const char* contrasenia);
int validaClave(const char* contrasenia);
int validaUsuario(const char* user);
int validaCedula(const char* cedula);
int validaEspecialidad(listaEspecialidades* lista ,const char* especialidad);
bool validaID(int *id);
#endif