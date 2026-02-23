/*
* @file a_semantico.h
 * Analizador semántico del preprocesador Lily
 */

#ifndef LILY_L_A_SEMANTICO
#define LILY_L_A_SEMANTICO

#include <stdbool.h>

#include "../common/defs.h"
#include "../common/error.h"
#include "../common/lde.h"
#include "../common/log.h"
#include "a_lexico_simbolo.h"
#include "lua_cpu.h"

/**
 * Estructura para almacenar detalles sobre la ejecución del análisis sintáctico
 */
struct lily_a_semantico_ctx {
    enum lily_error codigo; /**< Código a retornar */
    struct lily_a_lexico_simbolo* ultimo; /**< Último símbolo procesado antes de fallar */
};

/**
 * Función de entrada del analizador semántico
 * @param ast Árbol de sintaxis abstracta generado en el análisis sintáctico
 * @param [out] bytes Array de bytes resultante
 * @return Contexto de estado de la operación
 */
struct lily_a_semantico_ctx lily_a_semantico(struct lily_lde_lde *ast, char* bytes);

#endif