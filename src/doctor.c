#include "doctor.h"

int validarDr(doctor p){
    if(validaStrings(p.nombre)!=1){
        return ERROR_NOMBREoAPELLIDO_INVALIDO;
    }
    if(validaID(p.idDoctor)!=true){return ERROR_ID_INVALIDO;}
    if(validaStrings(p.apellidoP)!=1){return ERROR_NOMBREoAPELLIDO_INVALIDO;}
    if(validaStrings(p.appellidoM)!=1){return ERROR_NOMBREoAPELLIDO_INVALIDO;}
    if(validaCedula(p.cedula)!=1){return ERROR_CEDULA_INVALIDA;}
    if(buscarIDEspecialidad(p.idEspecialidad)!=1){return 3;}
    
}





struct nodoDoctor* crearNodo(doctor d){
    nodoDoctor* nuevo=malloc(sizeof(nodoDoctor));
    if(nuevo==NULL){return NULL;}
    nuevo->dato=d;
    nuevo->siguienteNodo=NULL;
    return nuevo;
}

int iniciarLista(listaDoctor* lista){
    lista->cabeza=NULL;
    lista->cola=NULL;
    lista->cantidad=0;
    lista->sigId=1;
    return DOCTOR_VALIDO;
}

void liberarLista(listaDoctor* lista){
    nodoDoctor* actual = lista->cabeza;
    while (actual!=NULL)
    {
        nodoDoctor* sig = actual->siguienteNodo;
        free(actual);
        actual=sig;
    }

    lista->cabeza=NULL;
    lista->cola=NULL;
    lista->cantidad=0;
}

doctor* buscarDr_ID(listaDoctor* lista, int drID){
    if(lista==NULL){return NULL;}
    for(nodoDoctor* n=lista->cabeza ; n!=NULL; n=n->siguienteNodo){
        if(n->dato.idDoctor==drID){return &n->dato;}
    }
    return NULL;
}

doctor* buscarDr_Cedula(listaDoctor* lista, const char* cedula){
    if(lista==NULL|| validaCedula(cedula)!=1){return NULL;}
    for(nodoDoctor* n=lista->cabeza;n!=NULL; n=n->siguienteNodo){
        if(n->dato.cedula==cedula){return &n->dato;}
    }
    return NULL;
}