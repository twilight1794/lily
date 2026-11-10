/*
 * Copyright (C) 2026 Giovanni Alfredo Garciliano Diaz
 *
 * This file is part of Lily.
 *
 * Lily is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Lily is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Lily. If not, see <https://www.gnu.org/licenses/>.
 */

#include "lily.h"

#define codigo_msg()                                              \
    msg_buf = d_printf("codigo=%d", *estado);                     \
    fun_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_FATAL, "lily_ensamble", msg_buf); \
    free(msg_buf);

uint8_t* lily_lily_ensamble(const char* datos_entrada, char* arquitectura, struct lily_dict_dict* opciones, struct lily_lily_archivo* (fun_abrir_archivo)(const char*, int, int*), int (fun_cerrar_archivo)(struct lily_lily_archivo*), f_mensajes_ptr fun_mensaje, size_t* tam_salida, enum lily_estado* estado, void** ctx) {
    // Todo lo que se reserva aquí se libera en "fin", haya o no error. Cada contexto se libera
    // en cuanto deja de hacer falta: tras informar de un error, o al pasar a la siguiente etapa
    char* msg_buf;
    uint8_t* datos_salida = NULL;
    struct lily_lde_lde* simbolos = NULL;
    struct lily_lde_lde* ast = NULL;
    lua_State* L = NULL;
    char* arquitectura_asm = NULL;
    *ctx = NULL;

    // Análisis léxico
    simbolos = lily_a_lexico(datos_entrada, fun_mensaje, estado, ctx);
    if (*estado != COD_OK) {
        struct lily_a_lexico_ctx* tmp_ctx = (struct lily_a_lexico_ctx*) *ctx;
        if (tmp_ctx != NULL) {
            char caracter_prob = datos_entrada[tmp_ctx->i_desp];
            msg_buf = d_printf("tipo=%d, i_inicial=%lu, i_desp=%lu (0x%x \"%c\").", tmp_ctx->tipo, tmp_ctx->i_inicial, tmp_ctx->i_desp, caracter_prob, isprint(caracter_prob)?caracter_prob:'?');
            fun_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "lily_ensamble", msg_buf);
            free(msg_buf);
        }
        goto fin;
    }

    // Análisis sintáctico
    ast = lily_a_sintactico(simbolos, fun_mensaje, estado, ctx);
    if (*estado != COD_OK) {
        struct lily_a_sintactico_ctx* tmp_ctx = (struct lily_a_sintactico_ctx*) *ctx;
        if (tmp_ctx != NULL && tmp_ctx->ultimo != NULL)
            msg_buf = d_printf("codigo=%d, %d.%d (%lu)", *estado, tmp_ctx->ultimo->tipo, tmp_ctx->ultimo->subtipo, tmp_ctx->ultimo->linea);
        else
            msg_buf = d_printf("codigo=%d", *estado);
        fun_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "lily_ensamble", msg_buf);
        free(msg_buf);
        goto fin;
    }
    free(*ctx);
    *ctx = NULL;

    // Cargar y preparar entorno de Lua (si falla, lily_lua_entorno_preparar ya cerró la sesión)
    L = lily_lua_entorno_preparar(estado, fun_mensaje);
    if (*estado != COD_OK) {
        L = NULL;
        codigo_msg();
        goto fin;
    }
    lily_lua_int_preparar(L);

    // Determinar arquitectura a usar
    char* arquitectura_final = NULL;
    arquitectura_asm = lily_a_semantico_obt_arquitectura_declarada(ast, estado, ctx);
    if (*estado != COD_OK) {
        // La directiva .CPU es inválida
        struct lily_a_semantico_ctx* tmp_ctx = (struct lily_a_semantico_ctx*) *ctx;
        char* cad_simbolo = (tmp_ctx != NULL) ? lily_simbolo_simbolo_print(tmp_ctx->ultimo) : NULL;
        msg_buf = d_printf("codigo=%d, %s", *estado, (cad_simbolo != NULL) ? cad_simbolo : "");
        fun_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_FATAL, "lily_ensamble", msg_buf);
        free(msg_buf);
        free(cad_simbolo);
        goto fin;
    }
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
        *estado = COD_LILY_CPU_NO_ESPECIFICADO;
        fun_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_FATAL, "lily_ensamble", "An architecture was not specified.");
        goto fin;
    }
    msg_buf = d_printf("The architecture selected is '%s'", arquitectura_final);
    fun_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_INFO, "lily_ensamble", msg_buf);
    free(msg_buf);
    // TODO: ver si se puede prescindir de esa conversión a int*
    struct lily_lily_archivo* archivo_arquitectura = fun_abrir_archivo(arquitectura_final, 0, (int*) estado);
    if (archivo_arquitectura == NULL) {
        *estado = COD_LILY_SIN_ESQUEMA;
        goto fin;
    }

    // Abrir esquema de la arquitectura en la ruta generada
    char* lua_msg = NULL;
    lily_lua_cpu_cargar(L, archivo_arquitectura->archivo, estado, &lua_msg);
    // El esquema ya se leyó: el archivo se cierra haya o no error
    fun_cerrar_archivo(archivo_arquitectura);
    if (*estado != COD_OK) {
        L = NULL; // lily_lua_cpu_cargar ya cerró la sesión
        msg_buf = d_printf("codigo=%d (%s)", *estado, lua_msg);
        fun_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_FATAL, "lily_ensamble", msg_buf);
        free(msg_buf);
        free(lua_msg);
        goto fin;
    }

    // Análisis semántico (sin errores, libera su contexto y deja *ctx en NULL)
    datos_salida = lily_a_semantico(ast, L, 0, tam_salida, opciones, fun_mensaje, estado, ctx);
    if (*estado != COD_OK) {
        struct lily_a_semantico_ctx* tmp_ctx = (struct lily_a_semantico_ctx*) *ctx;
        char* cad_simbolo = lily_simbolo_simbolo_print(tmp_ctx->ultimo);
        msg_buf = d_printf("codigo=%d, %s (%s)", *estado, (cad_simbolo != NULL) ? cad_simbolo : "", (tmp_ctx->lua_msg != NULL) ? tmp_ctx->lua_msg : "");
        fun_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_FATAL, "lily_ensamble", msg_buf);
        free(msg_buf);
        free(cad_simbolo);
        free(tmp_ctx->lua_msg);
        free(datos_salida);
        datos_salida = NULL;
    }

fin:
    // Los contextos de las etapas no tienen más memoria propia que el mensaje de Lua del semántico,
    // que ya se liberó arriba
    free(*ctx);
    *ctx = NULL;
    if (L != NULL) lua_close(L);
    free(arquitectura_asm);
    lily_simbolo_instrucciones_destroy(ast);
    lily_simbolo_lista_destroy(simbolos); // Solo quedan los símbolos que nadie tomó
    return datos_salida;
}

struct lily_lua_ejecucion_maquina* lily_lily_creacion_maquina(char* bytes, size_t tamano, char* arquitectura, struct lily_lua_ejecucion_ctx* ctx) {
    // Preparar contexto
    ctx->lua_msg = NULL;
    ctx->estado = COD_OK;

    // Sin arquitectura no hay nada que hacer
    if (arquitectura == NULL) {
        ctx->estado = COD_LILY_CPU_NO_ESPECIFICADO;
        return NULL;
    }

    // Cargar y preparar entorno de Lua
    lua_State* L = lily_lua_entorno_preparar(&ctx->estado, ctx->fun_mensaje);
    if (ctx->estado != COD_OK) {
        return NULL;
    }
    lily_lua_int_preparar(L);
    lily_lua_entorno_ejecucion_preparar(L, &ctx->estado);
    struct lily_lily_archivo* archivo_arquitectura = ctx->fun_abrir_archivo(arquitectura, 0, (int*) &ctx->estado);
    if (archivo_arquitectura == NULL) {
        ctx->estado = COD_LILY_SIN_ESQUEMA;
        lua_close(L);
        return NULL;
    }
    // El contexto de la ejecución se queda con la copia del mensaje de Lua, si lo hay
    char* lua_msg = NULL;
    lily_lua_cpu_cargar(L, archivo_arquitectura->archivo, &ctx->estado, &lua_msg);
    ctx->lua_msg = lua_msg;
    ctx->fun_cerrar_archivo(archivo_arquitectura);
    if (ctx->estado != COD_OK) {
        return NULL;
    }
    // Preparar máquina
    struct lily_lua_ejecucion_maquina* maquina = lily_lua_ejecucion_ini(L, ctx);
    if (maquina == NULL) {
        lua_close(L);
        return NULL;
    }
    lily_lua_ejecucion_arrancar(maquina, (uint8_t*) bytes, tamano, ctx);
    if (ctx->estado != COD_OK) {
        lily_lua_ejecucion_fin(maquina); // Cierra también la sesión de Lua
        return NULL;
    }
    return maquina;
}

void lily_lily_ejecutar_instruccion(void* maquina, struct lily_lua_ejecucion_ctx* ctx) {
    lily_lua_ejecucion_ejecutar(maquina, ctx);
}

void lily_lily_destruccion_maquina(struct lily_lua_ejecucion_maquina* maquina, struct lily_lua_ejecucion_ctx* ctx) {
    lily_lua_ejecucion_fin(maquina);
    if (ctx != NULL) {
        // Es la copia que reservó lily_lily_creacion_maquina, aunque el campo sea const
        free((char*) ctx->lua_msg);
        ctx->lua_msg = NULL;
    }
}
