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
#include "../common/error.h"
#include "../common/lde.h"
#include "../common/log.h"
#include "../common/nums.h"
#include "a_lexico_simbolo.h"
#include "lua_cpu.h"

/**
 * Añade un nuevo identificador a la lista de identificadores conocidos
 * @param identificadores Tabla de símbolos
 * @param identificador Identificador a añadir
 * @param valor Entero asociado al identificador
 * @param es_const Si el identificador será constante o variable
 * @param [out] ctx Contexto del estado de la operación
 */
static void lily_a_semantico_anad_identificador(struct lily_dict_dict* identificadores, char* identificador, union lily_a_lexico_numero* valor, bool es_const, struct lily_error_ctx* ctx);

/**
 * Ensambla una directiva
 * @param instruccion Directiva a procesar
 * @param identificadores Tabla de símbolos
 * @param pc Contador de instrucción
 * @param [out] ctx Contexto del estado de la operación
 */
static void lily_a_semantico_directiva(struct lily_a_sintactico_instruccion* instruccion, struct lily_dict_dict* identificadores, const size_t* pc, struct lily_error_ctx* ctx);

/**
 * Devuelve la arquitectura declarada en el archivo ensamblador
 * @param ast Árbol de sintaxis abstracta generado en el análisis sintáctico
 * @param ctx Contexto del estado de la operación
 * @return Cadena con el identificador de la arquitectura
 */
char* lily_a_semantico_obt_arquitectura_declarada(struct lily_lde_lde* ast, struct lily_error_ctx* ctx);

/**
 * Función de entrada del analizador semántico
 * @param ast Árbol de sintaxis abstracta generado en el análisis sintáctico
 * @param L Sesión de Lua
 * @param pc_inicial Valor predeterminado para el contador de programa
 * @param [out] tam Número de bytes generados
 * @param [out] ctx Contexto del estado de la operación
 * @return Array de bytes resultante
 */
uint8_t* lily_a_semantico(struct lily_lde_lde *ast, lua_State* L, size_t pc_inicial, size_t* tam, struct lily_error_ctx* ctx);
#endif