#include "dict.h"

struct Dict_Dict* Dict_Create(void){
    struct Dict_Dict* obj = (struct Dict_Dict*) calloc(1, sizeof(struct Dict_Dict));
    obj->raiz = NULL;
    obj->tamano = 0;
    return obj;
}

struct Dict_Nodo* Dict_Nodo_Create(void){
    struct Dict_Nodo* obj = (struct Dict_Nodo*) calloc(1, sizeof(struct Dict_Nodo));
    obj->padre = NULL;
    obj->izquierda = NULL;
    obj->derecha = NULL;
    obj->clave = NULL;
    obj->valor = NULL;
    return obj;
}

void Dict_IntercambiarNodos(struct Dict_Dict* dict, struct Dict_Nodo* u, struct Dict_Nodo* v){
    if (u->padre == NULL) dict->raiz = v;
    else if (u == u->padre->izquierda) u->padre->izquierda = v;
    else u->padre->derecha = v;
    if (v != NULL) v->padre = u->padre;
}

struct Dict_Nodo* Dict_Insert(struct Dict_Dict* dict, char* clave, void* valor, void** valor_ant){
    int comp;
    struct Dict_Nodo* padre = NULL;
    struct Dict_Nodo* nodo = dict->raiz;
    while (nodo != NULL){
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
    struct Dict_Nodo* nuevo = Dict_Nodo_Create();
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

struct Dict_Nodo* Dict_Get(struct Dict_Dict* dict, const char* clave){
    struct Dict_Nodo* nodo = dict->raiz;
    int comp;
    while (nodo != NULL && ((comp = strcmp(clave, nodo->clave)), comp)){
        if (comp < 0) nodo = nodo->izquierda;
        else if (comp > 0) nodo = nodo->derecha;
    }
    return nodo;
}

int Dict_Remove(struct Dict_Dict* dict, const char* clave){
    struct Dict_Nodo* nodo = Dict_Get(dict, clave);
    if (nodo == NULL) return 1;
    if (nodo->izquierda == NULL) Dict_IntercambiarNodos(dict, nodo, nodo->derecha);
    else if (nodo->derecha == NULL) Dict_IntercambiarNodos(dict, nodo, nodo->izquierda);
    else {
        // Sucesor
        struct Dict_Nodo* sucesor;
        struct Dict_Nodo* nodo_temp = nodo;
        if (nodo_temp->derecha != NULL){
            // Minimo
            struct Dict_Nodo* nodo_temp2 = nodo_temp->derecha;
            while (nodo_temp2->izquierda != NULL){
                nodo_temp2 = nodo_temp2->izquierda;
            }
            sucesor = nodo_temp2;
            // End Minimo
        } else {
            sucesor = nodo_temp->padre;
            while (sucesor != NULL && nodo_temp == sucesor->derecha){
                nodo_temp = sucesor;
                sucesor = sucesor->padre;
            }
        }
        // End Sucesor
        if (sucesor->padre != nodo){
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
    return 0;
}

size_t Dict_Size(struct Dict_Dict* dict){
    return dict->tamano;
}
