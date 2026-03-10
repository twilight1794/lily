/*
* @file a_semantico.h
 * Analizador semántico del preprocesador Lily
 */

#ifndef LILY_L_A_SEMANTICO
#define LILY_L_A_SEMANTICO

#include <stdbool.h>
#include <stdint.h>

#include <libintl.h>

#include "../common/defs.h"
#include "../common/dict.h"
#include "../common/estado.h"
#include "../common/lde.h"
#include "../common/log.h"
#include "../common/nums.h"
#include "simbolo.h"
#include "lua_cpu.h"
#include "lua_ensamble.h"

/**
 * Devuelve la arquitectura declarada en el archivo ensamblador
 * @param ast Árbol de sintaxis abstracta generado en el análisis sintáctico
 * @param ctx Contexto del estado de la operación
 * @return Cadena con el identificador de la arquitectura
 */
char* lily_a_semantico_obt_arquitectura_declarada(struct lily_lde_lde* ast, struct lily_ctx* ctx);

/**
 * Función de entrada del analizador semántico
 * @param ast Árbol de sintaxis abstracta generado en el análisis sintáctico
 * @param L Sesión de Lua
 * @param pc_inicial Valor predeterminado para el contador de programa
 * @param [out] tam Número de bytes generados
 * @param [out] ctx Contexto del estado de la operación
 * @return Array de bytes resultante
 */
uint8_t* lily_a_semantico(struct lily_lde_lde *ast, lua_State* L, size_t pc_inicial, size_t* tam, struct lily_ctx* ctx);
#endif
