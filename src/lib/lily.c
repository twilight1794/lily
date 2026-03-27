#include "lily.h"

#define lua_msg()                                                 \
    msg_buf = d_printf("codigo=%d (%s)", *estado, ((struct lily_a_semantico_ctx*) (*ctx))->lua_msg); \
    fun_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_FATAL, "lily_ensamble", msg_buf); \
    free(msg_buf);

uint8_t* lily_lily_ensamble(const char* datos_entrada, char* arquitectura, struct lily_lily_archivo* (fun_abrir_archivo)(const char*, int, int*), int (fun_cerrar_archivo)(struct lily_lily_archivo*), f_mensajes_ptr fun_mensaje, size_t* tam_salida, enum lily_estado* estado, void** ctx) {
    char* msg_buf;
    // Análisis léxico
    struct lily_lde_lde* simbolos = lily_a_lexico(datos_entrada, fun_mensaje, estado, ctx);
    if (*estado != COD_OK) {
        struct lily_a_lexico_ctx* tmp_ctx = (struct lily_a_lexico_ctx*) *ctx;
        char caracter_prob = datos_entrada[tmp_ctx->i_desp];
        msg_buf = d_printf("tipo=%d, i_inicial=%lu, i_desp=%lu (0x%x \"%c\").", tmp_ctx->tipo, tmp_ctx->i_inicial, tmp_ctx->i_desp, caracter_prob, isprint(caracter_prob)?caracter_prob:'?');

        fun_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "lily_ensamble", msg_buf);
        free(msg_buf);
        return NULL;
    }

    // Análisis sintáctico
    struct lily_lde_lde* ast = lily_a_sintactico(simbolos, fun_mensaje, estado, ctx);
    if (*estado != COD_OK) {
        struct lily_a_sintactico_ctx* tmp_ctx = (struct lily_a_sintactico_ctx*) *ctx;
        msg_buf = d_printf("codigo=%d, %d.%d (%lu)", *estado, tmp_ctx->ultimo->tipo, tmp_ctx->ultimo->subtipo, tmp_ctx->ultimo->linea);
        fun_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "lily_ensamble", msg_buf);
        free(msg_buf);
        return NULL;
    }

    // Cargar y preparar entorno de Lua
    lua_State* L = lily_lua_entorno_preparar(estado);
    if (*estado != COD_OK) {
        lua_msg();
        return NULL;
    }
    lily_lua_int_preparar(L);
    if (*estado != COD_OK) {
        lua_msg();
        return NULL;
    }

    // Determinar arquitectura a usar
    char* arquitectura_final = NULL;
    char* arquitectura_asm = lily_a_semantico_obt_arquitectura_declarada(ast, estado, ctx);
    if (arquitectura != NULL) {
        arquitectura_final = arquitectura;
        if (strcmp((arquitectura_asm == NULL)?"":arquitectura_asm, arquitectura) != 0) {
            msg_buf = d_printf("The architecture specified at parameter '%s' is different to architecture specified at file '%s'.", arquitectura, arquitectura_asm);
            fun_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_WARN, "lily_ensamble", msg_buf);
            free(msg_buf);
        }
    }
    else if (arquitectura_asm != NULL)
        arquitectura_final = arquitectura_asm;
    else {
        fun_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_FATAL, "lily_ensamble", "An architecture was not specified.");
        return NULL;
    }
    msg_buf = d_printf("The architecture selected is '%s'", arquitectura_final);
    fun_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_INFO, "lily_ensamble", msg_buf);
    free(msg_buf);
    // TODO: ver si se puede prescindir de esa conversión a int*
    struct lily_lily_archivo* archivo_arquitectura = fun_abrir_archivo(arquitectura_final, 0, (int*) estado);
    if (archivo_arquitectura == NULL) {
        *estado = COD_LILY_SIN_ESQUEMA;
        return NULL;
    }

    // Abrir esquema de la arquitectura en la ruta generada
    lily_lua_cpu_cargar(L, archivo_arquitectura->archivo, estado, ctx);
    if (*estado != COD_OK) {
        lua_msg();
        return NULL;
    }
    fun_cerrar_archivo(archivo_arquitectura);

    // Análisis semántico
    uint8_t* datos_salida = lily_a_semantico(ast, L, 0, tam_salida, fun_mensaje, estado, ctx);
    if (*estado != COD_OK) {
        struct lily_a_semantico_ctx* tmp_ctx = (struct lily_a_semantico_ctx*) *ctx;
        msg_buf = d_printf("codigo=%d, %s (%s)", *estado, lily_simbolo_simbolo_print(tmp_ctx->ultimo), tmp_ctx->lua_msg);
        fun_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_FATAL, "lily_ensamble", msg_buf);
        free(msg_buf);
        return NULL;
    }
    return datos_salida;
}
