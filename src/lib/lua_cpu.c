#include "lua_cpu.h"

#include "../common/log.h"

struct lily_lua_cpu_error_ctx lily_lua_cpu_est_parametros(lua_State* L, struct lily_lde_lde* params) {
    struct lily_lua_cpu_error_ctx ctx = {
        .codigo = COD_OK,
    };
    size_t tam = lily_lde_size(params);
    for (size_t i = 0; i < tam; i++) {
        struct lily_lde_nodo* nodo = lily_lde_get(params, i);
        struct lily_a_lexico_simbolo* simbolo = nodo->valor;
        if (simbolo->tipo == SIMB_OBJETO) {
            lua_pushstring(L, (char*) simbolo->valor);
            lua_seti(L, -2, i + 1);
        }
        else if (simbolo->tipo == SIMB_NUMERO) {
            // FIX: por ahora, un entero
            lua_pushinteger(L, *((long*) simbolo->valor));
            lua_seti(L, -2, i + 1);
        }
        else if (simbolo->tipo == SIMB_DESPLAZAMIENTO_AP) {
            size_t tam_desp = lily_lde_size((struct lily_lde_lde*) simbolo->valor);
            lua_createtable(L, tam_desp, 0);
            for (size_t j = 0; j < tam_desp; j++) {
                struct lily_lde_nodo* nodo_desp = lily_lde_get(simbolo->valor, j);
                struct lily_a_lexico_simbolo* simbolo_desp = nodo_desp->valor;
                if (simbolo_desp->tipo == SIMB_OBJETO) {
                    lua_pushstring(L, (char*) simbolo_desp->valor);
                    lua_seti(L, -2, j + 1);
                } else if (simbolo_desp->tipo == SIMB_NUMERO) {
                    // FIX: por ahora, un entero
                    lua_pushinteger(L, *((long*) simbolo_desp->valor));
                    lua_seti(L, -2, j + 1);
                } else {
                    // FIX: quitar este mensaje de log, es una condición que debería ser imposible
                    log_warn_gen("El preprocesado se olvidó %s", lily_a_lexico_simbolo_print(simbolo_desp));
                    return ctx;
                }
            }
            lua_seti(L, -2, i + 1);
        }
        else {
            // FIX: quitar este mensaje de log, es una condición que debería ser imposible
            log_warn_gen("El preprocesado se olvidó %s", lily_a_lexico_simbolo_print(simbolo));
            return ctx;
        }
    }
    return ctx;
}

lua_State *lily_lua_cpu_cargar(const char *codigo, struct lily_lua_cpu_error_ctx* error_ctx) {
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

    // Cargar símbolos globales
    lua_createtable(L,0,5);
    /// Tipos comunes
    lua_pushlightuserdata(L, (void*) UD_TIPO_INT3);
    lua_setfield(L, -2, "t_int3");
    lua_pushlightuserdata(L, (void*) UD_TIPO_INT4);
    lua_setfield(L, -2, "t_int4");
    lua_pushlightuserdata(L, (void*) UD_TIPO_INT8);
    lua_setfield(L, -2, "t_int8");
    lua_pushlightuserdata(L, (void*) UD_TIPO_INT16);
    lua_setfield(L, -2, "t_int16");
    lua_pushlightuserdata(L, (void*) UD_TIPO_INT32);
    lua_setfield(L, -2, "t_int32");
    /// Funciones

    /// Juntar todo
    lua_setglobal(L, "L");

    return L;
}

char* lily_lua_cpu_ensamblar(lua_State* L, const char* mnemo, struct lily_lde_lde* params, struct lily_lua_cpu_error_ctx* error_ctx) {
    char* bytes = lily_cadena_create();
    lua_pushstring(L, "ensamble");
    lua_gettable(L, -2);
    lua_pushstring(L, mnemo);
    lua_gettable(L, -2); // En este punto, todos los mnemónicos deben existir, pues ya se ha validado
    // Determinar tipo de mnemónico
    if (lua_isfunction(L, -1)) {
        printf("%s es de tipo función\n", mnemo);
        *error_ctx = lily_lua_cpu_est_parametros(L, params); // FIX: ver si vale la pena manejar un error aquí
        if (lua_pcall(L, 0, 1, 0) == LUA_OK) {
            // Trabajar con la devolución
            lua_pop(L, lua_gettop(L));
        }
    } else {
        // En este punto puede ser: List<int>, Tuple<char*, function>, List<Tuple<Params, Value>>
        int tipo_tentativo = lua_geti(L, -1, 1);
        if (tipo_tentativo == LUA_TNUMBER) {
            printf("%s es de tipo List<int>\n", mnemo);
        }
        else if (tipo_tentativo == LUA_TSTRING) {
            printf("%s es de tipo Tuple<char*, funcion>\n", mnemo);
        }
        else if (tipo_tentativo == LUA_TTABLE) {
            printf("%s es de tipo List<Tuple<Params, Value>>\n", mnemo);
        }
    }
    lua_pop(L, 3);

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
