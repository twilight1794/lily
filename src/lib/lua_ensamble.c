#include "lua_ensamble.h"

#include <ctype.h>

/**
 * Comprueba si \a simbolo cumple con alguno de los tipos conocidos por Lily
 * @param L Sesión de Lua
 * @param tipo Tipo contra el cual comprobar
 * @param simbolo Símbolo a comprobar
 * @param [out] ctx Estado de la ejecución al momento de salir de la función
 * @return \c true si el símbolo coincide con el tipo, \c false si no
 */
static bool lily_lua_cpu_comp_tipo_simbolo(lua_State* L, const char* tipo, struct lily_simbolo_simbolo* simbolo, struct lily_ctx* ctx);

/**
 * Lee la lista de parámetros en \a params para una instrucción, y prepara sus valores en la pila para Lua
 * @param L Sesión de Lua
 * @param params Lista de parámetros para el mnemónico
 * @param [out] ctx Estado de la ejecución al momento de salir de la función
 */
static void lily_lua_cpu_est_parametros(lua_State* L, struct lily_lde_lde* params, struct lily_ctx* ctx);

/**
 * Obtiene el resultado del ensamble de una instrucción
 * @param L Sesión de Lua
 * @param [out] tam Número de bytes generados al ensamblar
 * @param [out] ctx Estado de la ejecución al momento de salir de la función
 * @return Cadena de bytes que representan al mnemónico
 */
static uint8_t* lily_lua_cpu_procesar_resultado(lua_State* L, lua_Integer* tam, struct lily_ctx* ctx);

/**
 * Ensambla un mnemónico definido como una función
 * @param L Sesión de Lua
 * @param instruccion Instrucción que contiene el mnemónico a ensamblar
 * @param ctx Estado de la ejecución al momento de salir de la función
 */
static void lily_lua_cpu_ensamblar_funcion(lua_State* L, struct lily_simbolo_instruccion* instruccion, struct lily_ctx* ctx);

/**
 * Ensambla un mnemónico definido como una lista de bytes
 * @param L Sesión de Lua
 * @param instruccion Instrucción que contiene el mnemónico a ensamblar
 * @param ctx Estado de la ejecución al momento de salir de la función
 */
static void lily_lua_cpu_ensamblar_lista(lua_State* L, struct lily_simbolo_instruccion* instruccion, struct lily_ctx* ctx);

/**
 * Ensambla un mnemónico definido como una redirección a otro mnemónico
 * @param L Sesión de Lua
 * @param instruccion Instrucción que contiene el mnemónico a ensamblar
 * @param ctx Estado de la ejecución al momento de salir de la función
 */
static void lily_lua_cpu_ensamblar_redireccion(lua_State* L, struct lily_simbolo_instruccion* instruccion, struct lily_ctx* ctx);

/**
 * Ensambla un mnemónico definido como una lista de diferentes combinaciones de argumentos
 * @param L Sesión de Lua
 * @param instruccion Instrucción que contiene el mnemónico a ensamblar
 * @param ctx Estado de la ejecución al momento de salir de la función
 */
static void lily_lua_cpu_ensamblar_lparams(lua_State* L, struct lily_simbolo_instruccion* instruccion, struct lily_ctx* ctx);

static bool lily_lua_cpu_comp_tipo_simbolo(lua_State* L, const char* tipo, struct lily_simbolo_simbolo* simbolo, struct lily_ctx* ctx) {
    union lily_simbolo_numero n;
    // Primero, comprobar si el tipo es uno precargado
    if (!strcmp(tipo, "int3") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) {
            n.negativo = ((union lily_simbolo_numero*) simbolo->valor)->negativo;
            return n.negativo >= -4;
        }
        n.positivo = ((union lily_simbolo_numero*) simbolo->valor)->positivo;
        return n.positivo <= 7;
    }
    if (!strcmp(tipo, "uint3") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) return false;
        n.positivo = ((union lily_simbolo_numero*) simbolo->valor)->positivo;
        return n.positivo <= 7;
    }
    if (!strcmp(tipo, "sint3") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) {
            n.negativo = ((union lily_simbolo_numero*) simbolo->valor)->negativo;
            return n.negativo >= -4;
        }
        n.positivo = ((union lily_simbolo_numero*) simbolo->valor)->positivo;
        return n.positivo <= 3;
    }
    if (!strcmp(tipo, "int4") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) {
            n.negativo = ((union lily_simbolo_numero*) simbolo->valor)->negativo;
            return n.negativo >= -8;
        }
        n.positivo = ((union lily_simbolo_numero*) simbolo->valor)->positivo;
        return n.positivo <= 15;
    }
    if (!strcmp(tipo, "uint4") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) return false;
        n.positivo = ((union lily_simbolo_numero*) simbolo->valor)->positivo;
        return n.positivo <= 15;
    }
    if (!strcmp(tipo, "sint4") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) {
            n.negativo = ((union lily_simbolo_numero*) simbolo->valor)->negativo;
            return n.negativo >= -8;
        }
        n.positivo = ((union lily_simbolo_numero*) simbolo->valor)->positivo;
        return n.positivo <= 7 ;
    }
    if (!strcmp(tipo, "int8") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) {
            n.negativo = ((union lily_simbolo_numero*) simbolo->valor)->negativo;
            return n.negativo >= -128;
        }
        n.positivo = ((union lily_simbolo_numero*) simbolo->valor)->positivo;
        return n.positivo <= 255;
    }
    if (!strcmp(tipo, "uint8") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) return false;
        n.positivo = ((union lily_simbolo_numero*) simbolo->valor)->positivo;
        return n.positivo <= 255;
    }
    if (!strcmp(tipo, "sint8") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) {
            n.negativo = ((union lily_simbolo_numero*) simbolo->valor)->negativo;
            return n.negativo >= -128;
        }
        n.positivo = ((union lily_simbolo_numero*) simbolo->valor)->positivo;
        return n.positivo <= 127;
    }
    if (!strcmp(tipo, "int16") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) {
            n.negativo = ((union lily_simbolo_numero*) simbolo->valor)->negativo;
            return n.negativo >= -32768;
        }
        n.positivo = ((union lily_simbolo_numero*) simbolo->valor)->positivo;
        return n.positivo <= 65535;
    }
    if (!strcmp(tipo, "uint16") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) return false;
        n.positivo = ((union lily_simbolo_numero*) simbolo->valor)->positivo;
        return n.positivo <= 65535;
    }
    if (!strcmp(tipo, "sint16") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) {
            n.negativo = ((union lily_simbolo_numero*) simbolo->valor)->negativo;
            return n.negativo >= -32768;
        }
        n.positivo = ((union lily_simbolo_numero*) simbolo->valor)->positivo;
        return n.positivo <= 32767;
    }
    if (!strcmp(tipo, "int32") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) {
            n.negativo = ((union lily_simbolo_numero*) simbolo->valor)->negativo;
            return n.negativo >= -2147483648;
        }
        n.positivo = ((union lily_simbolo_numero*) simbolo->valor)->positivo;
        return n.positivo <= 4294967295;
    }
    if (!strcmp(tipo, "uint32") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) return false;
        n.positivo = ((union lily_simbolo_numero*) simbolo->valor)->positivo;
        return n.positivo <= 4294967295;
    }
    if (!strcmp(tipo, "sint32") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) {
            n.negativo = ((union lily_simbolo_numero*) simbolo->valor)->negativo;
            return n.negativo >= -2147483648;
        }
        n.positivo = ((union lily_simbolo_numero*) simbolo->valor)->positivo;
        return n.positivo <= 2147483647;
    }
    if ((!strcmp(tipo, "int64")) && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) {
            n.negativo = ((union lily_simbolo_numero*) simbolo->valor)->negativo;
            return n.negativo >= -9223372036854775807-1;
        }
        n.positivo = ((union lily_simbolo_numero*) simbolo->valor)->positivo;
        return n.positivo <= 18446744073709551615U;
    }
    if (!strcmp(tipo, "uint64") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) return false;
        n.positivo = ((union lily_simbolo_numero*) simbolo->valor)->positivo;
        return n.positivo >= 18446744073709551615U;
    }
    if (!strcmp(tipo, "sint64") && simbolo->tipo == SIMB_NUMERO) {
        if (simbolo->signo) {
            n.negativo = ((union lily_simbolo_numero*) simbolo->valor)->negativo;
            return n.negativo >= -9223372036854775807-1;
        }
        n.positivo = ((union lily_simbolo_numero*) simbolo->valor)->positivo;
        return n.positivo <= 9223372036854775807;
    }
    // Es (probablemente) un tipo definido por el usuario
    lua_getglobal(L, "_lily_tabla_idx");
    lua_Integer tabla_idx = lua_tointegerx(L, -1, NULL);
    lua_pop(L, 1);
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
        lua_pushinteger(L, ((union lily_simbolo_numero*) simbolo->valor)->negativo);
    }
    else if (simbolo->tipo == SIMB_OPERADOR && simbolo->subtipo == SIMB_DESPLAZAMIENTO_AP) {
        lua_createtable(L, (int) lily_lde_size(simbolo->valor), 0);
        for (size_t i = 0; i < lily_lde_size(simbolo->valor); i++) {
            struct lily_simbolo_simbolo* simbolo_desp = lily_lde_get(simbolo->valor, i)->valor;
            if (simbolo_desp->tipo == SIMB_OBJETO)
                lua_pushstring(L, (char*) simbolo_desp->valor);
            else if (simbolo_desp->tipo == SIMB_NUMERO)
                lua_pushinteger(L, ((union lily_simbolo_numero*) simbolo_desp->valor)->negativo);
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

static void lily_lua_cpu_est_parametros(lua_State* L, struct lily_lde_lde* params, struct lily_ctx* ctx) {
    lua_Integer tam = (lua_Integer) lily_lde_size(params);
    for (lua_Integer i = 0;  i < tam; i++) {
        struct lily_lde_nodo* nodo = lily_lde_get(params, i);
        struct lily_simbolo_simbolo* simbolo = nodo->valor;
        if (simbolo->tipo == SIMB_OBJETO) {
            lua_pushstring(L, (char*) simbolo->valor);
        }
        else if (simbolo->tipo == SIMB_NUMERO) {
            // FIX: por ahora, un entero
            lua_pushinteger(L, (lua_Integer) ((union lily_simbolo_numero*) simbolo->valor)->positivo);
        }
        else if (simbolo->tipo == SIMB_OPERADOR && simbolo->subtipo == SIMB_DESPLAZAMIENTO_AP) {
            int tam_desp = (int) lily_lde_size((struct lily_lde_lde*) simbolo->valor);
            lua_createtable(L, (int) tam_desp, 0);
            for (int j = 0; j < tam_desp; j++) {
                struct lily_lde_nodo* nodo_desp = lily_lde_get(simbolo->valor, j);
                struct lily_simbolo_simbolo* simbolo_desp = nodo_desp->valor;
                if (simbolo_desp->tipo == SIMB_OBJETO) {
                    lua_pushstring(L, (char*) simbolo_desp->valor);
                    lua_seti(L, -2, j + 1);
                } else if (simbolo_desp->tipo == SIMB_NUMERO) {
                    // FIX: por ahora, un entero
                    lua_pushinteger(L, (lua_Integer) ((union lily_simbolo_numero*) simbolo->valor)->positivo);
                    lua_seti(L, -2, j + 1);
                } else {
                    // FIX: quitar este mensaje de log, es una condición que debería ser imposible
                    log_warn_gen("El preprocesado se olvidó %s", lily_simbolo_simbolo_print(simbolo_desp));
                    return;
                }
            }
            lua_seti(L, -2, i + 1);
        }
        else {
            // FIX: quitar este mensaje de log, es una condición que debería ser imposible
            log_warn_gen("El preprocesado se olvidó %s", lily_simbolo_simbolo_print(simbolo));
            return;
        }
    }
}

static uint8_t* lily_lua_cpu_procesar_resultado(lua_State* L, lua_Integer* tam, struct lily_ctx* ctx) {
    if (!lua_istable(L, -1)) {
        ctx->codigo = COD_LUA_CPU_RES_ENSAMBLE_NO_TABLA;
        return NULL;
    }
    lua_len(L, -1);
    *tam = lua_tointeger(L, -1);
    lua_pop(L, 1);
    if (*tam == 0) {
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

static void lily_lua_cpu_ensamblar_funcion(lua_State* L, struct lily_simbolo_instruccion* instruccion, struct lily_ctx* ctx) {
    log_debug_gen(_("lua_cpu: %s es de tipo función"), (char*) instruccion->simbolo->valor);
    lily_lua_cpu_est_parametros(L, instruccion->params, ctx);
    if (lua_pcall(L, lily_lde_size(instruccion->params), 1, 0) == LUA_OK) {
        lua_Integer tam;
        instruccion->bytes = lily_lua_cpu_procesar_resultado(L, &tam, ctx);
        instruccion->tam_bytes = (size_t) tam;
    } else {
        ctx->codigo = COD_LUA_CPU_LUA_ERR;
        ctx->lua_msg = (char*) lua_tostring(L, -1);
        lua_pop(L, 1);
    }
}

static void lily_lua_cpu_ensamblar_lista(lua_State* L, struct lily_simbolo_instruccion* instruccion, struct lily_ctx* ctx) {
    log_debug_gen(_("lua_cpu: %s es de tipo List<int>"), (char*) instruccion->simbolo->valor);
    lua_pop(L, 1);
    lua_Integer tam;
    instruccion->bytes = lily_lua_cpu_procesar_resultado(L, &tam, ctx);
    instruccion->tam_bytes = (size_t) tam;
}

static void lily_lua_cpu_ensamblar_redireccion(lua_State* L, struct lily_simbolo_instruccion* instruccion, struct lily_ctx* ctx) {
    log_debug_gen(_("lua_cpu: %s es de tipo Tuple<char*, funcion> -> %s"), (char*) instruccion->simbolo->valor SEP lua_tostring(L, -1));
    lua_pop(L, 1);
    lua_geti(L, -1, 2); // <- Función para modificar el resultado (idx 9)
    lua_geti(L, -2, 1); // <- Cadena con el mnemónico al cual redirigir (idx 10)
    lua_getglobal(L, "_lily_tabla_idx");
    lua_Integer tabla_mnemos_idx = lua_tointeger(L, -1) + 1;
    lua_pop(L, 1);
    lua_gettable(L, (int) tabla_mnemos_idx);
    if (lua_isfunction(L, -1))
        lily_lua_cpu_ensamblar_funcion(L, instruccion, ctx);
    else {
        int tipo_tentativo = lua_geti(L, -1, 1);
        if (tipo_tentativo == LUA_TNUMBER)
            lily_lua_cpu_ensamblar_lista(L, instruccion, ctx);
        else if (tipo_tentativo == LUA_TTABLE)
            lily_lua_cpu_ensamblar_lparams(L, instruccion, ctx);
    }
    lua_pop(L, 1);

    // Generar tabla como argumento de la función de transformación
    lua_createtable(L, (int) instruccion->tam_bytes, 0);
    for (size_t i = 0; i < instruccion->tam_bytes; i++) {
        lua_pushinteger(L, (int) instruccion->bytes[i]);
        lua_seti(L, -2, (lua_Integer) i+1);
    }
    free(instruccion->bytes);

    // Ejecutar función de transformación
    if (lua_pcall(L, 1, 1, 0) == LUA_OK) {
        lua_Integer tam;
        instruccion->bytes = lily_lua_cpu_procesar_resultado(L, &tam, ctx);
        instruccion->tam_bytes = (size_t) tam;
    } else {
        ctx->codigo = COD_LUA_CPU_LUA_ERR;
        ctx->lua_msg = (char*) lua_tostring(L, -1);
        lua_pop(L, 1);
    }
}

static void lily_lua_cpu_ensamblar_lparams(lua_State* L, struct lily_simbolo_instruccion* instruccion, struct lily_ctx* ctx) {
    log_debug_gen(_("lua_cpu: %s es de tipo List<Tuple<List<Args>, Value>>"), (char*) instruccion->simbolo->valor);
    lua_pop(L, 1);
    lua_len(L, -1);
    lua_Integer tam_i = lua_tointeger(L, -1);
    lua_pop(L, 1);
    log_debug_gen(_("lua_cpu: hay %lld casos para %s"), tam_i SEP (char*) instruccion->simbolo->valor);
    for (lua_Integer i = 1; i <= tam_i; i++) {
        // Bucle para cada opción
        lua_geti(L, -1, i); // <- Tuple<List<Args>, values> (idx 9)
        lua_geti(L, -1, 1); // <- List<Args> (idx 10)

        // Obtener número de parámetros
        lua_len(L, -1);
        lua_Integer tam_params = lua_tointeger(L, -1);
        lua_pop(L, 1);

        // Obtener lista de tipos de parámetros
        char** lista_params = (char**) malloc(sizeof(char*)*tam_params);
        if (lista_params == NULL) {
            ctx->codigo = COD_MALLOC_FALLO;
            return;
        }
        size_t tam_cad_params = 0;
        for (lua_Integer j = 0; j < tam_params; j++) {
            lua_geti(L, -1, j + 1);
            lista_params[j] = (char*) lua_tostring(L, -1);
            lua_pop(L, 1);
            tam_cad_params += strlen(lista_params[j]) + 2;
        }
        tam_cad_params -= 1 ;

        // Generar mensaje de log
        char* msg_cad_params = (char*) malloc(sizeof(char)*tam_cad_params);
        if (msg_cad_params == NULL) {
            ctx->codigo = COD_MALLOC_FALLO;
            return;
        }
        lua_Integer k = 0;
        for (lua_Integer j = 0; j < tam_params; j++) {
            memcpy(msg_cad_params + k, lista_params[j], strlen(lista_params[j]));
            k += (lua_Integer) strlen(lista_params[j]);
            if (j < tam_params - 1) {
                memcpy(msg_cad_params + k, ", ", 2);
                k += 2;
            }
        }
        msg_cad_params[k] = '\0';
        log_debug_gen(_("lua_cpu: comprobando caso (%s)"), msg_cad_params);

        // Comprobar que los tamaños de las listas de argumentos coincidan
        if ((size_t) tam_params != lily_lde_size(instruccion->params)) {
            // Diferentes tamaños, ésta no es la opción
            lua_pop(L, 2);
            continue;
        }
        bool coincide = true;
        for (lua_Integer j = 0; j < tam_params; j++) {
            // Bucle para analizar cada argumento
            /// Obtener símbolo de argumento
            struct lily_simbolo_simbolo* simbolo_param = lily_lde_get(instruccion->params, j)->valor;
            /// Comprobar argumento
            coincide = lily_lua_cpu_comp_tipo_simbolo(L, lista_params[j], simbolo_param, ctx);
            if (!coincide) break;
        }
        lua_pop(L, 1); // quita idx 10

        // Es la opción
        if (coincide) {
            log_debug_gen(_("lua_cpu: seleccionado caso (%s)"), msg_cad_params);
            lua_geti(L, -1, 2); // values (idx 10)
            if (lua_isfunction(L, -1)) {
                lily_lua_cpu_ensamblar_funcion(L, instruccion, ctx);
                lua_pop(L, 1);
            }
            else {
                int tipo_tentativo = lua_geti(L, -1, 1);
                if (tipo_tentativo == LUA_TNUMBER) {
                    lily_lua_cpu_ensamblar_lista(L, instruccion, ctx);
                    lua_pop(L, 1);
                }
                else if (tipo_tentativo == LUA_TSTRING) {
                    lily_lua_cpu_ensamblar_redireccion(L, instruccion, ctx);
                    lua_pop(L, 2);
                }
            }
            return;
        }
        free(lista_params);
        free(msg_cad_params);
        lua_pop(L, 1);
    }
    // Si llegamos acá, no hay opciones para los argumentos provistos
    ctx->codigo = COD_LUA_CPU_CONJUNTO_ARGS_INEXISTENTE;
    ctx->ultimo = instruccion->simbolo;
}

void lily_lua_cpu_ensamblar(lua_State* L, struct lily_simbolo_instruccion* instruccion, struct lily_ctx* ctx) {
    // Obtener el mnemónico canónico
    char* mnemo = instruccion->simbolo->valor;
    for (size_t i = 0; i < strlen(mnemo); i++)
        mnemo[i] = toupper(mnemo[i]);

    // Obtener definición asociada al mnemónico
    lua_pushstring(L, "ensamble");
    lua_gettable(L, -2);
    lua_pushstring(L, mnemo);
    lua_gettable(L, -2);

    // Determinar tipo de mnemónico
    if (lua_isnil(L, -1)) {
        // El mnemónico no existe
        ctx->codigo = COD_LUA_CPU_MNEMO_INEXISTENTE;
        ctx->ultimo = instruccion->simbolo;
        lua_pop(L, lua_gettop(L));
        return;
    }
    if (lua_isfunction(L, -1)) {
        lily_lua_cpu_ensamblar_funcion(L, instruccion, ctx);
        lua_pop(L, 1);
    }
    else {
        // En este punto puede ser: List<int>, Tuple<char*, function>, List<Tuple<List<Args>, Value>>
        int tipo_tentativo = lua_geti(L, -1, 1);
        if (tipo_tentativo == LUA_TNUMBER) {
            lily_lua_cpu_ensamblar_lista(L, instruccion, ctx);
            lua_pop(L, 1);
        }
        else if (tipo_tentativo == LUA_TSTRING) {
            lily_lua_cpu_ensamblar_redireccion(L, instruccion, ctx);
            lua_pop(L, 2);
        }
        else if (tipo_tentativo == LUA_TTABLE) {
            lily_lua_cpu_ensamblar_lparams(L, instruccion, ctx);
            lua_pop(L, 2);
        }
    }
}
