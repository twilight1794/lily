#include "../lib/a_sintactico.h"

/**
 * Modo para procesar un mnemónico o directiva para el análisis sintáctico
 * @param [in,out] simbolos Listado de símbolos
 * @param [in,out] nodo Nodo desde el cual empezar a procesar en este modo
 * @þaram [in,out] instruccion Objeto para almacenar la instrucción procesada
 * @param [out] ctx Estado de la ejecución al momento de salir de la función, si no es \c NULL
 * @return Contexto del estado de la operación
 */
static void lily_a_sintactico_modo_instruccion(struct lily_lde_lde* simbolos, struct lily_lde_nodo** nodo, struct lily_simbolo_instruccion* instruccion, struct lily_ctx* ctx);

static void lily_a_sintactico_modo_instruccion(struct lily_lde_lde* simbolos, struct lily_lde_nodo** nodo, struct lily_simbolo_instruccion* instruccion, struct lily_ctx* ctx) {
    struct lily_lde_lde* pila_simbolos = lily_lde_create();
    if (pila_simbolos == NULL) {
        ctx->codigo = COD_MALLOC_FALLO;
        return;
    }
    struct lily_lde_nodo* nodo_viejo;
    struct lily_simbolo_simbolo* simbolo = (*nodo == NULL)?NULL:((*nodo)->valor);
    struct lily_simbolo_simbolo* o2 = NULL;
    struct lily_lde_lde* lista_desplazamiento = NULL;
    struct lily_lde_lde* pila_desplazamiento = NULL;
    // Guardar todos los símbolos de esa línea
    // Algoritmo shunting-yard
    while (*nodo != NULL && simbolo->linea == instruccion->simbolo->linea) {
        char *simb_cad = lily_simbolo_simbolo_print(simbolo);
        log_debug_gen("a_sintactico_rpn: %s", simb_cad);
        free(simb_cad);
        switch (simbolo->tipo) {
            case SIMB_OBJETO:
            case SIMB_NUMERO:
            case SIMB_VARIABLE:
            case SIMB_CADENA_NUL:
            case SIMB_CADENA_SIMPLE:
                lily_lde_insert(lista_actual, lily_lde_size(lista_actual), simbolo);
                // FIX: validar NULL
                nodo_viejo = *nodo;
                *nodo = (*nodo)->posterior;
                if (*nodo != NULL) simbolo = (*nodo)->valor;
                lily_lde_remove_node(simbolos, nodo_viejo);
                break;
            case SIMB_FUNCION:
                lily_lde_insert(pila_actual, lily_lde_size(pila_simbolos), *nodo);
                *nodo = (*nodo)->posterior;
                if (*nodo != NULL) simbolo = (*nodo)->valor;
                break;
            case SIMB_OPERADOR:
                while (
                    (pila_actual->final != NULL && (
                            ((struct lily_simbolo_simbolo*) ((struct lily_lde_nodo*) pila_actual->final->valor)->valor)->tipo != SIMB_PARENTESIS_AP ||
                            ((struct lily_simbolo_simbolo*) ((struct lily_lde_nodo*) pila_actual->final->valor)->valor)->tipo != SIMB_DESPLAZAMIENTO_AP)) &&
                    (
                        (lily_simbolo_precedencia((struct lily_simbolo_simbolo*) ((struct lily_lde_nodo*) pila_actual->final->valor)->valor) > lily_simbolo_precedencia(simbolo)) ||
                        ((lily_simbolo_precedencia((struct lily_simbolo_simbolo*) ((struct lily_lde_nodo*) pila_actual->final->valor)->valor) == lily_simbolo_precedencia(simbolo)) && (simbolo->subtipo == OP_BIT_NOT || simbolo->subtipo == OP_LOG_NEG))
                    )
                ) {
                    lily_lde_insert(lista_actual, lily_lde_size(lista_actual), ((struct lily_lde_nodo*) pila_actual->final->valor)->valor);
                    // FIX: validar NULL
                    lily_lde_remove_node(simbolos, pila_actual->final->valor);
                    lily_lde_remove_node(pila_actual, pila_actual->final);
                }
                lily_lde_insert(pila_actual, lily_lde_size(pila_actual), *nodo);
                *nodo = (*nodo)->posterior;
                if (*nodo != NULL) simbolo = (*nodo)->valor;
                break;
            case SIMB_PARENTESIS_AP:
                lily_lde_insert(pila_actual, lily_lde_size(pila_actual), *nodo);
                // FIX: validar NULL
                *nodo = (*nodo)->posterior;
                if (*nodo != NULL) simbolo = (*nodo)->valor;
                break;
            case SIMB_PARENTESIS_CI:
                // FIX: revisar
                while (pila_actual->final != NULL) {
                    o2 = (struct lily_simbolo_simbolo*) ((struct lily_lde_nodo*) pila_actual->final->valor)->valor;
                    if (o2->tipo == SIMB_PARENTESIS_AP) break;
                    lily_lde_insert(lista_actual, lily_lde_size(lista_actual), o2);
                    // FIX: validar NULL
                    lily_lde_remove_node(simbolos, pila_actual->final->valor);
                    lily_lde_remove_node(pila_actual, pila_actual->final);
                }
                if (pila_actual->final == NULL) {
                    ctx->codigo = COD_A_SINTACTICO_PARENTESIS_DESBALANCEADOS;
                    ctx->ultimo = simbolo;
                    break;
                }
                free(o2);
                o2 = NULL;
                lily_lde_remove_node(simbolos, pila_actual->final->valor);
                lily_lde_remove_node(pila_actual, pila_actual->final);
                *nodo = (*nodo)->posterior;
                if (*nodo != NULL) simbolo = (*nodo)->valor;
                // Note: Para función: sería esto mismo, y averiguar si ahora hay un símbolo llamada-a-función, y moverlo a la lista
                break;
            case SIMB_DESPLAZAMIENTO_AP:
                // TODO: rechazar desplazamiento anidado
                lista_desplazamiento = lily_lde_create();
                if (lista_desplazamiento == NULL) {
                    ctx->codigo = COD_MALLOC_FALLO;
                    ctx->ultimo = simbolo;
                    break;
                }
                pila_desplazamiento = lily_lde_create();
                if (pila_desplazamiento == NULL) {
                    ctx->codigo = COD_MALLOC_FALLO;
                    ctx->ultimo = simbolo;
                    break;
                }
                lily_lde_insert(pila_actual, lily_lde_size(pila_actual), *nodo);
                *nodo = (*nodo)->posterior;
                if (*nodo != NULL) simbolo = (*nodo)->valor;
                break;
            case SIMB_DESPLAZAMIENTO_CI:
                if (lista_desplazamiento == NULL) {
                    ctx->codigo = COD_A_SINTACTICO_PARENTESIS_DESBALANCEADOS;
                    ctx->ultimo = simbolo;
                    break;
                }
                while (pila_actual->final != NULL &&
                    ((struct lily_simbolo_simbolo*) ((struct lily_lde_nodo*) pila_actual->final->valor)->valor)->tipo != SIMB_DESPLAZAMIENTO_AP
                    ) {
                    o2 = (struct lily_simbolo_simbolo*) ((struct lily_lde_nodo*) pila_actual->final->valor)->valor;
                    lily_lde_insert(lista_actual, lily_lde_size(lista_actual), o2);
                    // FIX: validar NULL
                    lily_lde_remove_node(simbolos, pila_actual->final->valor);
                    lily_lde_remove_node(pila_actual, pila_actual->final);
                    }
                if (pila_actual->final == NULL) {
                    ctx->codigo = COD_A_SINTACTICO_PARENTESIS_DESBALANCEADOS;
                    ctx->ultimo = simbolo;
                    break;
                }
                if (lista_desplazamiento->inicio == NULL) {
                    ctx->codigo = COD_A_SINTACTICO_PARENTESIS_VACIOS;
                    ctx->ultimo = simbolo;
                    break;
                }
                if (o2 == NULL)
                    o2 = ((struct lily_lde_nodo*) pila_desplazamiento->final->valor)->valor;
                o2->valor = lista_desplazamiento;
                lista_desplazamiento = NULL;
                lily_lde_insert(instruccion->params, lily_lde_size(instruccion->params), o2);
                lily_lde_remove_node(simbolos, pila_desplazamiento->final->valor);
                lily_lde_remove_node(pila_desplazamiento, pila_desplazamiento->final);
                free(pila_desplazamiento);
                pila_desplazamiento = NULL;
                o2 = NULL;
                nodo_viejo = *nodo;
                *nodo = (*nodo)->posterior;
                if (*nodo != NULL) simbolo = (*nodo)->valor;
                lily_lde_remove_node(simbolos, nodo_viejo);
                break;
            case SIMB_SEPARADOR:
                while (pila_actual->final != NULL && (
                    ((struct lily_simbolo_simbolo*) ((struct lily_lde_nodo*) pila_actual->final->valor)->valor)->tipo != SIMB_PARENTESIS_AP &&
                    ((struct lily_simbolo_simbolo*) ((struct lily_lde_nodo*) pila_actual->final->valor)->valor)->tipo != SIMB_DESPLAZAMIENTO_AP
                    )) {
                    lily_lde_insert(lista_actual, lily_lde_size(lista_actual), (struct lily_lde_nodo*) ((struct lily_lde_nodo*) pila_actual->final->valor)->valor);
                    // FIX: validar NULL
                    lily_lde_remove_node(simbolos, pila_actual->final->valor);
                    lily_lde_remove_node(pila_actual, pila_actual->final);
                }
                nodo_viejo = *nodo;
                *nodo = (*nodo)->posterior;
                if (*nodo != NULL) simbolo = (*nodo)->valor;
                lily_lde_remove_node(simbolos, nodo_viejo);
                break;
            default:
                log_debug("Si llegamos acá, es que algo anda mal");
                ctx->codigo = COD_A_SINTACTICO_SIMBOLO_IMPROCEDENTE;
                ctx->ultimo = simbolo;
                return;
        }
        if (ctx->codigo != COD_OK) break;
    }
    while (lily_lde_size(pila_simbolos)) {
        struct lily_lde_nodo* nodo_pila = (struct lily_lde_nodo*) pila_simbolos->final->valor;
        struct lily_simbolo_simbolo* simbolo_pila = nodo_pila->valor;
        if (simbolo_pila->tipo != SIMB_PARENTESIS_AP) {
            lily_lde_insert(instruccion->params, lily_lde_size(instruccion->params), simbolo_pila);
            lily_lde_remove_node(simbolos, nodo_pila);
            lily_lde_remove_node(pila_simbolos, pila_simbolos->final);
        }
    }
}

struct lily_lde_lde* lily_a_sintactico(struct lily_lde_lde* simbolos, struct lily_ctx* ctx) {
    struct lily_lde_lde* ast = lily_lde_create();
    if (ast == NULL) {
        ctx->codigo = COD_MALLOC_FALLO;
        return ast;
    }

    // Primera validación: \a simbolos no debe estar vacío
    if (simbolos->tamano == 0) {
        ctx->ultimo = NULL;
        ctx->codigo = COD_A_SINTACTICO_SIN_SIMBOLOS;
        return ast;
    }

    char* simb_cad;
    // Iterar para cada línea, mientras haya nodos
    struct lily_lde_nodo* nodo = lily_lde_get(simbolos, 0);
    do {
        // Empezamos a procesar una línea: preparamos todo
        struct lily_simbolo_simbolo *simbolo = (struct lily_simbolo_simbolo *) nodo->valor;
        struct lily_simbolo_instruccion *instruccion = lily_simbolo_instruccion_create();
        // FIX: comprobar NULL
        struct lily_lde_nodo* nodo_viejo; //< Para eliminaciones de \a simbolos
        if (instruccion == NULL) {
            ctx->codigo = COD_MALLOC_FALLO;
            ctx->ultimo = NULL;
            break;
        }
        size_t linea_actual = simbolo->linea;

        // Primero, vemos si hay etiqueta
        if (simbolo->tipo == SIMB_ETI) {
            simb_cad = lily_simbolo_simbolo_print(simbolo);
            log_debug_gen("a_sintactico: %s", simb_cad);
            free(simb_cad);
            instruccion->etiqueta = simbolo;
            nodo_viejo = nodo;
            nodo = nodo->posterior;
            lily_lde_remove_node(simbolos, nodo_viejo);
        }
        if (nodo == NULL) break; // Si la etiqueta está al final de todo, salimos
        simbolo = nodo->valor;
        // Si no, vemos si seguimos en la misma línea
        if (simbolo->linea > linea_actual) {
            if (lily_lde_insert(ast, lily_lde_size(ast), instruccion) == NULL) {
                ctx->codigo = COD_MALLOC_FALLO;
                ctx->ultimo = NULL;
                break;
            }
            simb_cad = lily_simbolo_instruccion_print(instruccion);
            log_debug_gen("a_sintactico sim: %s", simb_cad);
            free(simb_cad);
            continue;
        }
        // Si seguimos en la misma línea, ya continuamos jsjsjs, hay cosas en la línea

        // Ahora vemos si lo siguiente es mnemónico o directiva
        if (simbolo->tipo == SIMB_MNEMO || simbolo->tipo == SIMB_DIRECTIVA) {
            simb_cad = lily_simbolo_simbolo_print(simbolo);
            log_debug_gen("a_sintactico: %s", simb_cad);
            free(simb_cad);
            instruccion->simbolo = simbolo;
            nodo_viejo = nodo;
            nodo = nodo->posterior;
            lily_lde_remove_node(simbolos, nodo_viejo);
            lily_a_sintactico_modo_instruccion(simbolos, &nodo, instruccion, ctx);
            if (ctx->codigo != COD_OK) break;
            lily_lde_insert(ast, lily_lde_size(ast), instruccion);
        }
        else if (simbolo->tipo == SIMB_ETI) {
            ctx->codigo = COD_A_SINTACTICO_ETIQUETA_MULTIPLE_LINEA;
            ctx->ultimo = simbolo;
            break;
        }
        else {
            ctx->codigo = COD_A_SINTACTICO_SIMBOLO_INICIAL_INVALIDO;
            ctx->ultimo = simbolo;
            break;
        }
        simb_cad = lily_simbolo_instruccion_print(instruccion);
        log_debug_gen("a_sintactico sim: %s", simb_cad);
        free(simb_cad);
    } while (nodo != NULL);

    if (ctx->codigo != COD_OK) {
        free(ast);
        return NULL;
    };

    log_debug_gen("sizeof(simbolos)=%lu", lily_lde_size(simbolos));
    for (size_t i = 0; i < lily_lde_size(simbolos); i++) {
        simb_cad = lily_simbolo_simbolo_print(lily_lde_get(simbolos,i)->valor);
        log_debug_gen("a_sintactico rest: %s", simb_cad);
        free(simb_cad);
    }
    return ast;
}