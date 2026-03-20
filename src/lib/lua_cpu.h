/*
 * @file lua_cpu.h
 * Funciones para cargar esquemas de descripción de microprocesadores para Lily
 */

#ifndef LILY_L_LUA_CPU
#define LILY_L_LUA_CPU

#include "../common/defs.h"
#include "../common/estado.h"
#include "../common/log.h"
#include  "simbolo.h"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define ptipo(pre, idx) printf("%s: %s\n", (pre), lua_typename(L, lua_type(L, idx)));

/**
 * Analiza un script de descripción de un microprocesador
 * @param L Sesión de Lua
 * @param codigo Cadena de caracteres que contiene el código del script
 * @param [out] ctx Estado de la ejecución al momento de salir de la función
 */
void lily_lua_cpu_cargar(lua_State* L, const char* codigo, struct lily_ctx* ctx);

#endif
