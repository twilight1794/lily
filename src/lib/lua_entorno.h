/*
 * Copyright (C) 2026 Giovanni Alfredo Garciliano Diaz
 *
 * This file is part of Lily.
 *
 * Lily is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Lily is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Lily. If not, see <https://www.gnu.org/licenses/>.
 */

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

lua_State* lily_lua_entorno_preparar(enum lily_estado* estado, f_mensajes_ptr fun_mensaje);

#endif
