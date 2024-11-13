/**
 * @file lde.h
 * Funciones para implementar una lista doblemente enlazada
 */

#ifndef LILY_C_LDE
#define LILY_C_LDE

#include <stdlib.h>
#include <string.h>

struct LDE_Nodo {
    struct LDE_Nodo* anterior;
    struct LDE_Nodo* posterior;
    void* valor;
};

struct LDE_LDE {
    struct LDE_Nodo* inicio;
    struct LDE_Nodo* final;
    size_t tamano;
};

/**
 * Crea una lista vacía
 * @return Puntero a una lista
 */
struct LDE_LDE* LDE_Create(void);

/**
 * Crea un nodo de lista vacío
 * @return Puntero a un nodo de lista
 */
struct LDE_Nodo* LDE_Nodo_Create(void);

/**
 * Añade un valor a la lista
 * @param lde Lista a manipular
 * @param pos Posición en la cual insertar el valor
 * @param valor Valor a añadir
 * @return Nodo recién insertado
 */
struct LDE_Nodo* LDE_Insert(struct LDE_LDE* lde, size_t pos, void* valor);

/**
 * Devuelve el valor de una posición de la lista
 * @param lde Lista a manipular
 * @param pos Posición del valor a devolver
 * @return Nodo que contiene el valor
 */
struct LDE_Nodo* LDE_Get(struct LDE_LDE* lde, size_t pos);

/**
 * Quita una clave de la lista
 * @param lde Lista a manipular
 * @param pos Posición del valor a eliminar
 * @return 0 si se eliminó el nodo, 1 si no
 */
int LDE_Remove(struct LDE_LDE* lde, size_t pos);

/**
 * Devuelve el número de elementos de la lista
 * @param lde Lista a manipular
 * @return Número de elementos de la lista
 */
size_t LDE_Size(struct LDE_LDE* lde);

#endif
