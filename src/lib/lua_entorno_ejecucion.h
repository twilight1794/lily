/*
 * @file lua_entorno_ejecucion.h
 * Funciones para modificar el entorno de Lua específicas para la etapa de ejecución
 */

#ifndef LILY_L_LUA_ENTORNO_EJECUCION
#define LILY_L_LUA_ENTORNO_EJECUCION

#include <math.h>

#include "mensajes.h"
#include "../common/bitarray.h"
#include "../common/defs.h"
#include "../common/estado.h"

#include <lua.h>
#include <lualib.h>

#define lua_registertable(L, n, f) \
    (lua_pushcfunction(L, f), lua_setfield(L, -2, n))

int lily_lua_entorno_ejecucion_leer_memoria(lua_State* L);
int lily_lua_entorno_ejecucion_escribir_memoria(lua_State* L);
int lily_lua_entorno_ejecucion_leer_registro(lua_State* L);
int lily_lua_entorno_ejecucion_escribir_registro(lua_State* L);

void lily_lua_entorno_ejecucion_preparar(lua_State* L, enum lily_estado* estado);

#endif
