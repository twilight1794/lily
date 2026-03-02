#include "lua_cpu.h"

static bool lily_lua_cpu_comp_tipo_simbolo(lua_State* L, char* tipo, struct lily_a_lexico_simbolo* simbolo, struct lily_error_ctx* ctx) {
    union lily_a_lexico_numero n;
    ctx->codigo = COD_OK;
    // Primero, comprobar si el tipo es uno precargado
    if (!strcmp(tipo, "int3") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) {
            n.negativo = ((union lily_a_lexico_numero*) simbolo->valor)->negativo;
            return n.negativo >= -4;
        }
        n.positivo = ((union lily_a_lexico_numero*) simbolo->valor)->positivo;
        return n.positivo <= 7;
    }
    if (!strcmp(tipo, "uint3") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) return false;
        n.positivo = ((union lily_a_lexico_numero*) simbolo->valor)->positivo;
        return n.positivo <= 7;
    }
    if (!strcmp(tipo, "sint3") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) {
            n.negativo = ((union lily_a_lexico_numero*) simbolo->valor)->negativo;
            return n.negativo >= -4;
        }
        n.positivo = ((union lily_a_lexico_numero*) simbolo->valor)->positivo;
        return n.positivo <= 3;
    }
    if (!strcmp(tipo, "int4") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) {
            n.negativo = ((union lily_a_lexico_numero*) simbolo->valor)->negativo;
            return n.negativo >= -8;
        }
        n.positivo = ((union lily_a_lexico_numero*) simbolo->valor)->positivo;
        return n.positivo <= 15;
    }
    if (!strcmp(tipo, "uint4") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) return false;
        n.positivo = ((union lily_a_lexico_numero*) simbolo->valor)->positivo;
        return n.positivo <= 15;
    }
    if (!strcmp(tipo, "sint4") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) {
            n.negativo = ((union lily_a_lexico_numero*) simbolo->valor)->negativo;
            return n.negativo >= -8;
        }
        n.positivo = ((union lily_a_lexico_numero*) simbolo->valor)->positivo;
        return n.positivo <= 7 ;
    }
    if (!strcmp(tipo, "int8") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) {
            n.negativo = ((union lily_a_lexico_numero*) simbolo->valor)->negativo;
            return n.negativo >= -128;
        }
        n.positivo = ((union lily_a_lexico_numero*) simbolo->valor)->positivo;
        return n.positivo <= 255;
    }
    if (!strcmp(tipo, "uint8") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) return false;
        n.positivo = ((union lily_a_lexico_numero*) simbolo->valor)->positivo;
        return n.positivo <= 255;
    }
    if (!strcmp(tipo, "sint8") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) {
            n.negativo = ((union lily_a_lexico_numero*) simbolo->valor)->negativo;
            return n.negativo >= -128;
        }
        n.positivo = ((union lily_a_lexico_numero*) simbolo->valor)->positivo;
        return n.positivo <= 127;
    }
    if (!strcmp(tipo, "int16") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) {
            n.negativo = ((union lily_a_lexico_numero*) simbolo->valor)->negativo;
            return n.negativo >= -32768;
        }
        n.positivo = ((union lily_a_lexico_numero*) simbolo->valor)->positivo;
        return n.positivo <= 65535;
    }
    if (!strcmp(tipo, "uint16") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) return false;
        n.positivo = ((union lily_a_lexico_numero*) simbolo->valor)->positivo;
        return n.positivo <= 65535;
    }
    if (!strcmp(tipo, "sint16") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) {
            n.negativo = ((union lily_a_lexico_numero*) simbolo->valor)->negativo;
            return n.negativo >= -32768;
        }
        n.positivo = ((union lily_a_lexico_numero*) simbolo->valor)->positivo;
        return n.positivo <= 32767;
    }
    if (!strcmp(tipo, "int32") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) {
            n.negativo = ((union lily_a_lexico_numero*) simbolo->valor)->negativo;
            return n.negativo >= -2147483648;
        }
        n.positivo = ((union lily_a_lexico_numero*) simbolo->valor)->positivo;
        return n.positivo <= 4294967295;
    }
    if (!strcmp(tipo, "uint32") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) return false;
        n.positivo = ((union lily_a_lexico_numero*) simbolo->valor)->positivo;
        return n.positivo <= 4294967295;
    }
    if (!strcmp(tipo, "sint32") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) {
            n.negativo = ((union lily_a_lexico_numero*) simbolo->valor)->negativo;
            return n.negativo >= -2147483648;
        }
        n.positivo = ((union lily_a_lexico_numero*) simbolo->valor)->positivo;
        return n.positivo <= 2147483647;
    }
    if ((!strcmp(tipo, "int64")) && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) {
            n.negativo = ((union lily_a_lexico_numero*) simbolo->valor)->negativo;
            return n.negativo >= -9223372036854775807-1;
        }
        n.positivo = ((union lily_a_lexico_numero*) simbolo->valor)->positivo;
        return n.positivo <= 18446744073709551615U;
    }
    if (!strcmp(tipo, "uint64") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) return false;
        n.positivo = ((union lily_a_lexico_numero*) simbolo->valor)->positivo;
        return n.positivo >= 18446744073709551615U;
    }
    if (!strcmp(tipo, "sint64") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) {
            n.negativo = ((union lily_a_lexico_numero*) simbolo->valor)->negativo;
            return n.negativo >= -9223372036854775807-1;
        }
        n.positivo = ((union lily_a_lexico_numero*) simbolo->valor)->positivo;
        return n.positivo <= 9223372036854775807;
    }
    // Es (probablemente) un tipo definido por el usuario
    lua_getglobal(L, "_lily_tabla_idx");
    lua_Integer tabla_idx = lua_tointegerx(L, -1, NULL);
    lua_pushstring(L, "tipos");
    lua_gettable(L, (int) tabla_idx);
    lua_pushstring(L, tipo);
    lua_gettable(L, -2);
    if (lua_isnil(L, -1)) {
        lua_pop(L, 2);
        ctx->codigo = COD_LUA_CPU_TIPO_INEXISTENTE;
        return false;
    }
    // Ajustar valor de símbolo
    if (simbolo->tipo == SIMB_OBJETO) {
        lua_pushstring(L, (char*) simbolo->valor);
    }
    else if (simbolo->tipo == SIMB_NUMERO) {
        lua_pushinteger(L, ((union lily_a_lexico_numero*) simbolo->valor)->negativo);
    }
    else if (simbolo->tipo == SIMB_DESPLAZAMIENTO_AP) {
        lua_createtable(L, (int) lily_lde_size(simbolo->valor), 0);
        for (size_t i = 0; i < lily_lde_size(simbolo->valor); i++ ) {
            struct lily_a_lexico_simbolo* simbolo_desp = lily_lde_get(simbolo->valor, i)->valor;
            if (simbolo_desp->tipo == SIMB_OBJETO)
                lua_pushstring(L, (char*) simbolo_desp->valor);
            else if (simbolo_desp->tipo == SIMB_NUMERO)
                lua_pushinteger(L, ((union lily_a_lexico_numero*) simbolo_desp->valor)->negativo);
            lua_seti(L, -2, (int) i + 1);
        }
    }
    // Llamar función
    if (lua_pcall(L, 1, 1, 0) != LUA_OK) {
        ctx->codigo = COD_LUA_CPU_TIPO_INEXISTENTE;
        ctx->lua_msg = (char*) lua_tostring(L, -1);
        lua_pop(L, 2);
        return false;
    }
    if (!lua_isboolean(L, -1)) {
        lua_pop(L, 2);
        ctx->codigo = COD_LUA_CPU_TIPO_FUNCION_NO_BOOLEANO;
        return false;
    }
    bool res = lua_toboolean(L, -1);
    lua_pop(L, 2);
    return res;
}

static void lily_lua_cpu_est_parametros(lua_State* L, struct lily_lde_lde* params, struct lily_error_ctx* ctx) {
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
                    return;
                }
            }
            lua_seti(L, -2, i + 1);
        }
        else {
            // FIX: quitar este mensaje de log, es una condición que debería ser imposible
            log_warn_gen("El preprocesado se olvidó %s", lily_a_lexico_simbolo_print(simbolo));
            return;
        }
    }
    ctx->codigo = COD_OK;
}

static uint8_t* lily_lua_cpu_procesar_resultado(lua_State* L, lua_Integer* tam, struct lily_error_ctx* ctx) {
    ctx->codigo = COD_OK;
    if (!lua_istable(L, -1)) {
        ctx->codigo = COD_LUA_CPU_RES_ENSAMBLE_NO_TABLA;
        return NULL;
    }
    lua_len(L, -1);
    *tam = lua_tointeger(L, -1);
    lua_pop(L, 1);
    if (tam == 0) {
        ctx->codigo = COD_LUA_CPU_RES_ENSAMBLE_VACIO;
        return NULL;
    }
    uint8_t* bytes = calloc(*tam, sizeof(uint8_t));
    if (bytes == NULL) {
        ctx->codigo = COD_MALLOC_FALLO;
        return NULL;
    }
    for (lua_Integer i = 0; i < *tam; i++) {
        lua_geti(L, -1, i+1);
        if (!lua_isinteger(L, -1)) {
            ctx->codigo = COD_LUA_CPU_RES_ENSAMBLE_NO_ENTERO;
            return NULL;
        }
        bytes[i] = lua_tointeger(L, -1);
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
    return bytes;
}

lua_State *lily_lua_cpu_cargar(const char *codigo, struct lily_error_ctx* ctx) {
    ctx->codigo = COD_OK;
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
            if (ctx != NULL) ctx->codigo = COD_LUA_CPU_DESC_NO_TABLA;
            lua_close(L);
            return NULL;
        }
        lua_pushinteger(L, lua_gettop(L));
        lua_setglobal(L, "_lily_tabla_idx");

        // Analizar id
        lua_pushstring(L, "id");
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            if (ctx != NULL) ctx->codigo = COD_LUA_CPU_DESC_NO_ID;
            lua_close(L);
            return NULL;
        }
        if (!lua_isstring(L, -1)) {
            if (ctx != NULL) ctx->codigo = COD_LUA_CPU_DESC_ID_NO_CADENA;
            lua_close(L);
            return NULL;
        }
        const char* objeto_id = lua_tostring(L, -1);
        lua_pop(L, 1);

        // Analizar registros
        lua_pushstring(L, "registros");
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            if (ctx != NULL) ctx->codigo = COD_LUA_CPU_DESC_NO_REGISTROS;
            lua_close(L);
            return NULL;
        }
        if (!lua_istable(L, -1)) {
            if (ctx != NULL) ctx->codigo = COD_LUA_CPU_DESC_REGISTROS_NO_TABLA;
            lua_close(L);
            return NULL;
        }
        lua_pop(L, 1);

        // Analizar tipos
        lua_pushstring(L, "tipos");
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            if (ctx != NULL) ctx->codigo = COD_LUA_CPU_DESC_NO_TIPOS;
            lua_close(L);
            return NULL;
        }
        if (!lua_istable(L, -1)) {
            if (ctx != NULL) ctx->codigo = COD_LUA_CPU_DESC_TIPOS_NO_TABLA;
            lua_close(L);
            return NULL;
        }
        lua_pop(L, 1);

        // Analizar ensamble
        lua_pushstring(L, "ensamble");
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            if (ctx != NULL) ctx->codigo = COD_LUA_CPU_DESC_NO_ENSAMBLE;
            lua_close(L);
            return NULL;
        }
        if (!lua_istable(L, -1)) {
            if (ctx != NULL) ctx->codigo = COD_LUA_CPU_DESC_ENSAMBLE_NO_TABLA;
            lua_close(L);
            return NULL;
        }
        lua_pop(L, 1);

        // Analizar opcodes
        lua_pushstring(L, "opcodes");
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            if (ctx != NULL) ctx->codigo = COD_LUA_CPU_DESC_NO_OPCODES;
            lua_close(L);
            return NULL;
        }
        if (!lua_istable(L, -1)) {
            if (ctx != NULL) ctx->codigo = COD_LUA_CPU_DESC_OPCODES_NO_TABLA;
            lua_close(L);
            return NULL;
        }
        lua_pop(L, 1);

        // Analizar desensamble
        lua_pushstring(L, "desensamble");
        lua_gettable(L, -2);
        if (lua_isnil(L, -1)) {
            if (ctx != NULL) ctx->codigo = COD_LUA_CPU_DESC_NO_DESENSAMBLE;
            lua_close(L);
            return NULL;
        }
        if (!lua_istable(L, -1)) {
            if (ctx != NULL) ctx->codigo = COD_LUA_CPU_DESC_DESENSAMBLE_NO_TABLA;
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

void lily_lua_cpu_ensamblar(lua_State* L, struct lily_a_sintactico_instruccion* instruccion, struct lily_error_ctx* ctx) {
    #define ptipo(pre) printf("%s: %s\n", (pre), lua_typename(L, lua_type(L, -1)));
    const char* mnemo = instruccion->simbolo->valor;
    lua_Integer tam;
    ctx->codigo = COD_OK;
    ctx->lua_msg = NULL;
    lua_pushstring(L, "ensamble");
    lua_gettable(L, -2);
    lua_pushstring(L, mnemo);
    lua_gettable(L, -2); // En este punto, todos los mnemónicos deben existir, pues ya se ha validado
    // Determinar tipo de mnemónico
    if (lua_isfunction(L, -1)) {
        printf("%s es de tipo función\n", mnemo);
        //*error_ctx = lily_lua_cpu_est_parametros(L, params); // FIX: ver si vale la pena manejar un error aquí
        if (lua_pcall(L, 0, 1, 0) == LUA_OK) {
            instruccion->bytes = lily_lua_cpu_procesar_resultado(L, &tam, ctx);
            instruccion->tam_bytes = (size_t) tam;
            lua_pop(L, 1);
        }
    } else {
        // En este punto puede ser: List<int>, Tuple<char*, function>, List<Tuple<Params, Value>>
        int tipo_tentativo = lua_geti(L, lua_gettop(L), 1);
        if (tipo_tentativo == LUA_TNUMBER) {
            printf("%s es de tipo List<int>\n", mnemo);
            lua_pop(L, 1);
            instruccion->bytes = lily_lua_cpu_procesar_resultado(L, &tam, ctx);
            instruccion->tam_bytes = (size_t) tam;
            lua_pop(L, 1);
        }
        else if (tipo_tentativo == LUA_TSTRING) {
            printf("%s es de tipo Tuple<char*, funcion>\n", mnemo);
            const char* mnemo_base = lua_tostring(L, -1);
            lua_pop(L, 3);
            // TODO: comprobar que la dependencia sea un DAG
            //bytes = lily_lua_cpu_ensamblar(L, mnemo_base, ctx);
        }
        else if (tipo_tentativo == LUA_TTABLE) {
            printf("%s es de tipo List<Tuple<Params, Value>>\n", mnemo);
            for (lua_Integer i = 0; lua_isnil(L, -1); i++) {
                // Bucle para cada opción
                lua_geti(L, -1, i);
                bool coincide = true;
                for (lua_Integer j = 0; lua_isnil(L, -1); j++) {
                    // Bucle para analizar cada parámetro
                    lua_geti(L, -1, j);
                    const char* tipo_param = lua_tostring(L, -1);
                }
                lua_pop(L, 1);
            }
        }
    }
    printf(">>%d\n", lua_gettop(L));
}
