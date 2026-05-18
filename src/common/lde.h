/*
 * Copyright (C) 2024-2026 Giovanni Alfredo Garciliano Diaz
 *
 * This file is part of Lily.
 *
 * Lily is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Lily is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Lily. If not, see <https://www.gnu.org/licenses/>.
 */

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
 * Añade un valor a la lista
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
 * Quita un elemento de la lista
 * @param lde Lista a manipular
 * @param pos Posición del valor a eliminar
 * @return \c LILY_LDE_OK si se eliminó el nodo, \c LILY_LDE_ERROR si no
 */
enum lily_lde_estados lily_lde_remove(struct lily_lde_lde* lde, size_t pos);

/**
 * Quita un nodo de la lista
 * @param lde Lista a manipular
 * @param nodo Nodo a eliminar
 * @return \c LILY_LDE_OK si se eliminó el nodo, \c LILY_LDE_ERROR si no
 */
enum lily_lde_estados lily_lde_remove_node(struct lily_lde_lde* lde, struct lily_lde_nodo* nodo);

/**
 * Devuelve el número de elementos de la lista
 * @param lde Lista a manipular
 * @return Número de elementos de la lista
 */
size_t lily_lde_size(const struct lily_lde_lde* lde);

// Macros para funciones auxiliares
#define lily_lde_push(lde, valor) lily_lde_insert(lde, lily_lde_size(lde), valor)

#define lily_lde_pop(lde) lily_lde_remove(lde, lily_lde_size(lde)-(lily_lde_size(lde)>0))

#define lily_lde_shift(lde) lily_lde_remove(lde, 0)

#define lily_lde_unshift(lde, valor) lily_lde_insert(lde, 0, valor)

#define lily_lde_foreach(lde, nodo_actual) for (struct lily_lde_nodo* nodo_actual = lily_lde_get(lde, 0); nodo_actual != NULL; nodo_actual = nodo_actual->posterior)

#define lily_lde_foreachm(lde, nodo_actual, nodo_posterior) for (struct lily_lde_nodo* nodo_actual = lily_lde_get(lde, 0), *nodo_posterior = nodo_actual->posterior; nodo_actual != NULL; nodo_actual = nodo_posterior, ((nodo_posterior==NULL)?NULL:(nodo_posterior=nodo_posterior->posterior)))

#endif
