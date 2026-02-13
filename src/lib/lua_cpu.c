#include "lua_cpu.h"

lua_State *lily_lua_cpu_cargar(const char *codigo, struct lily_lua_cpu_error_ctx *error_ctx) {
    // Cargar definición arquitectura a utilizar
    // FIX: por ahora, solo archivos de usuario
    lua_State* L = luaL_newstate();
    luaopen_base(L);
    luaopen_package(L);
    luaopen_string(L);
    luaopen_utf8(L);
    luaopen_table(L);
    luaopen_math(L);

    if (luaL_dostring(L, codigo) == LUA_OK) {
        if (!lua_istable(L, -1)) {
            if (error_ctx != NULL) error_ctx->codigo = COD_LUA_CPU_DESC_NO_TABLA;
            lua_close(L);
            return NULL;
        }

        // Analizar id
        lua_pushstring(L, "id");
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            if (error_ctx != NULL) error_ctx->codigo = COD_LUA_CPU_DESC_NO_ID;
            lua_close(L);
            return NULL;
        }
        if (!lua_isstring(L, -1)) {
            if (error_ctx != NULL) error_ctx->codigo = COD_LUA_CPU_DESC_ID_NO_CADENA;
            lua_close(L);
            return NULL;
        }
        const char* objeto_id = lua_tostring(L, -1);
        lua_pop(L, -1);

        // Analizar registros
        lua_pushstring(L, "registros");
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            if (error_ctx != NULL) error_ctx->codigo = COD_LUA_CPU_DESC_NO_REGISTROS;
            lua_close(L);
            return NULL;
        }
        if (!lua_istable(L, -1)) {
            if (error_ctx != NULL) error_ctx->codigo = COD_LUA_CPU_DESC_REGISTROS_NO_TABLA;
            lua_close(L);
            return NULL;
        }
        lua_pop(L, -1);

        // Analizar operandos
        lua_pushstring(L, "operandos");
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            if (error_ctx != NULL) error_ctx->codigo = COD_LUA_CPU_DESC_NO_OPERANDOS;
            lua_close(L);
            return NULL;
        }
        if (!lua_istable(L, -1)) {
            if (error_ctx != NULL) error_ctx->codigo = COD_LUA_CPU_DESC_OPERANDOS_NO_TABLA;
            lua_close(L);
            return NULL;
        }
        lua_pop(L, -1);

        // Analizar ensamble
        lua_pushstring(L, "ensamble");
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            if (error_ctx != NULL) error_ctx->codigo = COD_LUA_CPU_DESC_NO_ENSAMBLE;
            lua_close(L);
            return NULL;
        }
        if (!lua_istable(L, -1)) {
            if (error_ctx != NULL) error_ctx->codigo = COD_LUA_CPU_DESC_ENSAMBLE_NO_TABLA;
            lua_close(L);
            return NULL;
        }
        lua_pop(L, -1);

        // Analizar opcodes
        lua_pushstring(L, "opcodes");
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            if (error_ctx != NULL) error_ctx->codigo = COD_LUA_CPU_DESC_NO_OPCODES;
            lua_close(L);
            return NULL;
        }
        if (!lua_istable(L, -1)) {
            if (error_ctx != NULL) error_ctx->codigo = COD_LUA_CPU_DESC_OPCODES_NO_TABLA;
            lua_close(L);
            return NULL;
        }
        lua_pop(L, -1);

        // Analizar desensamble
        lua_pushstring(L, "desensamble");
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            if (error_ctx != NULL) error_ctx->codigo = COD_LUA_CPU_DESC_NO_DESENSAMBLE;
            lua_close(L);
            return NULL;
        }
        if (!lua_istable(L, -1)) {
            if (error_ctx != NULL) error_ctx->codigo = COD_LUA_CPU_DESC_DESENSAMBLE_NO_TABLA;
            lua_close(L);
            return NULL;
        }
        lua_pop(L, -1);
    } else {
        puts(lua_tostring(L, lua_gettop(L)));
        lua_pop(L, lua_gettop(L));
    }

    return L;
}