#include "login.h"
#include "validaciones.h"
#include "pacientes.h"

int ingresarClave(const char* claveUs, const char* claveG, int* intentos){
    if(*intentos >=MAX_INTENTOS){return ERROR_BLOQUEADO;}
    
    if(validaLogin(claveUs)!=1){return ERROR_FORMATO_INVALIDO;}

    if(strcmp(claveUs,claveG)!=0){
        *intentos+=1;
        return ERROR_CREDENCIALES;
    }
    
    *intentos=0;
    return LOGIN_EXITOSO;
}

int ingresarUsuario(const char* usuarioIngr, const char* usuarioG){
    if (validaUsuario(usuarioIngr)!=1){return ERROR_FORMATO_INVALIDO;}

    if(strcmp(usuarioIngr,usuarioG)!=0)
    {
        return USUARIO_INVALIDO;
    }
    
    return LOGIN_EXITOSO;
}