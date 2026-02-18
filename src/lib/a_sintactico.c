#include "../lib/a_sintactico.h"

static unsigned int lily_a_sintactico_precedencia(const struct lily_a_lexico_simbolo* operador) {
    // Todos son asociativos siniestros excepto los operadores de precedencia nivel 3
    switch (operador->subtipo) {
        case SIMB_PARENTESIS_AP:
        case SIMB_PARENTESIS_CI:
        case SIMB_DESPLAZAMIENTO_AP:
        case SIMB_DESPLAZAMIENTO_CI:
        case OP_MIEMBRO:
            return 1; // Asociativo siniestro
        case OP_BIT_NOT:
        case OP_LOG_NEG:
            //case sizeof
            return 2; // Asociativo diestro
        case OP_MULTI:
        case OP_DIV:
        case OP_MODULO:
            return 3; // Asociativo siniestro
        case OP_SUMA:
        case OP_RESTA:
            return 4; // Asociativo siniestro
        case OP_DESP_IZQ:
        case OP_DESP_DER:
            return 5;
        case OP_MENOR_QUE:
        case OP_MAYOR_QUE:
        case OP_MENOR_IGUAL:
        case OP_MAYOR_IGUAL:
            return 6;
        case OP_IGUAL:
        case OP_DIF:
            return 7;
        case OP_BIT_AND:
            return 8;
        case OP_BIT_XOR:
            return 9;
        case OP_BIT_OR:
            return 10;
        case OP_LOG_AND:
            return 11;
        case OP_LOG_OR:
            return 12;
        case SIMB_SEPARADOR:
            return 13;
    }
}

static struct lily_a_sintactico_ctx lily_a_sintactico_modo_instruccion(struct lily_lde_lde* simbolos, struct lily_lde_nodo** nodo, struct lily_a_sintactico_instruccion* instruccion) {
    struct lily_a_sintactico_ctx ctx = {
        .codigo = COD_OK,
        .ultimo = NULL,
    };
    struct lily_lde_lde* exp_simbolos = lily_lde_create();
    if (exp_simbolos == NULL) {
        ctx.codigo = COD_MALLOC_FALLO;
        return ctx;
    }
    struct lily_a_lexico_simbolo* simbolo = (*nodo)->valor;
    // Guardar todos los símbolos de esa línea
    while (simbolo != NULL && simbolo->linea == instruccion->simbolo->linea) {
        *nodo = (*nodo)->posterior;
        simbolo = (*nodo)->valor;
    }

    return ctx;
}

struct lily_a_sintactico_ctx lily_a_sintactico(struct lily_lde_lde* simbolos, struct lily_lde_lde *ast) {
    struct lily_a_sintactico_ctx ctx = {
        .codigo = COD_OK,
        .ultimo = NULL,
    };
    // Primera validación: \a simbolos no debe estar vacío
    if (simbolos->tamano == 0) {
        ctx.ultimo = NULL;
        ctx.codigo = COD_A_SINTACTICO_SIN_SIMBOLOS;
        return ctx;
    }

    /* CPU debe ser la primera instrucción si no se ha determinado aún la arquitectura a utilizar
    if (simbolo->tipo != SIMB_DIRECTIVA || simbolo->subtipo != DIR_CPU) {
        ctx.codigo = COD_A_SINTACTICO_CPU_NO_ESPECIFICADO;
        ctx.ultimo = simbolo;
        return ctx;
    }
    nodo = nodo->posterior;
    if (nodo == NULL) {
        ctx.codigo = COD_A_SINTACTICO_FIN_INESPERADO;
        ctx.ultimo = simbolo;
        return ctx;
    }
    simbolo = (struct lily_a_lexico_simbolo*) nodo->valor;
    if (simbolo->tipo != SIMB_CADENA_NUL) {
        ctx.codigo = COD_A_SINTACTICO_NO_CADENA_NUL;
        ctx.ultimo = simbolo;
        return ctx;
    }
    nodo = nodo->posterior;
    log_debug_gen("a_sintactico: cpu es \"%s\"", (char*) simbolo->valor);*/
    // Iterar para cada línea, mientras haya nodos
    struct lily_lde_nodo* nodo = lily_lde_get(simbolos, 0);
    do {
        // Empezamos a procesar una línea: preparamos todo
        struct lily_a_lexico_simbolo *simbolo = (struct lily_a_lexico_simbolo *) nodo->valor;
        struct lily_a_sintactico_instruccion *instruccion = lily_a_sintactico_instruccion_create();
        struct lily_lde_nodo* nodo_viejo; //< Para eliminaciones de \a simbolos
        if (instruccion == NULL) {
            ctx.codigo = COD_MALLOC_FALLO;
            ctx.ultimo = NULL;
            return ctx;
        }
        size_t linea_actual = simbolo->linea;

        // Primero, vemos si hay etiqueta
        if (simbolo->tipo == SIMB_ETI) {
            log_debug_gen("a_sintactico (%lu): +eti '%s'", simbolo->linea SEP (char*) simbolo->valor);
            instruccion->etiqueta = simbolo;
            nodo_viejo = nodo;
            nodo = nodo->posterior;
            lily_lde_remove_node(simbolos, nodo_viejo);
        }
        if (nodo == NULL) return ctx; // Si la etiqueta está al final de todo, salimos
        simbolo = nodo->valor;
        // Si no, vemos si seguimos en la misma línea
        if (simbolo->linea > linea_actual) {
            if (lily_lde_insert(simbolos, lily_lde_size(simbolos), instruccion) == NULL) {
                ctx.codigo = COD_MALLOC_FALLO;
                ctx.ultimo = NULL;
                return ctx;
            }
            continue;
        }
        // Si no, ya seguimos jsjsjs, hay cosas en la línea

        // Ahora vemos si lo siguiente es mnemónico o directiva
        if (simbolo->tipo == SIMB_MNEMO || simbolo->tipo == SIMB_DIRECTIVA) {
            log_debug_gen("a_sintactico (%lu): +%s '%s'", simbolo->linea SEP (simbolo->tipo == SIMB_MNEMO)?"mnemo":"directiva" SEP (char*) simbolo->valor);
            instruccion->simbolo = simbolo;
            nodo_viejo = nodo;
            nodo = nodo->posterior;
            lily_lde_remove_node(simbolos, nodo_viejo);
            ctx = lily_a_sintactico_modo_instruccion(simbolos, &nodo, instruccion);
            if (ctx.codigo != COD_OK) return ctx;
            lily_lde_insert(ast, lily_lde_size(ast), instruccion);
        }
        else if (simbolo->tipo == SIMB_ETI) {
            ctx.codigo = COD_A_SINTACTICO_ETIQUETA_MULTIPLE_LINEA;
            ctx.ultimo = simbolo;
            return ctx;
        }
        else {
            ctx.codigo = COD_A_SINTACTICO_SIMBOLO_INICIAL_INVALIDO;
            ctx.ultimo = simbolo;
            return ctx;
        }
    } while (nodo->posterior != NULL);
    log_debug_gen("sizeof(simbolos)=%lu", lily_lde_size(simbolos));
    return ctx;
}