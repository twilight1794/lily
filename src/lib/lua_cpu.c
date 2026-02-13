#include "lua_cpu.h"

lua_State *lily_lua_cpu_cargar(const char *codigo, struct lily_lua_cpu_error_ctx *error_ctx) {
    // Cargar definición arquitectura a utilizar
    lua_State* L = luaL_newstate();
    // FIX: hay qué quitar algunas bibliotecas porque tienen funciones peligrosas, y reimplementar las que pudieran ser útiles:
    // Para base: assert, error, getmetatable, ipairs, next, pairs, print, setmetatable, tonumber, tostring, type
    // Para package: analizar si deberíamos permitir cargar un archivo externo
    luaopen_base(L);
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
        lua_pop(L, 1);

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
        lua_pop(L, 1);

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
        lua_pop(L, 1);

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
        lua_pop(L, 1);

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
        lua_pop(L, 1);

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
        lua_pop(L, 1);
    } else {
        puts(lua_tostring(L, lua_gettop(L)));
        lua_pop(L, lua_gettop(L));
    }

    return L;
}

char* lily_lua_cpu_ensamblar(lua_State* L, const char* mnemo, void** params, struct lily_lua_cpu_error_ctx *error_ctx) {
    //char* bytes = lily_cadena_create();
    lua_pushstring(L, "ensamble");
    lua_gettable(L, -2);
    /*lua_pushstring(L, mnemo);
    lua_gettable(L, -2); // No hay validación: en este punto, todos los mnemónicos deben existir
    // Determinar tipo de mnemónico
    if (lua_isfunction(L, -1)) {
        printf("%s es de tipo función\n", mnemo);
        // Atajo: función
        // ...
    } else {
        // En este punto puede ser: List<int>, Tuple<char*, function>, List<Tuple<Params, Value>>
        int tipo_tentativo = lua_geti(L, -1, 1);
        if (tipo_tentativo == LUA_TNUMBER) printf("%s es de tipo List<int>\n", mnemo);
        else if (tipo_tentativo == LUA_TSTRING) printf("%s es de tipo Tuple<char*, funcion>\n", mnemo);
        else if (tipo_tentativo == LUA_TTABLE) printf("%s es de tipo List<Tuple<Params, Value>>\n", mnemo);
    }
    lua_pop(L, 3);*/

    lua_pushnil(L);
    while (lua_next(L, -2) != 0) {
        if (lua_type(L, -2) != LUA_TSTRING) {
            puts("Esto pasa?");
            lua_pop(L, 1); /* pop the value */
            continue;
        }
        char* elmnemo = lua_tostring(L, -2);
        if (lua_isfunction(L, -1)) {
            printf("%s es de tipo función\n", elmnemo);
            // Atajo: función
            // ...
        } else {
            // En este punto puede ser: List<int>, Tuple<char*, function>, List<Tuple<Params, Value>>
            int tipo_tentativo = lua_geti(L, -1, 1);
            if (tipo_tentativo == LUA_TNUMBER) printf("%s es de tipo List<int>\n", elmnemo);
            else if (tipo_tentativo == LUA_TSTRING) printf("%s es de tipo Tuple<char*, funcion>\n", elmnemo);
            else if (tipo_tentativo == LUA_TTABLE) printf("%s es de tipo List<Tuple<Params, Value>>\n", elmnemo);
        }
        lua_pop(L, 2);
    }

    return NULL;
    //return bytes;
}
