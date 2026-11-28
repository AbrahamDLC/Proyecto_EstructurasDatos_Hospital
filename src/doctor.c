#include "doctor.h"

int validarDr(doctor p){
    if(validaStrings(p.nombre)!=1){
        return ERROR_NOMBREoAPELLIDO_INVALIDO;
    }
    if(p.idDoctor<=0){return ERROR_ID_INVALIDO;}
    if(validaStrings(p.apellidoP)!=1){return ERROR_NOMBREoAPELLIDO_INVALIDO;}
    if(validaStrings(p.appellidoM)!=1){return ERROR_NOMBREoAPELLIDO_INVALIDO;}
    if(validaCedula(p.cedula)!=1){return ERROR_CEDULA_INVALIDA;}
    return DOCTOR_VALIDO;
    
}





struct nodoDoctor* crearNodoDr(doctor d){
    nodoDoctor* nuevo=malloc(sizeof(nodoDoctor));
    if(nuevo==NULL){return NULL;}
    nuevo->dato=d;
    nuevo->siguienteNodo=NULL;
    return nuevo;
}

int iniciarListaDr(listaDoctor* lista){
    lista->cabeza=NULL;
    lista->cola=NULL;
    lista->cantidad=0;
    lista->sigId=1;
    return DOCTOR_VALIDO;
}

void liberarListaDr(listaDoctor* lista){
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
        if(strcmp(n->dato.cedula,cedula)==0){return &n->dato;}
    }
    return NULL;
}

int registrarDr(listaDoctor* lista, doctor d, int* drID){
    if(lista==NULL){return ERROR_DR_NOENCONTRADO;}
    d.idDoctor=lista->sigId;
    int r=validarDr(d);
    if(r!=DOCTOR_VALIDO){return r;}

    for(nodoDoctor* n=lista->cabeza;n!=NULL;n=n->siguienteNodo){
        if(strcmp(n->dato.cedula,d.cedula)==0){return ERROR_CEDULA_DUPLICADA;}
    }

    d.activo=true;
    d.pacientesAsignados=0;
    nodoDoctor* nuevo=crearNodoDr(d);
    if(nuevo==NULL){return ERROR_MEMORIA;}

    if(lista->cabeza==NULL){
        lista->cabeza=nuevo;
    }else{
        lista->cola->siguienteNodo=nuevo;
    }
    lista->cola=nuevo;
    lista->cantidad++;
    lista->sigId++;
    if(drID!=NULL){*drID=d.idDoctor;}
    return DOCTOR_VALIDO;
}
