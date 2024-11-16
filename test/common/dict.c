#include <stdio.h>
#include "dict.h"

void print_nodo(struct Dict_Nodo* nodo){
    printf("(%p) Eti: %s, Val: %s, Izq: %p, Der: %p, padre: %p\n", (void *) nodo, nodo->clave, (char *) nodo->valor, (void *) nodo->izquierda, (void *) nodo->derecha, (void *) nodo->padre);
    return;
}

int main(void){
    struct Dict_Dict* midic = Dict_Create();
    printf("Tam: %lu\n", Dict_Size(midic));
    struct Dict_Nodo* nom = Dict_Insert(midic, "nombre", "Lirio Aketzalli");
    printf("Tam: %lu\n", Dict_Size(midic));
    struct Dict_Nodo* ap = Dict_Insert(midic, "apellido", "Mendoza de los Santos");
    printf("Tam: %lu\n", Dict_Size(midic));
    struct Dict_Nodo* nac = Dict_Insert(midic, "fnac", "31 may 2000");
    printf("Tam: %lu\n", Dict_Size(midic));
    printf("Raíz: %p\n", (void *) midic->raiz);
    print_nodo(nom);
    print_nodo(ap);
    print_nodo(nac);
    print_nodo(Dict_Get(midic, "nombre"));
    print_nodo(Dict_Get(midic, "apellido"));
    print_nodo(Dict_Get(midic, "fnac"));
    printf("(%p), casa\n", (void*) Dict_Get(midic, "casa"));
    struct Dict_Nodo* edad = Dict_Insert(midic, "fnac", "24");
    printf("(%p), edad\n", (void *) edad);
    Dict_Remove(midic, "nombre");
    printf("Tam: %lu\n", Dict_Size(midic));
    Dict_Remove(midic, "fnac");
    printf("Tam: %lu\n", Dict_Size(midic));
    Dict_Remove(midic, "escuela");
    printf("Tam: %lu\n", Dict_Size(midic));
    Dict_Remove(midic, "apellido");
    printf("Tam: %lu\n", Dict_Size(midic));
    free(midic);

    return 0;
}
