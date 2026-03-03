#include "lua_cpu.h"

lua_State *lily_lua_cpu_cargar(const char *codigo, struct lily_error_ctx* ctx) {
    // Cargar definición arquitectura a utilizar
    lua_State* L = luaL_newstate();
    // FIX: hay qué quitar algunas bibliotecas porque tienen funciones peligrosas, y reimplementar las que pudieran ser útiles:
    // Para base: assert, error, getmetatable, ipairs, next, pairs, print, setmetatable, tonumber, tostring, type, warn, _VERSION
    // Para package: analizar si deberíamos permitir cargar un archivo externo
    luaopen_base(L);
    luaopen_string(L);
    luaopen_utf8(L);
    luaopen_table(L);
    luaopen_math(L);

    if (luaL_dostring(L, codigo) == LUA_OK) {
        if (!lua_istable(L, -1)) {
            ctx->codigo = COD_LUA_CPU_DESC_NO_TABLA;
            lua_close(L);
            return NULL;
        }
        lua_pushinteger(L, lua_gettop(L));
        lua_setglobal(L, "_lily_tabla_idx");

        // Analizar id
        lua_pushstring(L, "id");
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            ctx->codigo = COD_LUA_CPU_DESC_NO_ID;
            lua_close(L);
            return NULL;
        }
        if (!lua_isstring(L, -1)) {
            ctx->codigo = COD_LUA_CPU_DESC_ID_NO_CADENA;
            lua_close(L);
            return NULL;
        }
        const char* objeto_id = lua_tostring(L, -1);
        lua_pop(L, 1);

        // Analizar registros
        lua_pushstring(L, "registros");
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            ctx->codigo = COD_LUA_CPU_DESC_NO_REGISTROS;
            lua_close(L);
            return NULL;
        }
        if (!lua_istable(L, -1)) {
            ctx->codigo = COD_LUA_CPU_DESC_REGISTROS_NO_TABLA;
            lua_close(L);
            return NULL;
        }
        lua_pop(L, 1);

        // Analizar tipos
        lua_pushstring(L, "tipos");
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            ctx->codigo = COD_LUA_CPU_DESC_NO_TIPOS;
            lua_close(L);
            return NULL;
        }
        if (!lua_istable(L, -1)) {
            ctx->codigo = COD_LUA_CPU_DESC_TIPOS_NO_TABLA;
            lua_close(L);
            return NULL;
        }
        lua_pop(L, 1);

        // Analizar ensamble
        lua_pushstring(L, "ensamble");
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            ctx->codigo = COD_LUA_CPU_DESC_NO_ENSAMBLE;
            lua_close(L);
            return NULL;
        }
        if (!lua_istable(L, -1)) {
            ctx->codigo = COD_LUA_CPU_DESC_ENSAMBLE_NO_TABLA;
            lua_close(L);
            return NULL;
        }
        lua_pop(L, 1);

        // Analizar opcodes
        lua_pushstring(L, "opcodes");
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            ctx->codigo = COD_LUA_CPU_DESC_NO_OPCODES;
            lua_close(L);
            return NULL;
        }
        if (!lua_istable(L, -1)) {
            ctx->codigo = COD_LUA_CPU_DESC_OPCODES_NO_TABLA;
            lua_close(L);
            return NULL;
        }
        lua_pop(L, 1);

        // Analizar desensamble
        lua_pushstring(L, "desensamble");
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            ctx->codigo = COD_LUA_CPU_DESC_NO_DESENSAMBLE;
            lua_close(L);
            return NULL;
        }
        if (!lua_istable(L, -1)) {
            ctx->codigo = COD_LUA_CPU_DESC_DESENSAMBLE_NO_TABLA;
            lua_close(L);
            return NULL;
        }
        lua_pop(L, 1);
    } else {
        ctx->codigo = COD_LUA_CPU_LUA_ERR;
        ctx->lua_msg = (char*) lua_tostring(L, -1);
        lua_close(L);
        return NULL;
    }

    // Cargar símbolos globales
    lua_createtable(L,0,18);
    /// Tipos comunes
    lua_pushlightuserdata(L, (void*) UD_TIPO_INT3);
    lua_setfield(L, -2, "int3");
    lua_pushlightuserdata(L, (void*) UD_TIPO_UINT3);
    lua_setfield(L, -2, "uint3");
    lua_pushlightuserdata(L, (void*) UD_TIPO_SINT3);
    lua_setfield(L, -2, "sint3");
    lua_pushlightuserdata(L, (void*) UD_TIPO_INT4);
    lua_setfield(L, -2, "int4");
    lua_pushlightuserdata(L, (void*) UD_TIPO_UINT4);
    lua_setfield(L, -2, "uint4");
    lua_pushlightuserdata(L, (void*) UD_TIPO_SINT4);
    lua_setfield(L, -2, "sint4");
    lua_pushlightuserdata(L, (void*) UD_TIPO_INT8);
    lua_setfield(L, -2, "int8");
    lua_pushlightuserdata(L, (void*) UD_TIPO_UINT8);
    lua_setfield(L, -2, "uint8");
    lua_pushlightuserdata(L, (void*) UD_TIPO_SINT8);
    lua_setfield(L, -2, "sint8");
    lua_pushlightuserdata(L, (void*) UD_TIPO_INT16);
    lua_setfield(L, -2, "int16");
    lua_pushlightuserdata(L, (void*) UD_TIPO_UINT16);
    lua_setfield(L, -2, "uint16");
    lua_pushlightuserdata(L, (void*) UD_TIPO_SINT16);
    lua_setfield(L, -2, "sint16");
    lua_pushlightuserdata(L, (void*) UD_TIPO_INT32);
    lua_setfield(L, -2, "int32");
    lua_pushlightuserdata(L, (void*) UD_TIPO_UINT32);
    lua_setfield(L, -2, "uint32");
    lua_pushlightuserdata(L, (void*) UD_TIPO_SINT32);
    lua_setfield(L, -2, "sint32");
    lua_pushlightuserdata(L, (void*) UD_TIPO_INT64);
    lua_setfield(L, -2, "int64");
    lua_pushlightuserdata(L, (void*) UD_TIPO_UINT64);
    lua_setfield(L, -2, "uint64");
    lua_pushlightuserdata(L, (void*) UD_TIPO_SINT64);
    lua_setfield(L, -2, "sint64");
    /// Funciones

    /// Juntar todo
    lua_setglobal(L, "L");

    return L;
}
