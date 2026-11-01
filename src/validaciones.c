#include "validaciones.h"
#include<stdio.h>

int validaStrings(const char* cadenaTxt){
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