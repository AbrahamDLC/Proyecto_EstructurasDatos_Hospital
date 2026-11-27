#include <stdlib.h>
#include <string.h>
#include "arbol.h"

static int comparar(const char* clave, int id, const nodoArbol* n){
    int c=strcmp(clave,n->clave);
    if(c!=0){return c;}
    return (id>n->id)-(id<n->id);
}

int iniciarArbol(arbolIndice* a){
    if(a==NULL){return ARBOL_CLAVE_INVALIDA;}
    a->raiz=NULL;
    a->cantidad=0;
    return ARBOL_OK;
}

int insertarArbol(arbolIndice* a, const char* clave, int id){
    if(a==NULL||clave==NULL||clave[0]=='\0'||strlen(clave)>=MAX_CLAVE||id<=0){
        return ARBOL_CLAVE_INVALIDA;
    }
    nodoArbol** p=&a->raiz;               /* p apunta al "hueco" donde irá el nodo */
    while(*p!=NULL){
        int c=comparar(clave,id,*p);
        if(c==0){return ARBOL_DUPLICADO;}
        p=(c<0)?&(*p)->izquierda:&(*p)->derecha;
    }
    nodoArbol* nuevo=malloc(sizeof(nodoArbol));
    if(nuevo==NULL){return ARBOL_ERROR_MEMORIA;}
    strcpy(nuevo->clave,clave);
    nuevo->id=id;
    nuevo->izquierda=NULL;
    nuevo->derecha=NULL;
    *p=nuevo;
    a->cantidad++;
    return ARBOL_OK;
}

nodoArbol* buscarArbol(const arbolIndice* a, const char* clave){
    if(a==NULL||clave==NULL){return NULL;}
    nodoArbol* n=a->raiz;
    nodoArbol* hallado=NULL;
    while(n!=NULL){
        int c=strcmp(clave,n->clave);
        if(c==0){hallado=n;n=n->izquierda;}   /* sigue a la izquierda: devuelve el de menor id */
        else if(c<0){n=n->izquierda;}
        else{n=n->derecha;}
    }
    return hallado;
}

static void recolectar(const nodoArbol* n, const char* clave, int ids[], int capacidad, int* cuantos){
    if(n==NULL){return;}
    int c=strcmp(clave,n->clave);
    if(c<=0){recolectar(n->izquierda,clave,ids,capacidad,cuantos);}
    if(c==0&&*cuantos<capacidad){ids[(*cuantos)++]=n->id;}
    if(c>=0){recolectar(n->derecha,clave,ids,capacidad,cuantos);}
}

int buscarTodosArbol(const arbolIndice* a, const char* clave, int ids[], int capacidad, int* n){
    if(a==NULL||clave==NULL||ids==NULL||n==NULL){return ARBOL_CLAVE_INVALIDA;}
    *n=0;
    recolectar(a->raiz,clave,ids,capacidad,n);
    return (*n>0)?ARBOL_OK:ARBOL_NO_ENCONTRADO;
}

static nodoArbol* eliminarRec(nodoArbol* n, const char* clave, int id, int* ok){
    if(n==NULL){return NULL;}
    int c=comparar(clave,id,n);
    if(c<0){n->izquierda=eliminarRec(n->izquierda,clave,id,ok);}
    else if(c>0){n->derecha=eliminarRec(n->derecha,clave,id,ok);}
    else{
        *ok=1;
        if(n->izquierda==NULL){nodoArbol* r=n->derecha;free(n);return r;}   /* 0 o 1 hijo */
        if(n->derecha==NULL){nodoArbol* l=n->izquierda;free(n);return l;}
        nodoArbol* suc=n->derecha;                                           /* 2 hijos: sucesor inorden */
        while(suc->izquierda!=NULL){suc=suc->izquierda;}
        strcpy(n->clave,suc->clave);
        n->id=suc->id;
        int ok2=0;
        n->derecha=eliminarRec(n->derecha,n->clave,n->id,&ok2);
    }
    return n;
}

int eliminarArbol(arbolIndice* a, const char* clave, int id){
    if(a==NULL||clave==NULL||id<=0){return ARBOL_CLAVE_INVALIDA;}
    int ok=0;
    a->raiz=eliminarRec(a->raiz,clave,id,&ok);
    if(!ok){return ARBOL_NO_ENCONTRADO;}
    a->cantidad--;
    return ARBOL_OK;
}

static int alturaRec(const nodoArbol* n){
    if(n==NULL){return 0;}
    int i=alturaRec(n->izquierda), d=alturaRec(n->derecha);
    return 1+(i>d?i:d);
}
int alturaArbol(const arbolIndice* a){return a==NULL?0:alturaRec(a->raiz);}

static void pre(const nodoArbol* n, const nodoArbol* arr[], int cap, int* k){
    if(n==NULL||*k>=cap){return;}
    arr[(*k)++]=n; pre(n->izquierda,arr,cap,k); pre(n->derecha,arr,cap,k);
}
static void in(const nodoArbol* n, const nodoArbol* arr[], int cap, int* k){
    if(n==NULL){return;}
    in(n->izquierda,arr,cap,k);
    if(*k<cap){arr[(*k)++]=n;}
    in(n->derecha,arr,cap,k);
}
static void post(const nodoArbol* n, const nodoArbol* arr[], int cap, int* k){
    if(n==NULL){return;}
    post(n->izquierda,arr,cap,k); post(n->derecha,arr,cap,k);
    if(*k<cap){arr[(*k)++]=n;}
}
void preordenArbol(const arbolIndice* a, const nodoArbol* arr[], int cap, int* n){*n=0; if(a!=NULL){pre(a->raiz,arr,cap,n);}}
void inordenArbol(const arbolIndice* a, const nodoArbol* arr[], int cap, int* n){*n=0; if(a!=NULL){in(a->raiz,arr,cap,n);}}
void postordenArbol(const arbolIndice* a, const nodoArbol* arr[], int cap, int* n){*n=0; if(a!=NULL){post(a->raiz,arr,cap,n);}}

static void liberarRec(nodoArbol* n){
    if(n==NULL){return;}
    liberarRec(n->izquierda);
    liberarRec(n->derecha);
    free(n);
}
void liberarArbol(arbolIndice* a){
    if(a==NULL){return;}
    liberarRec(a->raiz);
    a->raiz=NULL;
    a->cantidad=0;
}
