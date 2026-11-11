#ifndef VALIDACION_P
#define VALIDACION_P
#include <regex.h>

int validaStrings(const char* cadenaTxt);
int validaExpediente(const char* exped);
int validaLogin(const char* contrasenia);
int validaClave(const char* contrasenia);
int validaUsuario(const char* user);
int validaCedula(const char* cedula);
int validaID(int id);
#endif