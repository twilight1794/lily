/*
 * @file lua_cpu.h
 * Funciones para cargar archivos de descripción de microprocesadores para Lily
 */
 
 #ifndef LILY_L_LUA_CPU
 #define LILY_L_LUA_CPU

#include "../common/cadena.h"
#include "../common/defs.h"
#include "../common/error.h"
#include "../common/log.h"
#include  "a_lexico_simbolo.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

enum lily_lua_cpu_userdata_globales {
    UD_TIPO_INT3,
    UD_TIPO_UINT3,
    UD_TIPO_SINT3,
    UD_TIPO_INT4,
    UD_TIPO_UINT4,
    UD_TIPO_SINT4,
    UD_TIPO_INT8,
    UD_TIPO_UINT8,
    UD_TIPO_SINT8,
    UD_TIPO_INT16,
    UD_TIPO_UINT16,
    UD_TIPO_SINT16,
    UD_TIPO_INT32,
    UD_TIPO_UINT32,
    UD_TIPO_SINT32,
    UD_TIPO_INT64,
    UD_TIPO_UINT64,
    UD_TIPO_SINT64,
};

/**
 * Comprueba si \a simbolo cumple con alguno de los tipos conocidos por Lily
 * @param L Sesión de Lua
 * @param tipo Tipo contra el cual comprobar
 * @param simbolo Símbolo a comprobar
 * @param [out] ctx Estado de la ejecución al momento de salir de la función
 * @return \c true si el símbolo coincide con el tipo, \c false si no
 */
static bool lily_lua_cpu_comp_tipo_simbolo(lua_State* L, char* tipo, struct lily_a_lexico_simbolo* simbolo, struct lily_error_ctx* ctx);

/**
 * Lee la lista de parámetros en \a params para una instrucción, y prepara sus valores en la pila para Lua
 * @param L Sesión de Lua
 * @param params Lista de parámetros para el mnemónico
 * @param [out] ctx Estado de la ejecución al momento de salir de la función
 */
static void lily_lua_cpu_est_parametros(lua_State* L, struct lily_lde_lde* params, struct lily_error_ctx* ctx);

/**
 * Obtiene el resultado del ensamble de una instrucción
 * @param L Sesión de Lua
 * @param [out] tam Número de bytes generados al ensamblar
 * @param [out] ctx Estado de la ejecución al momento de salir de la función
 * @return Cadena de bytes que representan al mnemónico
 */
static uint8_t* lily_lua_cpu_procesar_resultado(lua_State* L, lua_Integer* tam, struct lily_error_ctx* ctx);

/**
 * Analiza un script de descripción de un microprocesador y genera la estructura
 * @param codigo Cadena de caracteres que contiene el código del script
 * @param [out] ctx Estado de la ejecución al momento de salir de la función
 * @return Sesión de Lua nueva
 */
lua_State *lily_lua_cpu_cargar(const char *codigo, struct lily_error_ctx* ctx);

/**
 * Ensambla un mnemónico
 * @param L Sesión de Lua
 * @param instruccion Instrucción que contiene el mnemónico a convertir
 * @param [out] ctx Estado de la ejecución al momento de salir de la función, si no es \c NULL
 * @param [out] ctx Estado de la ejecución al momento de salir de la función
 */
void lily_lua_cpu_ensamblar(lua_State* L, struct lily_a_sintactico_instruccion* instruccion, struct lily_error_ctx* ctx);

#endif