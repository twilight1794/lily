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

#define tiene_etiqueta() instruccion->simbolo != NULL

/**
 * Estructura para almacenar detalles sobre la ejecución del análisis sintáctico
 */
struct lily_a_sintactico_ctx {
    enum lily_error codigo; /**< Código a retornar */
    struct lily_a_lexico_simbolo* ultimo; /**< Último símbolo procesado antes de fallar */
};

/**
 * Devuelve el nivel de precedencia de un operador
 * @param operador Operador en cuestión
 * @return Nivel de precedencia de \a operador
 */
static unsigned int lily_a_sintactico_precedencia(const struct lily_a_lexico_simbolo* operador);

/**
 * Modo para procesar un mnemónico o directiva para el análisis sintáctico
 * @param [in,out] simbolos Listado de símbolos
 * @param [in,out] nodo Nodo desde el cual empezar a procesar en este modo
 * @þaram [in,out] instruccion Objeto para almacenar la instrucción procesada
 * @return Contexto del estado de la operación
 */
static struct lily_a_sintactico_ctx lily_a_sintactico_modo_instruccion(struct lily_lde_lde* simbolos, struct lily_lde_nodo** nodo, struct lily_a_sintactico_instruccion* instruccion);

/**
 * Función de entrada del analizador sintáctico
 * @param simbolos Lista de símbolos encontrados en el análisis léxico
 * @param [out] ast Árbol de sintaxis abstracta resultante
 * @return Contexto de estado de la operación
 */
struct lily_a_sintactico_ctx lily_a_sintactico(struct lily_lde_lde *simbolos, struct lily_lde_lde *ast);

#endif
