#include "lua_cpu.h"

static int lily_lua_cpu_base_assert(lua_State* L);
static int lily_lua_cpu_base_error(lua_State* L);
static int lily_lua_cpu_base_getmetatable(lua_State* L);
static int lily_lua_cpu_base_ipairs_it(lua_State* L);
static int lily_lua_cpu_base_ipairs(lua_State* L);
static int lily_lua_cpu_base_next(lua_State* L);
static int lily_lua_cpu_base_pairs_cont(lua_State* L, int status, lua_KContext k);
static int lily_lua_cpu_base_pairs(lua_State* L);
static int lily_lua_cpu_base_rawequal(lua_State* L);
static int lily_lua_cpu_base_rawget(lua_State* L);
static int lily_lua_cpu_base_rawlen(lua_State* L);
static int lily_lua_cpu_base_rawset(lua_State* L);
static int lily_lua_cpu_base_setmetatable(lua_State* L);
static int lily_lua_cpu_base_tonumber(lua_State* L);
static int lily_lua_cpu_base_tostring(lua_State* L);
static int lily_lua_cpu_base_type(lua_State* L);

static int lily_lua_cpu_base_assert(lua_State* L) {
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

static int lily_lua_cpu_base_error(lua_State* L) {
    if (lua_gettop(L) != 1) {
        lua_pushliteral(L, "error necesita 1 parámetro");
        lua_error(L);
    }
    return lua_error(L);
}

static int lily_lua_cpu_base_getmetatable(lua_State* L) {
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

static int lily_lua_cpu_base_ipairs_it(lua_State* L) {
    lua_pushinteger(L, 1);
    lua_arith(L, LUA_OPADD);
    lua_geti(L, 1, lua_tointeger(L, 2));
    return 2;
}

static int lily_lua_cpu_base_ipairs(lua_State* L) {
    if (lua_gettop(L) != 1) {
        lua_pushliteral(L, "ipairs necesita 1 parámetro");
        lua_error(L);
    }
    lua_pushcfunction(L, lily_lua_cpu_base_ipairs_it);
    lua_pushvalue(L, 1);
    lua_pushinteger(L, 0);
    return 3;
}

static int lily_lua_cpu_base_next(lua_State* L) {
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

static int lily_lua_cpu_base_pairs_cont(lua_State* L, int status, lua_KContext k) {
    // De lbaselib.c
    (void) L; (void) status; (void) k;
    return 3;
}

static int lily_lua_cpu_base_pairs(lua_State* L) {
    if (lua_gettop(L) != 1) {
        lua_pushliteral(L, "pairs necesita 1 parámetro");
        lua_error(L);
    }
    if (luaL_getmetafield(L, 1, "__pairs") == LUA_TNIL) {
        lua_pushcfunction(L, lily_lua_cpu_base_next);
        lua_pushvalue(L, 1);
        lua_pushnil(L);
    }
    else {
        // Hay un metamétodo _pairs
        lua_pushvalue(L, 1);
        lua_callk(L, 1, 3, 0, lily_lua_cpu_base_pairs_cont);
    }
    return 3;
}

static int lily_lua_cpu_base_rawequal(lua_State* L) {
    if (lua_gettop(L) != 2) {
        lua_pushliteral(L, "rawequal necesita 1 parámetro");
        lua_error(L);
    }
    lua_pushboolean(L, lua_rawequal(L, 1, 2));
    return 1;
}

static int lily_lua_cpu_base_rawget(lua_State* L) {
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

static int lily_lua_cpu_base_rawlen(lua_State* L) {
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

static int lily_lua_cpu_base_rawset(lua_State* L) {
    if (lua_gettop(L) != 3) {
        lua_pushliteral(L, "rawset necesita 3 parámetros");
        lua_error(L);
    }
    if (lua_type(L, 1) != LUA_TTABLE) {
        lua_pushliteral(L, "rawget requiere una tabla para operar");
        lua_error(L);
    }
    if (lua_isnoneornil(L, 2)) {
        lua_pushliteral(L, "rawget necesita un índice entero");
        lua_error(L);
    }
    lua_rawset(L, 1);
    return 1;
}

static int lily_lua_cpu_base_setmetatable(lua_State* L) {
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
        lua_pushliteral(L, "rawlen requiere una tabla o nil como metatabla");
        lua_error(L);
    }
    // Si ya hay un campo __metatable, error
    if (luaL_getmetafield(L, 1, "__metatable") != LUA_TNIL) {
        lua_pushliteral(L, "la tabla ya tiene una metatabla");
    }
    lua_setmetatable(L, 1);
    return 1;
}

static int lily_lua_cpu_base_tonumber(lua_State* L) {
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

static int lily_lua_cpu_base_tostring(lua_State* L) {
    if (lua_gettop(L) != 1) {
        lua_pushliteral(L, "tostring necesita 1 parámetro");
        lua_error(L);
    }
    lua_tolstring(L, 1, NULL);
    return 1;
}

static int lily_lua_cpu_base_type(lua_State* L) {
    int t = lua_type(L, 1);
    if (t == LUA_TNONE) {
        lua_pushliteral(L, "type necesita 1 parámetro");
        lua_error(L);
    }
    char* tcad = NULL;
    // Averiguar primero si se trata de nuestros tipos enteros
    if (t == LUA_TTABLE) {
        lua_pushstring(L, "__size");
        lua_gettable(L, 1);
        int t_tam = lua_type(L, 2);
        if (t_tam == LUA_TNUMBER) {
            int v_tam = lua_tointeger(L, 2);
            lua_pop(L, 1);
            lua_pushstring(L, "__signed");
            lua_gettable(L, 1);
            int t_signo = lua_type(L, 2);
            if (t_signo == LUA_TNUMBER) {
                int v_signo = lua_tointeger(L, 2);
                lua_pop(L, 1);
                if (v_tam == 1 && v_signo == 0) tcad = "int8";
                else if (v_tam == 1 && v_signo == 1) tcad = "uint8";
                else if (v_tam == 1 && v_signo == 2) tcad = "sint8";
                else if (v_tam == 2 && v_signo == 0) tcad = "int16";
                else if (v_tam == 2 && v_signo == 1) tcad = "uint16";
                else if (v_tam == 2 && v_signo == 2) tcad = "sint16";
                else if (v_tam == 4 && v_signo == 0) tcad = "int32";
                else if (v_tam == 4 && v_signo == 1) tcad = "uint32";
                else if (v_tam == 4 && v_signo == 2) tcad = "sint32";
                else if (v_tam == 8 && v_signo == 0) tcad = "int64";
                else if (v_tam == 8 && v_signo == 1) tcad = "uint64";
                else if (v_tam == 8 && v_signo == 2) tcad = "sint64";
            }
        }
    } // Si no, probablemente se trate de un tipo normal
    if (tcad == NULL) tcad = (char*) lua_typename(L, t);
    lua_pushstring(L, tcad);
    return 1;
}

lua_State* lily_lua_cpu_cargar(const char *codigo, struct lily_ctx* ctx) {
    // Cargar definición arquitectura a utilizar
    lua_State* L = luaL_newstate();
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
    }
    else {
        ctx->codigo = COD_LUA_CPU_LUA_ERR;
        ctx->lua_msg = (char*) lua_tostring(L, -1);
        lua_close(L);
        return NULL;
    }

    // Reemplazos y añadidos a basic
    lua_pushcfunction(L, lily_lua_cpu_base_assert);
    lua_setglobal(L, "assert");
    lua_pushcfunction(L, lily_lua_cpu_base_error);
    lua_setglobal(L, "error");
    lua_pushcfunction(L, lily_lua_cpu_base_getmetatable);
    lua_setglobal(L, "getmetatable");
    lua_pushcfunction(L, lily_lua_cpu_base_ipairs);
    lua_setglobal(L, "ipairs");
    lua_pushcfunction(L, lily_lua_cpu_base_next);
    lua_setglobal(L, "next");
    lua_pushcfunction(L, lily_lua_cpu_base_pairs);
    lua_setglobal(L, "pairs");
    lua_pushcfunction(L, lily_lua_cpu_base_rawequal);
    lua_setglobal(L, "rawequal");
    lua_pushcfunction(L, lily_lua_cpu_base_rawget);
    lua_setglobal(L, "rawget");
    lua_pushcfunction(L, lily_lua_cpu_base_rawlen);
    lua_setglobal(L, "rawlen");
    lua_pushcfunction(L, lily_lua_cpu_base_rawset);
    lua_setglobal(L, "rawset");
    lua_pushcfunction(L, lily_lua_cpu_base_setmetatable);
    lua_setglobal(L, "setmetatable");
    lua_pushcfunction(L, lily_lua_cpu_base_tonumber);
    lua_setglobal(L, "tonumber");
    lua_pushcfunction(L, lily_lua_cpu_base_tostring);
    lua_setglobal(L, "tostring");
    lua_pushcfunction(L, lily_lua_cpu_base_type);
    lua_setglobal(L, "type");
    lua_pushstring(L, "Lua 5.4");
    lua_setglobal(L, "_VERSION");
    char* _lily_version = (char*) malloc(snprintf(NULL, 0, "Lily %s (commit %s%s)", LILY_VERSION, LILY_COMMIT, LILY_MODIFICADO));
    if (_lily_version == NULL) {
        ctx->codigo = COD_MALLOC_FALLO;
        lua_close(L);
        return NULL;
    }
    sprintf(_lily_version, "Lily %s (commit %s%s)", LILY_VERSION, LILY_COMMIT, LILY_MODIFICADO);
    lua_pushstring(L, _lily_version);
    lua_setglobal(L, "LUA_VERSION");
    free(_lily_version);

    lua_createtable(L, 0, 0);

    /// Juntar todo
    lua_setglobal(L, "L");

    return L;
}
