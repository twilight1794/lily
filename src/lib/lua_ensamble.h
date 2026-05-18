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
 * @file lua_ensamble.h
 * Funciones para ensamblar según un esquema de descripción de microprocesador
 */

#ifndef LILY_L_LUA_ENSAMBLE
#define LILY_L_LUA_ENSAMBLE

#include <ctype.h>

#include "../common/cadena.h"
#include "../common/defs.h"
#include "../common/estado.h"
#include "../common/lde.h"
#include "../common/log.h"
#include "a_semantico.h"
#include "lua_int.h"
#include "simbolo.h"

#include <lua.h>

/**
 * Ensambla un mnemónico
 * @param L Sesión de Lua
 * @param instruccion Instrucción que contiene el mnemónico a ensamblar
 * @param [out] ctx Estado de la ejecución al momento de salir de la función
 */
void lily_lua_cpu_ensamblar(lua_State* L, struct lily_simbolo_instruccion* instruccion, f_mensajes_ptr enviar_mensaje, enum lily_estado* estado, void** ctx);

#endif
