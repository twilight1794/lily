/*
 * @file lua_cpu.h
 * Funciones para cargar esquemas de descripción de microprocesadores para Lily
 */
 
 #ifndef LILY_L_LUA_CPU
 #define LILY_L_LUA_CPU

#include "../common/defs.h"
#include "../common/error.h"
#include "../common/log.h"
#include  "a_lexico_simbolo.h"

#include <libintl.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define ptipo(pre, idx) printf("%s: %s\n", (pre), lua_typename(L, lua_type(L, idx)));

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
 * Analiza un script de descripción de un microprocesador y genera la estructura
 * @param codigo Cadena de caracteres que contiene el código del script
 * @param [out] ctx Estado de la ejecución al momento de salir de la función
 * @return Sesión de Lua nueva
 */
lua_State *lily_lua_cpu_cargar(const char *codigo, struct lily_error_ctx* ctx);

#endif