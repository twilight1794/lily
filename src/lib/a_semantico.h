/*
* @file a_semantico.h
 * Analizador semántico del preprocesador Lily
 */

#ifndef LILY_L_A_SEMANTICO
#define LILY_L_A_SEMANTICO

#include <stdbool.h>
#include <stdint.h>

#include "../common/defs.h"
#include "../common/dict.h"
#include "../common/estado.h"
#include "../common/lde.h"
#include "../common/log.h"
#include "../common/nums.h"
#include "lua_cpu.h"
#include "lua_ensamble.h"
#include "mensajes.h"
#include "simbolo.h"

/**
 * Nivel de reducción de una expresión
 */
enum lily_a_semantico_reduccion {
    SIN_REDUCCION, /**< La expresión no ha sido reducida */
    REDUCCION_TEMPORAL, /**< La expresión ha sido reducida con un valor temporal, y será reevaluada en iteraciones posteriores */
    REDUCCION_DEFINITIVA, /**< La expresión ha sido reducida definitivamente */
};

struct lily_a_semantico_ctx {
    struct lily_simbolo_simbolo* ultimo; /**< Último símbolo procesado antes de fallar */
    char* lua_msg; /**< Para errores de Lua, mensaje devuelto */
};

/**
 * Devuelve la arquitectura declarada en el archivo ensamblador
 * @param ast Árbol de sintaxis abstracta generado en el análisis sintáctico
 * @param [out] ctx Contexto del estado de la operación
 * @return Cadena con el identificador de la arquitectura
 */
char* lily_a_semantico_obt_arquitectura_declarada(struct lily_lde_lde* ast, enum lily_estado* estado, void** ctx);

/**
 * Función de entrada del analizador semántico
 * @param ast Árbol de sintaxis abstracta generado en el análisis sintáctico
 * @param L Sesión de Lua
 * @param pc_inicial Valor predeterminado para el contador de programa
 * @param [out] tam Número de bytes generados
 * @param enviar_mensaje Función para enviar un mensaje
 * @param [out] estado Estado de la operación
 * @param [out] ctx Contexto de la operación
 * @return Array de bytes resultante
 */
uint8_t* lily_a_semantico(struct lily_lde_lde *ast, lua_State* L, size_t pc_inicial, size_t* tam, f_mensajes_ptr enviar_mensaje, enum lily_estado* estado, void** ctx);

#endif
