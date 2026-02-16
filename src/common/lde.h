/**
 * @file lde.h
 * Funciones para implementar una lista doblemente enlazada
 */

#ifndef LILY_C_LDE
#define LILY_C_LDE

#include <stdlib.h>
#include <string.h>

/**
 * Estados para LDE
 */
enum lily_lde_estados {
    LILY_LDE_OK,
    LILY_LDE_ERROR,
    LILY_LDE_NODO_EXTRANO
};

/**
 * Elemento de una lista
 */
struct lily_lde_nodo {
    struct lily_lde_nodo* anterior; ///< @private Elemento antecesor en la lista. NULL si es el primero.
    struct lily_lde_nodo* posterior; ///< @private Elemento sucesor en la lista. NULL si es el último.
    void* valor; ///< Puntero al valor de un elemento de la lista.
};

/**
 * Objeto base para almacenar una lista
 */
struct lily_lde_lde {
    struct lily_lde_nodo* inicio; ///< Primer elemento de la lista.
    struct lily_lde_nodo* final; ///< Último elemento de la lista.
    size_t tamano; ///< Número de elementos de la lista.
};

/**
 * Crea una lista vacía
 * @return Puntero a una lista
 */
struct lily_lde_lde* lily_lde_create(void);

/**
 * Crea un nodo de lista vacío
 * @return Puntero a un nodo de lista
 */
struct lily_lde_nodo* lily_lde_nodo_create(void);

/**
 * Añade un valor a la lista
 * @param lde Lista a manipular
 * @param pos Posición en la cual insertar el valor
 * @param valor Valor a añadir
 * @return Nodo recién insertado
 */
struct lily_lde_nodo* lily_lde_insert(struct lily_lde_lde* lde, size_t pos, void* valor);

/**
 * Devuelve el valor de una posición de la lista
 * @param lde Lista a manipular
 * @param pos Posición del valor a devolver
 * @return Nodo que contiene el valor
 */
struct lily_lde_nodo* lily_lde_get(const struct lily_lde_lde* lde, size_t pos);

/**
 * Devuelve la posición de un nodo en la lista
 * @param lde Lista a manipular
 * @param nodo Nodo a buscar
 * @param [out] pos Posición donde se halla \a nodo
 * @return \c LILY_LDE_OK si se encontró el valor, \c LILY_LDE_NODO_EXTRANO si el nodo no pertenece a la lista, \c LILY_LDE_ERROR en cualquier otro caso
 */
enum lily_lde_estados lily_lde_get_idx(const struct lily_lde_lde* lde, const struct lily_lde_nodo* nodo, size_t* pos);

/**
 * Quita una clave de la lista
 * @param lde Lista a manipular
 * @param pos Posición del valor a eliminar
 * @return \c LILY_LDE_OK si se eliminó el nodo, \c LILY_LDE_ERROR si no
 */
enum lily_lde_estados lily_lde_remove(struct lily_lde_lde* lde, size_t pos);

/**
 * Devuelve el número de elementos de la lista
 * @param lde Lista a manipular
 * @return Número de elementos de la lista
 */
size_t lily_lde_size(const struct lily_lde_lde* lde);

#endif
