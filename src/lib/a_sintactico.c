#include "../lib/a_sintactico.h"

static unsigned int lily_a_sintactico_precedencia(const struct lily_a_lexico_simbolo* operador) {
    // Todos son asociativos siniestros excepto los operadores de precedencia nivel 2
    switch (operador->subtipo) {
        case SIMB_PARENTESIS_AP:
        case SIMB_PARENTESIS_CI:
        case SIMB_DESPLAZAMIENTO_AP:
        case SIMB_DESPLAZAMIENTO_CI:
        case OP_MIEMBRO:
            return 1;
        case OP_BIT_NOT:
        case OP_LOG_NEG:
            //case sizeof
            return 2; // Asociativo diestro
        case OP_MULTI:
        case OP_DIV:
        case OP_MODULO:
            return 3;
        case OP_SUMA:
        case OP_RESTA:
            return 4;
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
        default:
            return 14;
    }
}

static struct lily_a_sintactico_ctx lily_a_sintactico_modo_instruccion(struct lily_lde_lde* simbolos, struct lily_lde_nodo** nodo, struct lily_a_sintactico_instruccion* instruccion) {
    struct lily_a_sintactico_ctx ctx = {
        .codigo = COD_OK,
        .ultimo = NULL,
    };
    struct lily_lde_lde* pila_simbolos = lily_lde_create();
    if (pila_simbolos == NULL) {
        ctx.codigo = COD_MALLOC_FALLO;
        return ctx;
    }
    struct lily_a_lexico_simbolo* simbolo = (*nodo)->valor;
    struct lily_a_lexico_simbolo* o2 = NULL;
    struct lily_lde_lde* lista_desplazamiento = NULL;
    // Guardar todos los símbolos de esa línea
    // Algoritmo shunting-yard
    while (simbolo != NULL && simbolo->linea == instruccion->simbolo->linea) {
        char *simb_cad = lily_a_lexico_simbolo_print(simbolo);
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
                struct lily_lde_nodo* nodo_viejo = *nodo;
                *nodo = (*nodo)->posterior;
                simbolo = (*nodo)->valor;
                lily_lde_remove_node(simbolos, nodo_viejo);
                break;
            case SIMB_FUNCION:
                lily_lde_insert(pila_simbolos, lily_lde_size(pila_simbolos), *nodo);
                *nodo = (*nodo)->posterior;
                simbolo = (*nodo)->valor;
                break;
            case SIMB_OPERADOR:
                while (
                    (pila_simbolos->final != NULL && (
                            ((struct lily_a_lexico_simbolo*) ((struct lily_lde_nodo*) pila_simbolos->final->valor)->valor)->tipo != SIMB_PARENTESIS_AP ||
                            ((struct lily_a_lexico_simbolo*) ((struct lily_lde_nodo*) pila_simbolos->final->valor)->valor)->tipo != SIMB_DESPLAZAMIENTO_AP)) &&
                    (
                        (lily_a_sintactico_precedencia((struct lily_a_lexico_simbolo*) ((struct lily_lde_nodo*) pila_simbolos->final->valor)->valor) > lily_a_sintactico_precedencia(simbolo)) ||
                        ((lily_a_sintactico_precedencia((struct lily_a_lexico_simbolo*) ((struct lily_lde_nodo*) pila_simbolos->final->valor)->valor) == lily_a_sintactico_precedencia(simbolo)) && (simbolo->subtipo == OP_BIT_NOT || simbolo->subtipo == OP_LOG_NEG))
                    )
                ) {
                    lily_lde_insert(lista_actual, lily_lde_size(lista_actual), ((struct lily_lde_nodo*) pila_simbolos->final->valor)->valor);
                    // FIX: validar NULL
                    lily_lde_remove_node(simbolos, pila_simbolos->final->valor);
                    lily_lde_remove_node(pila_simbolos, pila_simbolos->final);
                }
                lily_lde_insert(pila_simbolos, lily_lde_size(pila_simbolos), *nodo);
                *nodo = (*nodo)->posterior;
                simbolo = (*nodo)->valor;
                break;
            case SIMB_PARENTESIS_AP:
                lily_lde_insert(pila_simbolos, lily_lde_size(pila_simbolos), *nodo);
                *nodo = (*nodo)->posterior;
                simbolo = (*nodo)->valor;
                break;
            case SIMB_PARENTESIS_CI:
                // FIX: revisar
                while (pila_simbolos->final != NULL &&
                    ((struct lily_a_lexico_simbolo*) ((struct lily_lde_nodo*) pila_simbolos->final->valor)->valor)->tipo != SIMB_PARENTESIS_AP
                    ) {
                    o2 = (struct lily_a_lexico_simbolo*) ((struct lily_lde_nodo*) pila_simbolos->final->valor)->valor;
                    lily_lde_insert(lista_actual, lily_lde_size(lista_actual), o2);
                    // FIX: validar NULL
                    lily_lde_remove_node(simbolos, pila_simbolos->final->valor);
                    lily_lde_remove_node(pila_simbolos, pila_simbolos->final);
                }
                if (pila_simbolos->final == NULL) {
                    ctx.codigo = COD_A_SINTACTICO_PARENTESIS_DESBALANCEADOS;
                    ctx.ultimo = simbolo;
                    return ctx;
                }
                free(o2);
                o2 = NULL;
                lily_lde_remove_node(simbolos, pila_simbolos->final->valor);
                lily_lde_remove_node(pila_simbolos, pila_simbolos->final);
                *nodo = (*nodo)->posterior;
                simbolo = (*nodo)->valor;
                // Note: Para función: sería esto mismo, y averiguar si ahora hay un símbolo llamada-a-función, y moverlo a la lista
                break;
            case SIMB_DESPLAZAMIENTO_AP:
                lista_desplazamiento = lily_lde_create();
                if (lista_desplazamiento == NULL) {
                    ctx.codigo = COD_MALLOC_FALLO;
                    ctx.ultimo = simbolo;
                    return ctx;
                }
                lily_lde_insert(pila_simbolos, lily_lde_size(pila_simbolos), *nodo);
                *nodo = (*nodo)->posterior;
                simbolo = (*nodo)->valor;
                break;
            case SIMB_DESPLAZAMIENTO_CI:
                o2 = (pila_simbolos->final == NULL)?NULL: ((struct lily_a_lexico_simbolo*) ((struct lily_lde_nodo*) pila_simbolos->final->valor)->valor);
                if (o2 == NULL || o2->tipo != SIMB_DESPLAZAMIENTO_AP) {
                    ctx.codigo = COD_A_SINTACTICO_PARENTESIS_DESBALANCEADOS;
                    ctx.ultimo = simbolo;
                    return ctx;
                }
                if (lista_desplazamiento->inicio == NULL) {
                    ctx.codigo = COD_A_SINTACTICO_PARENTESIS_VACIOS;
                    ctx.ultimo = simbolo;
                }
                o2->valor = lista_desplazamiento;
                lista_desplazamiento = NULL;
                lily_lde_insert(lista_actual, lily_lde_size(lista_actual), o2);
                lily_lde_remove_node(simbolos, pila_simbolos->final->valor);
                lily_lde_remove_node(pila_simbolos, pila_simbolos->final);
                *nodo = (*nodo)->posterior;
                simbolo = (*nodo)->valor;
                break;
            case SIMB_SEPARADOR:
                while (pila_simbolos->final != NULL && (
                    ((struct lily_a_lexico_simbolo*) ((struct lily_lde_nodo*) pila_simbolos->final->valor)->valor)->tipo != SIMB_PARENTESIS_AP &&
                    ((struct lily_a_lexico_simbolo*) ((struct lily_lde_nodo*) pila_simbolos->final->valor)->valor)->tipo != SIMB_DESPLAZAMIENTO_AP
                    )) {
                    lily_lde_insert(lista_actual, lily_lde_size(lista_actual), (struct lily_lde_nodo*) ((struct lily_lde_nodo*) pila_simbolos->final->valor)->valor);
                    // FIX: validar NULL
                    lily_lde_remove_node(simbolos, pila_simbolos->final->valor);
                    lily_lde_remove_node(pila_simbolos, pila_simbolos->final);
                }
                *nodo = (*nodo)->posterior;
                simbolo = (*nodo)->valor;
                break;
            default:
                log_debug("Si llegamos acá, es que algo anda mal");
                return ctx;
        }
    }
    while (pila_simbolos->final != NULL) {
        struct lily_lde_nodo* nodo_pila = (struct lily_lde_nodo*) pila_simbolos->final->valor;
        if (((struct lily_a_lexico_simbolo*) ((struct lily_lde_nodo*) nodo_pila->valor)->valor)->tipo != SIMB_PARENTESIS_AP) {
            lily_lde_insert(instruccion->params, lily_lde_size(instruccion->params), nodo_pila->valor);
            lily_lde_remove_node(simbolos, nodo_pila->valor);
            lily_lde_remove_node(pila_simbolos, nodo_pila);
        }
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
    char* simb_cad;
    // Iterar para cada línea, mientras haya nodos
    struct lily_lde_nodo* nodo = lily_lde_get(simbolos, 0);
    do {
        // Empezamos a procesar una línea: preparamos todo
        struct lily_a_lexico_simbolo *simbolo = (struct lily_a_lexico_simbolo *) nodo->valor;
        struct lily_a_sintactico_instruccion *instruccion = lily_a_sintactico_instruccion_create();
        // FIX: comprobar NULL
        struct lily_lde_nodo* nodo_viejo; //< Para eliminaciones de \a simbolos
        if (instruccion == NULL) {
            ctx.codigo = COD_MALLOC_FALLO;
            ctx.ultimo = NULL;
            return ctx;
        }
        size_t linea_actual = simbolo->linea;

        // Primero, vemos si hay etiqueta
        if (simbolo->tipo == SIMB_ETI) {
            simb_cad = lily_a_lexico_simbolo_print(simbolo);
            log_debug_gen("a_sintactico: %s", simb_cad);
            free(simb_cad);
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
            simb_cad = lily_a_lexico_simbolo_print(simbolo);
            log_debug_gen("a_sintactico: %s", simb_cad);
            free(simb_cad);
            instruccion->simbolo = simbolo;
            nodo_viejo = nodo;
            nodo = nodo->posterior;
            lily_lde_remove_node(simbolos, nodo_viejo);
            ctx = lily_a_sintactico_modo_instruccion(simbolos, &nodo, instruccion);
            if (ctx.codigo != COD_OK) return ctx;
            lily_lde_insert(ast, lily_lde_size(ast), instruccion);
            struct lily_lde_nodo* simb_nodo = instruccion->params->inicio;
            while (simb_nodo != NULL) {
                simb_cad = lily_a_lexico_simbolo_print(simb_nodo->valor);
                log_debug_gen("a_sintactico_comp: %s", simb_cad);
                free(simb_cad);
                struct lily_a_lexico_simbolo* simb_simb = simb_nodo->valor;
                if (simb_simb->tipo == SIMB_DESPLAZAMIENTO_AP) {
                    for (struct lily_lde_nodo* simb_it = ((struct lily_lde_lde*)simb_simb->valor)->inicio; simb_it != NULL; simb_it = simb_it->posterior) {
                        simb_cad = lily_a_lexico_simbolo_print(simb_it->valor);
                        log_debug_gen("a_sintactico_desp: %s", simb_cad);
                        free(simb_cad);
                    }
                }
                simb_nodo = simb_nodo->posterior;
            }
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