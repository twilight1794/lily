#include "lde.h"

struct LDE_LDE* LDE_Create(void){
    struct LDE_LDE* lde = (struct LDE_LDE*) calloc(1, sizeof(struct LDE_LDE));
    if (lde == NULL) return NULL;
    lde->inicio = NULL;
    lde->final = NULL;
    lde->tamano = 0;
    return lde;
}

struct LDE_Nodo* LDE_Nodo_Create(void){
    struct LDE_Nodo* nodo = (struct LDE_Nodo*) calloc(1, sizeof(struct LDE_Nodo));
    if (nodo == NULL) return NULL;
    nodo->anterior = NULL;
    nodo->posterior = NULL;
    nodo->valor = NULL;
    return nodo;
}

struct LDE_Nodo* LDE_Insert(struct LDE_LDE* lde, size_t pos, void* valor){
    if (pos > lde->tamano) return NULL;
    struct LDE_Nodo* nodo = LDE_Nodo_Create();
    if (nodo == NULL) return NULL;
    nodo->valor = valor;
    struct LDE_Nodo* hermano = lde->inicio;
    struct LDE_Nodo* hermano_anterior = NULL;
    for (size_t i=0; i<pos; i++){
        hermano_anterior = hermano;
        hermano = hermano->posterior;
    }
    if (hermano != NULL){
        nodo->anterior = hermano->anterior;
        nodo->posterior = hermano;
        if (nodo->anterior != NULL) nodo->anterior->posterior = nodo;
        else lde->inicio = nodo;
        nodo->posterior->anterior = nodo;
    } else {
        nodo->anterior = hermano_anterior;
        if (nodo->anterior != NULL) nodo->anterior->posterior = nodo;
        else lde->inicio = nodo;
        lde->final = nodo;
    }
    lde->tamano++;
    return nodo;
}

struct LDE_Nodo* LDE_Get(struct LDE_LDE* lde, size_t pos){
    if (pos >= lde->tamano) return NULL;
    struct LDE_Nodo* nodo = lde->inicio;
    for (size_t i=0; i<pos; i++){
        nodo = nodo->posterior;
    }
    return nodo;
}

int LDE_Remove(struct LDE_LDE* lde, size_t pos){
    if (pos >= lde->tamano) return 1;
    struct LDE_Nodo* nodo = LDE_Get(lde, pos);
    if (nodo == NULL) return 1;
    if (nodo->anterior != NULL) nodo->anterior->posterior = nodo->posterior;
    else lde->inicio = nodo->posterior;
    if (nodo->posterior != NULL) nodo->posterior->anterior = nodo->anterior;
    else lde->final = nodo->anterior;
    free(nodo);
    lde->tamano--;
    return 0;
}

inline size_t LDE_Size(struct LDE_LDE* lde){
    return lde->tamano;
}
