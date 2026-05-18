/*
 * Copyright (C) 2026 Giovanni Alfredo Garciliano Diaz
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

/*
 * @file a_sintactico.h
 * Analizador sintáctico del preprocesador Lily
 */

#ifndef LILY_L_A_SINTACTICO
#define LILY_L_A_SINTACTICO

#include <stdbool.h>

#include "../common/defs.h"
#include "../common/estado.h"
#include "../common/lde.h"
#include "../common/log.h"
#include "mensajes.h"
#include "simbolo.h"

#define lista_actual ((lista_desplazamiento != NULL)?lista_desplazamiento:(instruccion->params))
#define pila_actual ((pila_desplazamiento != NULL)?pila_desplazamiento:pila_simbolos)

struct lily_a_sintactico_ctx {
    struct lily_simbolo_simbolo* ultimo; /**< Último símbolo procesado antes de fallar */
};

/**
 * Función de entrada del analizador sintáctico
 * @param simbolos Lista de símbolos encontrados en el análisis léxico
 * @param enviar_mensaje Función para enviar un mensaje
 * @param [out] estado Estado de la operación
 * @param [out] ctx Contexto de la operación
 * @return Árbol de sintaxis abstracta resultante
 */
struct lily_lde_lde* lily_a_sintactico(struct lily_lde_lde *simbolos, f_mensajes_ptr enviar_mensaje, enum lily_estado* estado, void** ctx);

#endif
