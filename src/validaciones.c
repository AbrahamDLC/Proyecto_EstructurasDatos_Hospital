#include "validaciones.h"
#include<stdio.h>
#include <ctype.h>

int validaStrings(const char* cadenaTxt){
    if(cadenaTxt==NULL){return 0;}
    regex_t regexp;
    const char* pattern = "^[a-zA-ZáéíóúÁÉÍÓÚÑñ ]+$";
    int valido; //se guarda 1 o 0 dependiendo del match
    valido = regcomp(&regexp, pattern,REG_EXTENDED);//compila la expresion y la guarda como 0 si todo está bien Y REG_EXTENDED permite usar los operadores de las expresiones regulares como + o {}

    if (valido!=0)
    {
        return -1; //el -1 le dice al programa que hubo un error en la compil.
    }
    valido= regexec(&regexp, cadenaTxt,0,NULL,0);
    regfree(&regexp);
    
    if (valido==0){return 1;}
    if (valido==REG_NOMATCH){return 0;}
    return -1;
    
}

//==============================================================================

int validaExpediente(const char* exped){
    if (exped==NULL){return 0;}
    
    regex_t regexp;
    const char* pattern = "^PS-([0-9]{3})-([0-9]{3})$";
    int valido;
    valido = regcomp(&regexp,pattern, REG_EXTENDED);

    if(valido!=0){return -1;}
    valido=regexec(&regexp, exped,0,NULL,0);
    regfree(&regexp);

    if(valido==0){return 1;}
    if(valido==REG_NOMATCH){return 0;}
    return -1;

}

//===========================================================
//VALIDA LOGIN

int validaClave(const char* contrasenia){
    if(contrasenia==NULL){return 0;}
    regex_t regexp;
    const char* pattern= "^[a-zA-Z0-9]{8,10}$";
    int valido;
    valido = regcomp(&regexp, pattern, REG_EXTENDED);
    if(valido!=0){return -1;}
    valido=regexec(&regexp, contrasenia,0,NULL,0);
    regfree(&regexp);
    if(valido==0){return 1;}
    if(valido==REG_NOMATCH){return 0;}
    return -1;

}

int validaUsuario(const char* user){
    if(user==NULL){return 0;}
    regex_t regexp;

    const char* pattern="^[a-zA-Z0-9.?]{8}$";
    int valido;
    valido = regcomp(&regexp, pattern, REG_EXTENDED);
    if(valido!=0){return -1;}
    valido = regexec(&regexp, user, 0,NULL,0);
    regfree(&regexp);
    if(valido==0){return 1;}
    if(valido==REG_NOMATCH){return 0;}
    return -1;
}
//===============================================================
//DOCTOR
int validaCedula(const char* cedula){
    if(cedula==NULL){return 0;}
    regex_t regexp;

    const char* pattern = "^[0-9]{8,9}$";
    int validarPatron = regcomp(&regexp,pattern, REG_EXTENDED);
    if(validarPatron!=0){return -1;}
    validarPatron = regexec(&regexp, cedula, 0,NULL,0);
    regfree(&regexp);
    if(validarPatron==0){return 1;}
    if(validarPatron==REG_NOMATCH){ return 0;}
    return -1;
}

bool validaID(int *id) {
    char buffer[12];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return false;
    buffer[strcspn(buffer, "\n")] = '\0';

    if (buffer[0] == '\0') return false;
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (!isdigit((unsigned char)buffer[i])) return false;
    }

    *id = atoi(buffer);
    return *id > 0;
}

