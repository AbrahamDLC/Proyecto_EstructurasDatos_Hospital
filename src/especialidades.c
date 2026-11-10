#include "especialidades.h"
#include "validaciones.h"

int iniciarEspecialidad(listaEspecialidades* lista){
    lista->cabeza =NULL;
    lista->cola=NULL;
    lista->cantidad=0;
    lista->sigId=1;
    return ESPECIALIDAD_VALIDA;
}
void liberarEspecialidad(listaEspecialidades* lista){
    nodoEspecialidad* actual = lista->cabeza;
    while (actual != NULL) {
        nodoEspecialidad* sig = actual->siguienteNodo;
        free(actual);
        actual = sig;
    }
    lista->cabeza = NULL;
    lista->cola = NULL;
    lista->cantidad = 0;
}

struct nodoEspecialidad* crearNodo(especialidad e){
    nodoEspecialidad* nuevo = malloc(sizeof(nodoEspecialidad));
    if(nuevo==NULL){return NULL;}
    nuevo->dato=e;
    nuevo->siguienteNodo=NULL;
    return nuevo;
}

int registrarEspecialidad(listaEspecialidades* lista,especialidad e){
    e.id=lista->sigId;
    int especialidadValida = validaStrings(e.especialidadN);
    if(especialidadValida!=1){return ESPECIALIDAD_INVALIDA;}
//ESPECIALIDAD DUPLICADA
    for(nodoEspecialidad* n=lista->cabeza;n!=NULL;n=n->siguienteNodo){
        if(strcmp(n->dato.especialidadN, e.especialidadN)==0){
            return ESPECIALIDAD_DUPLICADA;
        }
    }

//GUARDAMOS LA ESPECIALIDAD

    nodoEspecialidad* nuevo = crearNodo(e);
    if(nuevo==NULL){return ERROR_MEMORIA;}
    if(lista->cabeza==NULL){
        lista->cabeza=nuevo;
    }else{
        lista->cola->siguienteNodo=nuevo;
        lista->cola=nuevo;
    }
    lista->cantidad++;
    lista->sigId++;
    return ESPECIALIDAD_VALIDA;

}

//FUNCIONES DE BÚSQUEDA
especialidad* buscarIDEspecialidad(listaEspecialidades* lista, int id){
    if(lista==NULL){return NULL;}
    for(nodoEspecialidad* n = lista->cabeza; n!=NULL; n=n->siguienteNodo){
        if(n->dato.id==id){return &n->dato.especialidadN;}
    }
    return NULL;
}

especialidad* buscarNombreEspecialidad(listaEspecialidades* lista, const char* nombre){
    if(lista||validaStrings(nombre)!=1){return NULL;}
    for(nodoEspecialidad* n=lista->cabeza;n!=NULL;n=n->siguienteNodo){
        if(strcmp(n->dato.especialidadN,nombre)==0){return &n->dato.especialidadN;}
    }
    return NULL;
}

