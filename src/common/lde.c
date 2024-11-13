#include "lde.h"

struct LDE_LDE* LDE_Create(void){
    struct LDE_LDE* lde = (struct LDE_LDE*) calloc(1, sizeof(struct LDE_LDE));
    return lde;
}

struct LDE_Nodo* LDE_Nodo_Create(void){
    struct LDE_Nodo* nodo = (struct LDE_Nodo*) calloc(1, sizeof(struct LDE_Nodo));
    return nodo;
}

struct LDE_Nodo* LDE_Insert(struct LDE_LDE* lde, size_t pos, void* valor){
    if (pos >= lde->tamano) return NULL;
    struct LDE_Nodo* nodo = LDE_Nodo_Create();
    struct LDE_Nodo* hermano = lde->inicio;
    nodo->valor = valor;
    if (hermano){
        // Lista no vacía
        for (size_t i=0; i<pos; i++){
            hermano = hermano->posterior;
        }
        nodo->posterior = hermano->posterior;
        nodo->anterior = hermano;
        if (nodo->anterior) nodo->anterior->posterior = nodo;
        if (nodo->posterior) nodo->posterior->anterior = nodo;
    } else {
        // Lista vacía
        lde->inicio = nodo;
    }
    return nodo;
}

struct LDE_Nodo* LDE_Get(struct LDE_LDE* lde, size_t pos){
    if (pos >= lde->tamano) return NULL;
    struct LDE_Nodo* nodo = lde->inicio;
    while (pos){
        nodo = nodo->posterior;
        pos--;
    }
    return nodo;
}

int LDE_Remove(struct LDE_LDE* lde, size_t pos){
    if (pos >= lde->tamano) return 1;
    struct LDE_Nodo* nodo = lde->inicio;
    for (size_t i=0; i<pos; i++){
        nodo = nodo->posterior;
    }
    if (nodo->anterior) nodo->anterior->posterior = nodo->posterior;
    if (nodo->posterior) nodo->posterior->anterior = nodo->anterior;
    free(nodo);
    return 0;
}

inline size_t LDE_Size(struct LDE_LDE* lde){
    return lde->tamano;
}
