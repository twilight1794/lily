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
