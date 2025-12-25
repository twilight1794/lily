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
 * Estados para una iteración
 */
enum lily_dict_estados {
    LILY_DICT_OK, ///< Se puede seguir iterando.
    LILY_DICT_ERROR, ///< Hay un error que impide iterar.
    LILY_DICT_FIN ///< Ya no hay más nodos por los qué iterar.
};

/**
 * Par clave-valor de un diccionario
 */
struct lily_dict_nodo {
    struct lily_dict_nodo* padre; ///< @private Nodo padre del nodo. NULL si es el nodo raíz.
    struct lily_dict_nodo* izquierda; ///< @private Hijo izquierdo del nodo.
    struct lily_dict_nodo* derecha; ///< @private Hijo derecho del nodo.
    char* clave; ///< Clave de la entrada de diccionario.
    void* valor; ///< Valor de la entrada de diccionario.
};

/*
 * Objeto base para almacenar un diccionario
 */
struct lily_dict_dict {
    struct lily_dict_nodo* raiz; ///< @private Nodo raíz del árbol asociado a este diccionario.
    size_t tamano; ///< Número de entradas existentes en el diccionario.
};

// Operaciones base

/**
 * Crea un diccionario vacío
 * @return Puntero a un diccionario, o NULL si hubo un error
 */
struct lily_dict_dict* lily_dict_create(void);

/**
 * Crea un nodo de diccionario vacío
 * @return Puntero a un nodo de diccionario, o NULL si hubo un error
 */
struct lily_dict_nodo* lily_dict_nodo_create(void);

/**
 * Añade una clave al diccionario
 * @param dict Diccionario a manipular
 * @param clave Clave a añadir
 * @param valor Valor a añadir
 * @param[out] valor_ant Puntero para almacenar el valor actual de la clave, si fuera diferente de NULL, y si existiera
 * @return Nodo recién insertado, o NULL si hubo un error
 */
struct lily_dict_nodo* lily_dict_insert(struct lily_dict_dict* dict, char* clave, void* valor, void** valor_ant);

/**
 * Devuelve el valor de una clave del diccionario
 * @param dict Diccionario a manipular
 * @param clave Clave a devolver
 * @return Nodo que contiene la clave, o NULL si hubo un error o no existe la clave
 */
struct lily_dict_nodo* lily_dict_get(const struct lily_dict_dict* dict, const char* clave);

/**
 * Quita una clave del diccionario
 * @param dict Diccionario a manipular
 * @param clave Clave a eliminar
 * @return \c LILY_DICT_OK si se eliminó el nodo, \c LILY_DICT_ERROR si no
 */
enum lily_dict_estados lily_dict_remove(struct lily_dict_dict* dict, const char* clave);

/**
 * Devuelve el número de elementos del diccionario
 * @param dict Diccionario a manipular
 * @return Número de elementos del diccionario
 */
size_t lily_dict_size(const struct lily_dict_dict* dict);

// Iteraciones

/**
 * Objeto para guardar el estado de una iteración sobre un diccionario
 */
struct lily_dict_iterador {
    const struct lily_dict_dict* dict; ///< @private Diccionario sobre el cual se trabajará.
    struct lily_dict_nodo* nodo; ///< @private Nodo sobre el que nos encontramos actualmente.
    bool fin; ///< Indica si se ha terminado de iterar sobre el diccionario.
};

/**
 * Crea un iterador para un diccionario
 * @param dict Diccionario a iterar
 * @return Puntero a un iterador, o NULL si hubo un error
 */
struct lily_dict_iterador* lily_dict_iterador_create(const struct lily_dict_dict* dict);

/**
 * Avanza un elemento en el recorrido del diccionario
 * @param iter Iterador a utilizar
 * @param[out] nodo Nodo devuelto
 * @return \c LILY_DICT_OK mientras haya nodos para devolver, \c LILY_DICT_FIN si es el último nodo, \c LILY_DICT_ERROR si hubo un error
 */
enum lily_dict_estados lily_dict_iterate(struct lily_dict_iterador* iter, struct lily_dict_nodo** nodo);

#endif