#include "lily.h"

uint8_t* lily_lily_ensamble(const char* datos_entrada, char* arquitectura, struct lily_lily_archivo* (fun_abrir_archivo)(const char*, int, struct lily_ctx*), int (fun_cerrar_archivo)(struct lily_lily_archivo*), size_t* tam_salida, struct lily_ctx* ctx) {
    struct lily_log_config* l = (struct lily_log_config*) ctx->log_cfg;
    // Análisis léxico
    struct lily_lde_lde* simbolos = lily_a_lexico(datos_entrada, ctx);
    log_info_v(l, "lily_ensamble", _("lily_a_lexico: %d."), ctx->codigo);
    if (ctx->codigo != COD_OK) {
        char caracter_prob = datos_entrada[ctx->i_desp];
        log_fatal_v(l, "lily_ensamble", _("type=%d, initial_i=%lu, offset_i=%lu (0x%x \"%c\")."), ctx->tipo, ctx->i_inicial, ctx->i_desp, caracter_prob, isprint(caracter_prob)?caracter_prob:'?');
        return NULL;
    }

    // Análisis sintáctico
    struct lily_lde_lde* ast = lily_a_sintactico(simbolos, ctx);
    log_info_v(l, "lily_ensamble", _("lily_a_sintactico: %d."), ctx->codigo);
    if (ctx->codigo != COD_OK) {
        log_fatal_v(l, "lily_ensamble", _("codigo=%d, %d.%d (%lu)"), ctx->codigo, ctx->ultimo->tipo, ctx->ultimo->subtipo, ctx->ultimo->linea);
        return NULL;
    }

    // Cargar y preparar entorno de Lua
    lua_State* L = lily_lua_entorno_preparar(ctx);
    if (ctx->codigo != COD_OK) {
        log_fatal_v(l, "lily_ensamble", _("codigo=%d (%s)"), ctx->codigo, ctx->lua_msg);
        return NULL;
    }
    lily_lua_int_preparar(L, ctx);
    if (ctx->codigo != COD_OK) {
        log_fatal_v(l, "lily_ensamble", _("codigo=%d (%s)"), ctx->codigo, ctx->lua_msg);
        return NULL;
    }

    // Determinar arquitectura a usar
    char* arquitectura_final = NULL;
    char* arquitectura_asm = lily_a_semantico_obt_arquitectura_declarada(ast, ctx);
    if (arquitectura != NULL) {
        arquitectura_final = arquitectura;
        if (strcmp((arquitectura_asm == NULL)?"":arquitectura_asm, arquitectura) != 0) {
            log_warn_v(l, "lily_ensamble", _("The architecture specified at parameter '%s' is different to architecture specified at file '%s'."), arquitectura, arquitectura_asm);
        }
    } else if (arquitectura_asm != NULL)
        arquitectura_final = arquitectura_asm;
    else {
        log_fatal(l, "lily_ensamble", _("An architecture was not specified."));
        return NULL;
    }
    log_info_v(l, "lily_ensamble", _("The architecture selected is '%s'"), arquitectura_final);
    struct lily_lily_archivo* archivo_arquitectura = fun_abrir_archivo(arquitectura_final, 0, ctx);

    // Abrir esquema de la arquitectura en la ruta generada
    lily_lua_cpu_cargar(L, archivo_arquitectura->archivo, ctx);
    log_info_v(l, "lily_ensamble", _("lily_lua_cpu_cargar: %d."), ctx->codigo);
    if (ctx->codigo != COD_OK) {
        log_fatal_v(l, "lily_ensamble", _("codigo=%d (%s)"), ctx->codigo, ctx->lua_msg);
        return NULL;
    }
    fun_cerrar_archivo(archivo_arquitectura);

    // Análisis semántico
    uint8_t* datos_salida = lily_a_semantico(ast, L, 0, tam_salida, ctx);
    log_info_v(l, "lily_ensamble", _("lily_a_semantico: %d (%ld bytes)."), ctx->codigo, *tam_salida);
    if (ctx->codigo != COD_OK) {
        log_fatal_v(l, "lily_ensamble", _("codigo=%d, %s, Lua: '%s')"), ctx->codigo, lily_simbolo_simbolo_print(ctx->ultimo), ctx->lua_msg);
        return NULL;
    }
    return datos_salida;
}
