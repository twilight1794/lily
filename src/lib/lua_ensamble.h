/*
 * @file lua_ensamble.h
 * Funciones para ensamblar según un esquema de descripción de microprocesador
 */

#ifndef LILY_L_LUA_ENSAMBLE
#define LILY_L_LUA_ENSAMBLE

#include "../common/cadena.h"
#include "../common/defs.h"
#include "../common/estado.h"
#include "../common/lde.h"
#include "../common/log.h"
#include  "a_lexico_simbolo.h"

#include <libintl.h>
#include <lua.h>

/**
 * Comprueba si \a simbolo cumple con alguno de los tipos conocidos por Lily
 * @param L Sesión de Lua
 * @param tipo Tipo contra el cual comprobar
 * @param simbolo Símbolo a comprobar
 * @param [out] ctx Estado de la ejecución al momento de salir de la función
 * @return \c true si el símbolo coincide con el tipo, \c false si no
 */
static bool lily_lua_cpu_comp_tipo_simbolo(lua_State* L, const char* tipo, struct lily_a_lexico_simbolo* simbolo, struct lily_ctx* ctx);

/**
 * Lee la lista de parámetros en \a params para una instrucción, y prepara sus valores en la pila para Lua
 * @param L Sesión de Lua
 * @param params Lista de parámetros para el mnemónico
 * @param [out] ctx Estado de la ejecución al momento de salir de la función
 */
static void lily_lua_cpu_est_parametros(lua_State* L, struct lily_lde_lde* params, struct lily_ctx* ctx);

/**
 * Obtiene el resultado del ensamble de una instrucción
 * @param L Sesión de Lua
 * @param [out] tam Número de bytes generados al ensamblar
 * @param [out] ctx Estado de la ejecución al momento de salir de la función
 * @return Cadena de bytes que representan al mnemónico
 */
static uint8_t* lily_lua_cpu_procesar_resultado(lua_State* L, lua_Integer* tam, struct lily_ctx* ctx);

/**
 * Ensambla un mnemónico definido como una función
 * @param L Sesión de Lua
 * @param instruccion Instrucción que contiene el mnemónico a ensamblar
 * @param ctx Estado de la ejecución al momento de salir de la función
 */
static void lily_lua_cpu_ensamblar_funcion(lua_State* L, struct lily_a_sintactico_instruccion* instruccion, struct lily_ctx* ctx);

/**
 * Ensambla un mnemónico definido como una lista de bytes
 * @param L Sesión de Lua
 * @param instruccion Instrucción que contiene el mnemónico a ensamblar
 * @param ctx Estado de la ejecución al momento de salir de la función
 */
static void lily_lua_cpu_ensamblar_lista(lua_State* L, struct lily_a_sintactico_instruccion* instruccion, struct lily_ctx* ctx);

/**
 * Ensambla un mnemónico definido como una redirección a otro mnemónico
 * @param L Sesión de Lua
 * @param instruccion Instrucción que contiene el mnemónico a ensamblar
 * @param ctx Estado de la ejecución al momento de salir de la función
 */
static void lily_lua_cpu_ensamblar_redireccion(lua_State* L, struct lily_a_sintactico_instruccion* instruccion, struct lily_ctx* ctx);

/**
 * Ensambla un mnemónico definido como una lista de diferentes combinaciones de argumentos
 * @param L Sesión de Lua
 * @param instruccion Instrucción que contiene el mnemónico a ensamblar
 * @param ctx Estado de la ejecución al momento de salir de la función
 */
static void lily_lua_cpu_ensamblar_lparams(lua_State* L, struct lily_a_sintactico_instruccion* instruccion, struct lily_ctx* ctx);

/**
 * Ensambla un mnemónico
 * @param L Sesión de Lua
 * @param instruccion Instrucción que contiene el mnemónico a ensamblar
 * @param [out] ctx Estado de la ejecución al momento de salir de la función
 */
void lily_lua_cpu_ensamblar(lua_State* L, struct lily_a_sintactico_instruccion* instruccion, struct lily_ctx* ctx);

#endif