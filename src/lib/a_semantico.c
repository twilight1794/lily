#include "a_semantico.h"

struct lily_a_semantico_ctx lily_a_semantico(struct lily_lde_lde *ast, char* bytes) {
    struct lily_a_semantico_ctx ctx = {
        .codigo = COD_OK,
        .ultimo = NULL,
    };

    struct lily_lde_nodo* nodo = lily_lde_get(ast, 0);
    struct lily_a_sintactico_instruccion *instruccion = (struct lily_a_sintactico_instruccion *) nodo->valor;

    // CPU debe ser la primera instrucción si no se ha determinado aún la arquitectura a utilizar
    // FIX: hallar el modo de comprobar si no se ha determinado ya la arquitectura
    if (instruccion->simbolo->tipo != SIMB_DIRECTIVA || instruccion->simbolo->subtipo != DIR_CPU) {
        ctx.codigo = COD_A_SEMANTICO_CPU_NO_ESPECIFICADO;
        ctx.ultimo = instruccion->simbolo;
        return ctx;
    }
/*    if (nodo == NULL) {
        ctx.codigo = COD_A_SINTACTICO_FIN_INESPERADO;
        ctx.ultimo = simbolo;
        return ctx;
    }
    simbolo = (struct lily_a_lexico_simbolo*) nodo->valor;*/
    if (!lily_lde_size(instruccion->params) || ((struct lily_a_lexico_simbolo*) instruccion->params->inicio->valor)->tipo != SIMB_CADENA_NUL) {
        ctx.codigo = COD_A_SINTACTICO_NO_CADENA_NUL;
        ctx.ultimo = instruccion->simbolo;
        return ctx;
    }
    log_debug_gen("a_semantico: cpu es \"%s\"", (char*) ((struct lily_a_lexico_simbolo*) instruccion->params->inicio->valor)->valor);
    nodo = nodo->posterior;
    //do {
    //} while (nodo->posterior != NULL);
    return ctx;
}