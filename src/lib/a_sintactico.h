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
#include "../lib/simbolo.h"

#define lista_actual ((lista_desplazamiento != NULL)?lista_desplazamiento:(instruccion->params))
#define pila_actual ((pila_desplazamiento != NULL)?pila_desplazamiento:pila_simbolos)

/**
 * Función de entrada del analizador sintáctico
 * @param simbolos Lista de símbolos encontrados en el análisis léxico
 * @param [out] ctx Estado de la ejecución al momento de salir de la función, si no es \c NULL
 * @return Árbol de sintaxis abstracta resultante
 */
struct lily_lde_lde* lily_a_sintactico(struct lily_lde_lde *simbolos, struct lily_ctx* ctx);

#endif
