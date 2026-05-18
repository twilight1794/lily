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

enum lily_lde_estados lily_lde_get_idx(const struct lily_lde_lde* lde, const struct lily_lde_nodo* nodo, size_t* pos) {
    if (lde == NULL || nodo == NULL) return LILY_LDE_ERROR;
    struct lily_lde_nodo* nodo_tentativo = lde->inicio;
    for (size_t i=0; i<lde->tamano; i++) {
        if (nodo_tentativo == nodo) {
            *pos = i;
            return LILY_LDE_OK;
        }
    }
    return LILY_LDE_NODO_EXTRANO;
}

enum lily_lde_estados lily_lde_remove(struct lily_lde_lde* lde, size_t pos) {
    if (lde == NULL || pos >= lde->tamano) return LILY_LDE_ERROR;
    return lily_lde_remove_node(lde, lily_lde_get(lde, pos));
}

enum lily_lde_estados lily_lde_remove_node(struct lily_lde_lde* lde, struct lily_lde_nodo* nodo) {
    if (lde == NULL || nodo == NULL) return LILY_LDE_ERROR;
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
