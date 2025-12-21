/**
 * @file dict.h
 * Implementación de un diccionario
 */

#ifndef LILY_C_DICT
#define LILY_C_DICT

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/**
 * Posibles estados durante una iteración
 */
enum Dict_Estados
{
    DICT_OK, ///< Se puede seguir iterando
    DICT_ERROR, ///< Hay un error que impide iterar
    DICT_FIN ///< Ya no hay más nodos por los qué iterar
};

/**
 * Objeto para almacenar un elemento de un diccionario
 */
struct Dict_Nodo {
    struct Dict_Nodo* padre; ///< @private Nodo padre del nodo. NULL si es el nodo raíz
    struct Dict_Nodo* izquierda; ///< @private Hijo izquierdo del nodo
    struct Dict_Nodo* derecha; ///< @private Hijo derecho del nodo
    char* clave; ///< Clave de la entrada de diccionario
    void* valor; ///< Valor de la entrada de diccionario
};

/*
 * Objeto base para almacenar un diccionario
 */
struct Dict_Dict {
    struct Dict_Nodo* raiz; ///< @private Nodo raíz del árbol asociado a este diccionario
    size_t tamano; ///< @private Número de entradas existentes en el diccionario
};

// Operaciones base

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
 * @param[out] valor_ant Puntero para almacenar el valor actual de la clave, si fuera diferente de NULL, y si existiera
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

// Iteraciones

/**
 * Objeto para guardar el estado de una iteración sobre un diccionario
 */
struct Dict_Iterador {
    const struct Dict_Dict* dict; ///< @private Diccionario sobre el cual se trabajará
    struct Dict_Nodo* nodo; ///< @private Nodo sobre el que nos encontramos actualmente
    bool fin; ///< Indica si se ha terminado de iterar sobre el diccionario
};

/**
 * Crea un iterador para un diccionario
 * @param dict Diccionario a iterar
 * @return Puntero a un iterador, o NULL si hubo un error
 */
struct Dict_Iterador* Dict_Crear_Iterador(const struct Dict_Dict* dict);

/**
 * Avanza un elemento en el recorrido del diccionario
 * @param iter Iterador a utilizar
 * @param[out] nodo Nodo devuelto
 * @return 0 mientras haya nodos para devolver, 1 si es el último nodo, 2 si hubo un error
 */
int Dict_Iterar(struct Dict_Iterador* iter, struct Dict_Nodo** nodo);

#endif