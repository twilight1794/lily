#include "lily.h"

uint8_t* lily_lily_ensamble(const char* datos_entrada, char* arquitectura, struct lily_lily_archivo* (fun_abrir_archivo)(const char*, int), int (fun_cerrar_archivo)(struct lily_lily_archivo*), size_t* tam_salida, struct lily_ctx* ctx) {
    // Análisis léxico
    struct lily_lde_lde* simbolos = lily_a_lexico(datos_entrada, ctx);
    log_info_gen(_("lily_a_lexico: %d."), ctx->codigo);
    if (ctx->codigo != COD_OK) {
        char caracter_prob = datos_entrada[ctx->i_desp];
        log_fatal_gen(_("type=%d, initial_i=%lu, offset_i=%lu (0x%x \"%c\")."), ctx->tipo SEP ctx->i_inicial SEP ctx->i_desp SEP caracter_prob SEP isprint(caracter_prob)?caracter_prob:'?');
        return NULL;
    }

    // Análisis sintáctico
    struct lily_lde_lde* ast = lily_a_sintactico(simbolos, ctx);
    log_info_gen(_("lily_a_sintactico: %d."), ctx->codigo);
    if (ctx->codigo != COD_OK) {
        log_fatal_gen(_("codigo=%d, %d.%d (%lu)"), ctx->codigo SEP ctx->ultimo->tipo SEP ctx->ultimo->subtipo SEP ctx->ultimo->linea);
        return NULL;
    }

    // Determinar arquitectura a usar
    char* arquitectura_final = NULL;
    char* arquitectura_asm = lily_a_semantico_obt_arquitectura_declarada(ast, ctx);
    if (arquitectura != NULL) {
        arquitectura_final = arquitectura;
        if (strcmp((arquitectura_asm == NULL)?"":arquitectura_asm, arquitectura) != 0) {
            log_warn_gen(_("The architecture specified at parameter '%s' is different to architecture specified at file '%s'."), arquitectura SEP arquitectura_asm);
        }
    } else if (arquitectura_asm != NULL)
        arquitectura_final = arquitectura_asm;
    else {
        log_fatal(_("An architecture was not specified."));
        return NULL;
    }
    log_info_gen(_("The architecture selected is '%s'"), arquitectura_final);
    struct lily_lily_archivo* archivo_arquitectura = fun_abrir_archivo(arquitectura_final, 0);

    // Abrir ruta generada
    lua_State* L = lily_lua_cpu_cargar(archivo_arquitectura->archivo, ctx);
    log_info_gen(_("lily_lua_cpu_cargar: %d."), ctx->codigo);
    if (ctx->codigo != COD_OK) {
        log_fatal_gen(_("codigo=%d (%s)"), ctx->codigo SEP ctx->lua_msg);
        return NULL;
    }
    fun_cerrar_archivo(archivo_arquitectura);

    // Análisis semántico
    uint8_t* datos_salida = lily_a_semantico(ast, L, 0, tam_salida, ctx);
    log_info_gen(_("lily_a_semantico: %d (%ld bytes)."), ctx->codigo SEP *tam_salida);
    if (ctx->codigo != COD_OK) {
        log_fatal_gen(_("codigo=%d, %s, Lua: '%s')"), ctx->codigo SEP lily_simbolo_simbolo_print(ctx->ultimo) SEP ctx->lua_msg);
        return NULL;
    }
    return datos_salida;
}