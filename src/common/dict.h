/**
 * @file dict.h
 * Implementación de un diccionario
 */

#ifndef LILY_C_DICT
#define LILY_C_DICT

#include <stdlib.h>
#include <string.h>

struct Dict_Nodo {
    struct Dict_Nodo* padre;
    struct Dict_Nodo* izquierda;
    struct Dict_Nodo* derecha;
    char* clave;
    void* valor;
};

struct Dict_Dict {
    struct Dict_Nodo* raiz;
    size_t tamano;
};

/**
 * Crea un diccionario vacío
 * @return Puntero a un diccionario, o NULL si hubo un error
 */
struct Dict_Dict* Dict_Create(void);

/**
 * Crea un nodo de diccionario vacío
 * @return Puntero a un nodo de diccionario, o NULL si hubo un error
 */
struct Dict_Nodo* Dict_Nodo_Create(void);

/**
 * Añade una clave al diccionario
 * @param dict Diccionario a manipular
 * @param clave Clave a añadir
 * @param valor Valor a añadir
 * @param valor_ant Puntero para almacenar el valor actual de la clave, si fuera diferente de NULL, y si existiera
 * @return Nodo recién insertado, o NULL si hubo un error
 */
struct Dict_Nodo* Dict_Insert(struct Dict_Dict* dict, char* clave, void* valor, void** valor_ant);

/**
 * Devuelve el valor de una clave del diccionario
 * @param dict Diccionario a manipular
 * @param clave Clave a devolver
 * @return Nodo que contiene la clave, o NULL si hubo un error o no existe la clave
 */
struct Dict_Nodo* Dict_Get(const struct Dict_Dict* dict, const char* clave);

/**
 * Quita una clave del diccionario
 * @param dict Diccionario a manipular
 * @param clave Clave a eliminar
 * @return 0 si se eliminó el nodo, 1 si no
 */
int Dict_Remove(struct Dict_Dict* dict, const char* clave);

/**
 * Devuelve el número de elementos del diccionario
 * @param dict Diccionario a manipular
 * @return Número de elementos del diccionario
 */
size_t Dict_Size(const struct Dict_Dict* dict);

#endif
