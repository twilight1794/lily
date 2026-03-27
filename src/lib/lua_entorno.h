/*
 * @file lua_entorno.h
 * Funciones para modificar el entorno de Lua
 */

#ifndef LILY_L_LUA_ENTORNO
#define LILY_L_LUA_ENTORNO

#include "../common/defs.h"
#include "../common/estado.h"
#include "../common/nums.h"
#include "lua_int.h"
#include "simbolo.h"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

// Modificaciones a biblioteca base
int lily_lua_entorno_base_assert(lua_State* L);
int lily_lua_entorno_base_error(lua_State* L);
int lily_lua_entorno_base_getmetatable(lua_State* L);
int lily_lua_entorno_base_ipairs_it(lua_State* L);
int lily_lua_entorno_base_ipairs(lua_State* L);
int lily_lua_entorno_base_next(lua_State* L);
int lily_lua_entorno_base_pairs_cont(lua_State* L, int status, lua_KContext k);
int lily_lua_entorno_base_pairs(lua_State* L);
int lily_lua_entorno_base_rawequal(lua_State* L);
int lily_lua_entorno_base_rawget(lua_State* L);
int lily_lua_entorno_base_rawlen(lua_State* L);
int lily_lua_entorno_base_rawset(lua_State* L);
int lily_lua_entorno_base_setmetatable(lua_State* L);
int lily_lua_entorno_base_tonumber(lua_State* L);
int lily_lua_entorno_base_tostring(lua_State* L);
int lily_lua_entorno_base_type(lua_State* L);

lua_State* lily_lua_entorno_preparar(enum lily_estado* estado);

#endif
