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
 * @file lua_cpu.h
 * Funciones para cargar esquemas de descripción de microprocesadores para Lily
 */

#ifndef LILY_L_LUA_CPU
#define LILY_L_LUA_CPU

#include "../common/defs.h"
#include "../common/estado.h"
#include "../common/log.h"
#include "a_semantico.h"
#include "simbolo.h"

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
void lily_lua_cpu_cargar(lua_State* L, const char* codigo, enum lily_estado* estado, void** ctx);

#endif
