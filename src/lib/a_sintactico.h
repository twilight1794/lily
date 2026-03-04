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
#include "../lib/a_lexico_simbolo.h"

#define lista_actual ((lista_desplazamiento != NULL)?lista_desplazamiento:(instruccion->params))
#define pila_actual ((pila_desplazamiento != NULL)?pila_desplazamiento:pila_simbolos)

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
 * @param [out] ctx Estado de la ejecución al momento de salir de la función, si no es \c NULL
 * @return Contexto del estado de la operación
 */
static void lily_a_sintactico_modo_instruccion(struct lily_lde_lde* simbolos, struct lily_lde_nodo** nodo, struct lily_a_sintactico_instruccion* instruccion, struct lily_ctx* ctx);

/**
 * Función de entrada del analizador sintáctico
 * @param simbolos Lista de símbolos encontrados en el análisis léxico
 * @param [out] ctx Estado de la ejecución al momento de salir de la función, si no es \c NULL
 * @return Árbol de sintaxis abstracta resultante
 */
struct lily_lde_lde* lily_a_sintactico(struct lily_lde_lde *simbolos, struct lily_ctx* ctx);

#endif
