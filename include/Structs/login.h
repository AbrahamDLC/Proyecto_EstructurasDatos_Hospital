#ifndef LOGIN
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#define LEN 10
#define LEN_USER 15
#define MAX_INTENTOS 5

typedef struct Login{
    char usuario[LEN_USER];
    char clave[LEN];

}Login;

typedef enum{
    LOGIN_EXITOSO=1,
    ERROR_FORMATO_INVALIDO,
    ERROR_CREDENCIALES,
    ERROR_BLOQUEADO
}ErrorLogin;
int ingresarClave(const char* claveU, const char* claveGuardada, int* intentos);
int ingresarUsuario(const char* user, const char*);





#endif

