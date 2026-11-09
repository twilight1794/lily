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

#include "lua_cpu.h"

void lily_lua_cpu_copiar_msg(lua_State* L, char** destino) {
    free(*destino);
    *destino = NULL;
    size_t tam;
    const char* msg = lua_tolstring(L, -1, &tam);
    if (msg == NULL) return;
    *destino = (char*) malloc(tam + 1);
    if (*destino != NULL) memcpy(*destino, msg, tam + 1);
}

void lily_lua_cpu_cargar(lua_State* L, const char* codigo, enum lily_estado* estado, char** lua_msg) {
    // Cargar definición arquitectura a utilizar
    if (luaL_dostring(L, codigo) == LUA_OK) {
        if (!lua_istable(L, -1)) {
            *estado = COD_LUA_CPU_DESC_NO_TABLA;
            lua_close(L);
            return;
        }
        lua_pushinteger(L, lua_gettop(L));
        lua_setglobal(L, "_lily_tabla_idx");
        lua_setglobal(L, "_lily_esquema");
        lua_getglobal(L, "_lily_esquema");

        // Analizar id
        lua_pushstring(L, "id");
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            *estado = COD_LUA_CPU_DESC_NO_ID;
            lua_close(L);
            return;
        }
        if (!lua_isstring(L, -1)) {
            *estado = COD_LUA_CPU_DESC_ID_NO_CADENA;
            lua_close(L);
            return;
        }
        const char* objeto_id = lua_tostring(L, -1);
        lua_pop(L, 1);

        // Analizar registros
        lua_pushstring(L, "registros");
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            *estado = COD_LUA_CPU_DESC_NO_REGISTROS;
            lua_close(L);
            return;
        }
        if (!lua_istable(L, -1)) {
            *estado = COD_LUA_CPU_DESC_REGISTROS_NO_TABLA;
            lua_close(L);
            return;
        }
        lua_pop(L, 1);

        // Analizar tipos
        lua_pushstring(L, "tipos");
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            *estado = COD_LUA_CPU_DESC_NO_TIPOS;
            lua_close(L);
            return;
        }
        if (!lua_istable(L, -1)) {
            *estado = COD_LUA_CPU_DESC_TIPOS_NO_TABLA;
            lua_close(L);
            return;
        }
        lua_pop(L, 1);

        // Analizar ensamble
        lua_pushstring(L, "ensamble");
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            *estado = COD_LUA_CPU_DESC_NO_ENSAMBLE;
            lua_close(L);
            return;
        }
        if (!lua_istable(L, -1)) {
            *estado = COD_LUA_CPU_DESC_ENSAMBLE_NO_TABLA;
            lua_close(L);
            return;
        }
        lua_pop(L, 1);

        // Analizar opcodes
        lua_pushstring(L, "opcodes");
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            *estado = COD_LUA_CPU_DESC_NO_OPCODES;
            lua_close(L);
            return;
        }
        if (!lua_istable(L, -1)) {
            *estado = COD_LUA_CPU_DESC_OPCODES_NO_TABLA;
            lua_close(L);
            return;
        }
        lua_pop(L, 1);

        // Analizar desensamble
        lua_pushstring(L, "desensamble");
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            *estado = COD_LUA_CPU_DESC_NO_DESENSAMBLE;
            lua_close(L);
            return;
        }
        if (!lua_istable(L, -1)) {
            *estado = COD_LUA_CPU_DESC_DESENSAMBLE_NO_TABLA;
            lua_close(L);
            return;
        }
        lua_pop(L, 1);
    }
    else {
        *estado = COD_LUA_EXC;
        lily_lua_cpu_copiar_msg(L, lua_msg); // Antes de cerrar la sesión, que libera el mensaje
        lua_close(L);
        return;
    }
}
