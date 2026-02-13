/*
 * @file lua_cpu.h
 * Funciones para cargar archivos de descripción de microprocesadores para Lily
 */
 
 #ifndef LILY_L_LUA_CPU
 #define LILY_L_LUA_CPU

#include "../common/cadena.h"
#include "../common/defs.h"
#include "../common/error.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

/**
 * Estructura para almacenar detalles sobre el estado de la ejecución
 */
struct lily_lua_cpu_error_ctx {
    enum lily_error codigo; //< Código del error generado
};

/**
 * Analiza un script de descripción de un microprocesador y genera la estructura
 * @param codigo Cadena de caracteres que contiene el código del script
 * @param [out] error_ctx Estado de la ejecución al momento de salir de la función, si no es \c NULL
 * @return Sesión de Lua nueva
 */
lua_State *lily_lua_cpu_cargar(const char *codigo, struct lily_lua_cpu_error_ctx *error_ctx);

/**
 *
 * @param L Sesión de Lua
 * @param mnemo Mnemónico a convertir
 * @param params Lista de parámetros para el mnemónico
 * @param [out] error_ctx Estado de la ejecución al momento de salir de la función, si no es \c NULL
 * @return Lista de bytes que representan la conversión a código objeto del mnemónico
 */
char* lily_lua_cpu_ensamblar(lua_State* L, const char* mnemo, void** params, struct lily_lua_cpu_error_ctx *error_ctx);

#endif