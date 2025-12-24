#include "dict.h"

struct dict_dict* lily_dict_create(void) {
    struct dict_dict* obj = (struct dict_dict*) calloc(1, sizeof(struct dict_dict));
    if (obj == NULL) return NULL;
    obj->raiz = NULL;
    obj->tamano = 0;
    return obj;
}

struct dict_nodo* lily_dict_nodo_create(void) {
    struct dict_nodo* obj = (struct dict_nodo*) calloc(1, sizeof(struct dict_nodo));
    if (obj == NULL) return NULL;
    obj->padre = NULL;
    obj->izquierda = NULL;
    obj->derecha = NULL;
    obj->clave = NULL;
    obj->valor = NULL;
    return obj;
}

void Dict_IntercambiarNodos(struct dict_dict* dict, struct dict_nodo* u, struct dict_nodo* v) {
    if (u->padre == NULL) dict->raiz = v;
    else if (u == u->padre->izquierda) u->padre->izquierda = v;
    else u->padre->derecha = v;
    if (v != NULL) v->padre = u->padre;
}

struct dict_nodo* lily_dict_insert(struct dict_dict* dict, char* clave, void* valor, void** valor_ant) {
    int comp;
    struct dict_nodo* padre = NULL;
    struct dict_nodo* nodo = dict->raiz;
    while (nodo != NULL) {
        padre = nodo;
        comp = strcmp(clave, nodo->clave);
        if (comp < 0) nodo = nodo->izquierda;
        else if (comp > 0) nodo = nodo->derecha;
        else {
            // La clave ya existe
            if (valor_ant != NULL) *valor_ant = nodo->valor;
            nodo->valor = valor;
            return nodo;
        }
    }
    struct dict_nodo* nuevo = lily_dict_nodo_create();
    if (nuevo == NULL) return NULL;
    nuevo->padre = padre;
    nuevo->clave = clave;
    nuevo->valor = valor;
    if (padre == NULL) dict->raiz = nuevo;
    else {
        comp = strcmp(clave, padre->clave);
        if (comp < 0) padre->izquierda = nuevo;
        else if (comp > 0) padre->derecha = nuevo;
    }
    dict->tamano++;
    return nuevo;
}

struct dict_nodo* lily_dict_get(const struct dict_dict* dict, const char* clave) {
    struct dict_nodo* nodo = dict->raiz;
    int comp;
    while (nodo != NULL && ((comp = strcmp(clave, nodo->clave)), comp)) {
        if (comp < 0) nodo = nodo->izquierda;
        else if (comp > 0) nodo = nodo->derecha;
    }
    return nodo;
}

enum dict_estados lily_dict_remove(struct dict_dict* dict, const char* clave) {
    struct dict_nodo* nodo = lily_dict_get(dict, clave);
    if (nodo == NULL) return LILY_DICT_ERROR;
    if (nodo->izquierda == NULL) Dict_IntercambiarNodos(dict, nodo, nodo->derecha);
    else if (nodo->derecha == NULL) Dict_IntercambiarNodos(dict, nodo, nodo->izquierda);
    else {
        // Sucesor
        struct dict_nodo* sucesor;
        struct dict_nodo* nodo_temp = nodo;
        if (nodo_temp->derecha != NULL) {
            // Minimo
            struct dict_nodo* nodo_temp2 = nodo_temp->derecha;
            while (nodo_temp2->izquierda != NULL) {
                nodo_temp2 = nodo_temp2->izquierda;
            }
            sucesor = nodo_temp2;
            // End Minimo
        } else {
            sucesor = nodo_temp->padre;
            while (sucesor != NULL && nodo_temp == sucesor->derecha) {
                nodo_temp = sucesor;
                sucesor = sucesor->padre;
            }
        }
        // End Sucesor
        if (sucesor->padre != nodo) {
            Dict_IntercambiarNodos(dict, sucesor, sucesor->derecha);
            sucesor->derecha = nodo->derecha;
            sucesor->derecha->padre = sucesor;
        }
        Dict_IntercambiarNodos(dict, nodo, sucesor);
        sucesor->izquierda = nodo->izquierda;
        sucesor->izquierda->padre = sucesor;
    }
    free(nodo);
    dict->tamano--;
    return LILY_DICT_OK;
}

size_t lily_dict_size(const struct dict_dict* dict) {
    return dict->tamano;
}

struct dict_iterador* lily_dict_iterador_create(const struct dict_dict* dict) {
    struct dict_iterador* obj = (struct dict_iterador*) malloc(sizeof(struct dict_iterador));
    if (obj == NULL) return NULL;
    obj->dict = dict;
    obj->nodo = dict->raiz;
    obj->fin = false;
    return obj;
}

enum dict_estados lily_dict_iterate(struct dict_iterador* iter, struct dict_nodo** nodo) {
    if (iter == NULL) return LILY_DICT_ERROR;
    if (iter->dict->raiz == NULL) return LILY_DICT_FIN; // <- No hay nodos por los cuales iterar
    if (iter->fin == true) return LILY_DICT_FIN; // <- Ya habíamos acabado
    // Me devuevlo
    *nodo = iter->nodo;
    // Busco el siguiente nodo
    if (iter->nodo->izquierda != NULL) {
        // Sigamos a la izquierda
        iter->nodo = iter->nodo->izquierda;
    } else if (iter->nodo->derecha != NULL) {
        // Sigamos a la derecha
        iter->nodo = iter->nodo->derecha;
    } else {
        // Llegamos a una cerrada: subamos hasta encontrar dónde más podemos bajar
        while (true) {
            // Subir un nivel
            struct dict_nodo* nodo_ant = iter->nodo;
            iter->nodo = iter->nodo->padre;
            // ¿Acabé de visitar?
            if (iter->nodo == NULL) return LILY_DICT_FIN;
            // ¿Voy a la derecha o acabé en este nivel?
            if (iter->nodo->izquierda == nodo_ant && iter->nodo->derecha != NULL) {
                // Venía de la izquierda, voy a la derecha
                iter->nodo = iter->nodo->derecha;
                return LILY_DICT_OK;
            }
            // En este punto, ya venía de la derecha, o no puedo ir a la derecha
        }
    }
    return LILY_DICT_OK;
}