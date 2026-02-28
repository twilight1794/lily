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
};

/**
 * Estructura para almacenar detalles sobre el estado de la ejecución
 */
struct lily_lua_cpu_error_ctx {
    enum lily_error codigo; //< Código del error generado
};

/**
 * Comprueba si \a simbolo cumple con alguno de los tipos predeterminados de Lily
 * @param tipo Tipo contra el cual comprobar
 * @param simbolo Símbolo a comprobar
 * @return \c true si el símbolo coincide con el tipo, \c false si no
 */
static bool lily_lua_cpu_comp_tipo_simbolo_(char* tipo, struct lily_a_lexico_simbolo* simbolo);

/**
 * Lee la lista de parámetros en \a params para una instrucción, y prepara sus valores en la pila para Lua
 * @param L Sesión de Lua
 * @param params Lista de parámetros para el mnemónico
 * @return Estado de la ejecución
 */
struct lily_lua_cpu_error_ctx lily_lua_cpu_est_parametros(lua_State* L, struct lily_lde_lde* params);

/**
 * Analiza un script de descripción de un microprocesador y genera la estructura
 * @param codigo Cadena de caracteres que contiene el código del script
 * @param [out] error_ctx Estado de la ejecución al momento de salir de la función, si no es \c NULL
 * @return Sesión de Lua nueva
 */
lua_State *lily_lua_cpu_cargar(const char *codigo, struct lily_lua_cpu_error_ctx* error_ctx);

/**
 *
 * @param L Sesión de Lua
 * @param mnemo Mnemónico a convertir
 * @param params Lista de parámetros para el mnemónico
 * @param [out] error_ctx Estado de la ejecución al momento de salir de la función, si no es \c NULL
 * @return Lista de bytes que representan la conversión a código objeto del mnemónico
 */
char* lily_lua_cpu_ensamblar(lua_State* L, const char* mnemo, struct lily_lde_lde* params, struct lily_lua_cpu_error_ctx* error_ctx);

#endif