#include "lde.h"

struct lily_lde_lde* lily_lde_create(void) {
    struct lily_lde_lde* lde = (struct lily_lde_lde*) calloc(1, sizeof(struct lily_lde_lde));
    if (lde == NULL) return NULL;
    lde->inicio = NULL;
    lde->final = NULL;
    lde->tamano = 0;
    return lde;
}

struct lily_lde_nodo* lily_lde_nodo_create(void) {
    struct lily_lde_nodo* nodo = (struct lily_lde_nodo*) calloc(1, sizeof(struct lily_lde_nodo));
    if (nodo == NULL) return NULL;
    nodo->anterior = NULL;
    nodo->posterior = NULL;
    nodo->valor = NULL;
    return nodo;
}

struct lily_lde_nodo* lily_lde_insert(struct lily_lde_lde* lde, size_t pos, void* valor) {
    if (pos > lde->tamano) return NULL;
    struct lily_lde_nodo* nodo = lily_lde_nodo_create();
    if (nodo == NULL) return NULL;
    nodo->valor = valor;
    struct lily_lde_nodo* hermano = lde->inicio;
    struct lily_lde_nodo* hermano_anterior = NULL;
    for (size_t i=0; i<pos; i++) {
        hermano_anterior = hermano;
        hermano = hermano->posterior;
    }
    if (hermano != NULL) {
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

struct lily_lde_nodo* lily_lde_get(const struct lily_lde_lde* lde, size_t pos) {
    if (pos >= lde->tamano) return NULL;
    struct lily_lde_nodo* nodo = lde->inicio;
    for (size_t i=0; i<pos; i++) {
        nodo = nodo->posterior;
    }
    return nodo;
}

enum lily_lde_estados lily_lde_remove(struct lily_lde_lde* lde, size_t pos) {
    if (pos >= lde->tamano) return LILY_LDE_ERROR;
    struct lily_lde_nodo* nodo = lily_lde_get(lde, pos);
    if (nodo == NULL) return LILY_LDE_ERROR;
    if (nodo->anterior != NULL) nodo->anterior->posterior = nodo->posterior;
    else lde->inicio = nodo->posterior;
    if (nodo->posterior != NULL) nodo->posterior->anterior = nodo->anterior;
    else lde->final = nodo->anterior;
    free(nodo);
    lde->tamano--;
    return 0;
}

inline size_t lily_lde_size(const struct lily_lde_lde* lde) {
    return lde->tamano;
}
