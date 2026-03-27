#include "lua_cpu.h"

void lily_lua_cpu_cargar(lua_State* L, const char* codigo, enum lily_estado* estado, void** ctx) {
    // Cargar definición arquitectura a utilizar
    if (luaL_dostring(L, codigo) == LUA_OK) {
        if (!lua_istable(L, -1)) {
            *estado = COD_LUA_CPU_DESC_NO_TABLA;
            lua_close(L);
            return;
        }
        lua_pushinteger(L, lua_gettop(L));
        lua_setglobal(L, "_lily_tabla_idx");

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
        *estado = COD_LUA_CPU_LUA_ERR;
        ((struct lily_a_semantico_ctx*) (*ctx))->lua_msg = (char*) lua_tostring(L, -1);
        lua_close(L);
        return;
    }
}
