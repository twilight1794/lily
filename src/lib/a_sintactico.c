#include "../lib/a_sintactico.h"

enum lily_error lily_a_sintactico(struct lily_lde_lde* simbolos, struct lily_lde_lde* ast, struct lily_a_sintactico_ctx* ctx) {
    // FIX: validar que \a simbolos no esté vacío desde antes
    // CPU debe ser la primera instrucción si no se ha determinado aún la arquitectura a utilizar
    struct lily_lde_nodo* nodo = lily_lde_get(simbolos, 0);
    struct lily_a_lexico_simbolo* simbolo = (struct lily_a_lexico_simbolo*) nodo->valor;
    if (simbolo->tipo != SIMB_DIRECTIVA || strcmp((char*) simbolo->valor, "CPU")) return COD_A_SINTACTICO_CPU_NO_ESPECIFICADO;
    nodo = nodo->posterior;
    if (nodo == NULL) {
        if (ctx != NULL) {
            ctx->codigo = COD_A_SINTACTICO_FIN_INESPERADO;
            ctx->ultimo = simbolo;
        }
        return COD_A_SINTACTICO_FIN_INESPERADO;
    }
    simbolo = (struct lily_a_lexico_simbolo*) nodo->valor;
    if (simbolo->tipo !=  SIMB_CADENA_NUL) {
        if (ctx != NULL) {
            ctx->codigo = COD_A_SINTACTICO_NO_CADENA_NUL;
            ctx->ultimo = simbolo;
        }
        return COD_A_SINTACTICO_NO_CADENA_NUL;
    }
    nodo = nodo->posterior;
    log_debug_gen("a_sintactico: cpu es \"%s\"", (char*) simbolo->valor);
    // Iterar por el resto de nodos, si los hubiera
    while (nodo->posterior != NULL) {
        nodo = nodo->posterior;
    }
    return COD_OK;
}