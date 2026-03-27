#include "lua_entorno.h"

int lily_lua_entorno_base_assert(lua_State* L) {
    int n = lua_gettop(L);
    if (n < 1 || n > 2) {
        // FIX: establecer acá un buen sistema de errores
        lua_pushliteral(L, "assert necesita 1 o 2 parámetros");
        return lua_error(L);
    }
    // Comprobaciones
    if (lua_toboolean(L, 1) == 0) {
        lua_remove(L, 1);
        if (n == 1)
            lua_pushliteral(L, "la condición assert falló");
        lua_error(L);
    }
    return 2;
}

int lily_lua_entorno_base_error(lua_State* L) {
    if (lua_gettop(L) != 1) {
        lua_pushliteral(L, "error necesita 1 parámetro");
        lua_error(L);
    }
    return lua_error(L);
}

int lily_lua_entorno_base_getmetatable(lua_State* L) {
    if (lua_gettop(L) != 1) {
        lua_pushliteral(L, "getmetatable necesita 1 parámetro");
        lua_error(L);
    }
    if (!lua_getmetatable(L, 1)) {
        lua_pushnil(L);
        return 1;
    }
    luaL_getmetafield(L, 1, "__metatable");
    return 1;
}

int lily_lua_entorno_base_ipairs_it(lua_State* L) {
    lua_pushinteger(L, 1);
    lua_arith(L, LUA_OPADD);
    lua_geti(L, 1, lua_tointeger(L, 2));
    return 2;
}

int lily_lua_entorno_base_ipairs(lua_State* L) {
    if (lua_gettop(L) != 1) {
        lua_pushliteral(L, "ipairs necesita 1 parámetro");
        lua_error(L);
    }
    lua_pushcfunction(L, lily_lua_entorno_base_ipairs_it);
    lua_pushvalue(L, 1);
    lua_pushinteger(L, 0);
    return 3;
}

int lily_lua_entorno_base_next(lua_State* L) {
    int n = lua_gettop(L);
    if (n < 1 || n > 2) {
        lua_pushliteral(L, "next necesita 1 o 2 parámetros");
        lua_error(L);
    }
    lua_settop(L, 2);
    if (lua_next(L, 1)) return 2;
    else {
        lua_pushnil(L);
        return 1;
    }
}

int lily_lua_entorno_base_pairs_cont(lua_State* L, int status, lua_KContext k) {
    // De lbaselib.c
    (void) L; (void) status; (void) k;
    return 3;
}

int lily_lua_entorno_base_pairs(lua_State* L) {
    if (lua_gettop(L) != 1) {
        lua_pushliteral(L, "pairs necesita 1 parámetro");
        lua_error(L);
    }
    if (luaL_getmetafield(L, 1, "__pairs") == LUA_TNIL) {
        lua_pushcfunction(L, lily_lua_entorno_base_next);
        lua_pushvalue(L, 1);
        lua_pushnil(L);
    }
    else {
        // Hay un metamétodo _pairs
        lua_pushvalue(L, 1);
        lua_callk(L, 1, 3, 0, lily_lua_entorno_base_pairs_cont);
    }
    return 3;
}

int lily_lua_entorno_base_rawequal(lua_State* L) {
    if (lua_gettop(L) != 2) {
        lua_pushliteral(L, "rawequal necesita 1 parámetro");
        lua_error(L);
    }
    lua_pushboolean(L, lua_rawequal(L, 1, 2));
    return 1;
}

int lily_lua_entorno_base_rawget(lua_State* L) {
    if (lua_gettop(L) != 2) {
        lua_pushliteral(L, "rawget necesita 2 parámetros");
        lua_error(L);
    }
    if (lua_type(L, 1) != LUA_TTABLE) {
        lua_pushliteral(L, "rawget requiere una tabla para operar");
        lua_error(L);
    }
    lua_rawget(L, 1);
    return 1;
}

int lily_lua_entorno_base_rawlen(lua_State* L) {
    if (lua_gettop(L) != 1) {
        lua_pushliteral(L, "rawlen necesita 1 parámetro");
        lua_error(L);
    }
    int t = lua_type(L, 1);
    if (t != LUA_TTABLE && t != LUA_TSTRING) {
        lua_pushliteral(L, "rawlen requiere una tabla o cadena");
        lua_error(L);
    }
    lua_pushinteger(L, lua_rawlen(L, 1));
    return 1;
}

int lily_lua_entorno_base_rawset(lua_State* L) {
    if (lua_gettop(L) != 3) {
        lua_pushliteral(L, "rawset necesita 3 parámetros");
        lua_error(L);
    }
    if (lua_type(L, 1) != LUA_TTABLE) {
        lua_pushliteral(L, "rawset requiere una tabla para operar");
        lua_error(L);
    }
    if (lua_isnoneornil(L, 2)) {
        lua_pushliteral(L, "rawset necesita un índice entero");
        lua_error(L);
    }
    lua_rawset(L, 1);
    return 1;
}

int lily_lua_entorno_base_setmetatable(lua_State* L) {
    if (lua_gettop(L) != 2) {
        lua_pushliteral(L, "setmetatable necesita 2 parámetros");
        lua_error(L);
    }
    if (lua_type(L, 1) != LUA_TTABLE) {
        lua_pushliteral(L, "setmetatable requiere una tabla para operar");
        lua_error(L);
    }
    int t = lua_type(L, 2);
    if (t != LUA_TTABLE && t != LUA_TNIL) {
        lua_pushliteral(L, "setmetatable requiere una tabla o nil como metatabla");
        lua_error(L);
    }
    // Si ya hay un campo __metatable, error
    if (luaL_getmetafield(L, 1, "__metatable") != LUA_TNIL) {
        lua_pushliteral(L, "la tabla ya tiene una metatabla");
    }
    lua_setmetatable(L, 1);
    return 1;
}

int lily_lua_entorno_base_tonumber(lua_State* L) {
    // FIX: admitir parámetro para base
    if (lua_gettop(L) != 1) {
        lua_pushliteral(L, "tonumber necesita 1 parámetro");
        lua_error(L);
    }
    if (lua_type(L, 1) == LUA_TNUMBER) return 1;
    else {
        size_t tam;
        const char *val = lua_tolstring(L, 1, &tam);
        if (val == NULL || lua_stringtonumber(L, val) != tam + 1)
            lua_pushnil(L);
    }
    return 1;
}

int lily_lua_entorno_base_tostring(lua_State* L) {
    if (lua_gettop(L) != 1) {
        lua_pushliteral(L, "tostring necesita 1 parámetro");
        lua_error(L);
    }
    lua_tolstring(L, 1, NULL);
    return 1;
}

int lily_lua_entorno_base_type(lua_State* L) {
    int t = lua_type(L, 1);
    if (t == LUA_TNONE) {
        lua_pushliteral(L, "type necesita 1 parámetro");
        lua_error(L);
    }
    enum lily_lua_tipo tipo = lily_lua_int_obj(L);
    char* cad = lily_lua_int_tipo_cad(tipo); // TODO: comprobar si de verdad nunca devolveremos nil acá
    lua_pushstring(L, cad);
    return 1;
}

lua_State* lily_lua_entorno_preparar(enum lily_estado* estado) {
    lua_State* L = luaL_newstate();
    // Bibliotecas integradas
    luaopen_string(L);
    luaopen_utf8(L);
    luaopen_table(L);
    luaopen_math(L);

    lua_register(L, "assert", lily_lua_entorno_base_assert);
    lua_register(L, "error", lily_lua_entorno_base_error);
    lua_register(L, "getmetatable", lily_lua_entorno_base_getmetatable);
    lua_register(L, "ipairs", lily_lua_entorno_base_ipairs);
    lua_register(L, "next", lily_lua_entorno_base_next);
    lua_register(L, "pairs", lily_lua_entorno_base_pairs);
    lua_register(L, "rawequal", lily_lua_entorno_base_rawequal);
    lua_register(L, "rawget", lily_lua_entorno_base_rawget);
    lua_register(L, "rawlen", lily_lua_entorno_base_rawlen);
    lua_register(L, "rawset", lily_lua_entorno_base_rawset);
    lua_register(L, "setmetatable", lily_lua_entorno_base_setmetatable);
    lua_register(L, "tonumber", lily_lua_entorno_base_tonumber);
    lua_register(L, "tostring", lily_lua_entorno_base_tostring);
    lua_register(L, "type", lily_lua_entorno_base_type);
    lua_pushstring(L, "Lua 5.4");
    lua_setglobal(L, "_VERSION");
    char* _lily_version = (char*) malloc(snprintf(NULL, 0, "Lily %s (commit %s%s)", LILY_VERSION, LILY_COMMIT, LILY_MODIFICADO));
    if (_lily_version == NULL) {
        *estado = COD_MALLOC_FALLO;
        lua_close(L);
        return NULL;
    }
    sprintf(_lily_version, "Lily %s (commit %s%s)", LILY_VERSION, LILY_COMMIT, LILY_MODIFICADO);
    lua_pushstring(L, _lily_version);
    lua_setglobal(L, "LILY_VERSION");
    free(_lily_version);
    return L;
}
