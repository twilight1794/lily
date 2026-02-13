/*
 * @file a_sintactico.h
 * Analizador sintáctico del preprocesador Lily
 */

#ifndef LILY_L_A_SINTACTICO
#define LILY_L_A_SINTACTICO

#include <stdbool.h>

#include "../common/defs.h"
#include "../common/error.h"
#include "../common/lde.h"
#include "../common/log.h"
#include "../lib/a_lexico_simbolo.h"

/**
 * Estructura para almacenar detalles sobre la ejecución del análisis sintáctico
 */
struct lily_a_sintactico_ctx {
    enum lily_error codigo; /**< Código a retornar */
    struct lily_a_lexico_simbolo* ultimo; /**< Último símbolo procesado antes de fallar */
};

/**
 * Función de entrada del analizador sintáctico
 * @param simbolos Lista de símbolos encontrados en el análisis léxico
 * @param [out] ast Árbol de sintaxis abstracta resultante
 * @param [out] ctx Estado de la ejecución al momento de salir de la función, si no es \c NULL
 * @return Código de estado de la operación
 */
enum lily_error lily_a_sintactico(struct lily_lde_lde* simbolos, struct lily_lde_lde* ast, struct lily_a_sintactico_ctx* ctx);

#endif
