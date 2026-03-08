#include "a_semantico.h"

/**
 * Añade un nuevo identificador a la lista de identificadores conocidos
 * @param identificadores Tabla de símbolos
 * @param identificador Identificador a añadir
 * @param valor Entero asociado al identificador
 * @param es_const Si el identificador será constante o variable
 * @param [out] ctx Contexto del estado de la operación
 */
static void lily_a_semantico_anad_identificador(struct lily_dict_dict* identificadores, char* identificador, union lily_simbolo_numero* valor, bool es_const, struct lily_ctx* ctx);

/**
 * Reduce una lista de expresiones, aplicando las operaciones y reemplazos pertinentes
 * @param instruccion Directiva a procesar
 * @param identificadores Tabla de símbolos
 * @param [out] ctx Contexto del estado de la operación
 * @return Si la lista de símbolos pudo ser reducida al máximo
 */
static bool lily_a_semantico_reducir(struct lily_simbolo_instruccion* instruccion, struct lily_dict_dict* identificadores, struct lily_ctx* ctx);

/**
 * Ensambla una directiva
 * @param instruccion Directiva a procesar
 * @param identificadores Tabla de símbolos
 * @param pc Contador de instrucción
 * @param [out] ctx Contexto del estado de la operación
 */
static void lily_a_semantico_directiva(struct lily_simbolo_instruccion* instruccion, struct lily_dict_dict* identificadores, const size_t* pc, struct lily_ctx* ctx);

static void lily_a_semantico_anad_identificador(struct lily_dict_dict* identificadores, char* identificador, union lily_simbolo_numero* valor, bool es_const, struct lily_ctx* ctx) {
    struct lily_dict_nodo* nodo = lily_dict_get(identificadores, identificador);
    if (nodo != NULL) {
        if (((struct lily_simbolo_identificador*) nodo->valor)->es_const)
            ctx->codigo = COD_A_SEMANTICO_REDEF_CONSTANTE;
        else
            ctx->codigo = COD_A_SEMANTICO_REDEF_VARIABLE;
        return;
    }
    struct lily_simbolo_identificador* val = lily_simbolo_identificador_create();
    if (val == NULL) ctx->codigo = COD_MALLOC_FALLO;
    else {
        val->valor = valor;
        val->es_const = es_const;
        nodo = lily_dict_insert(identificadores, identificador, val, NULL);
        if (nodo == NULL) ctx->codigo = COD_MALLOC_FALLO;
    }
}

static bool lily_a_semantico_reducir(struct lily_simbolo_instruccion* instruccion, struct lily_dict_dict* identificadores, struct lily_ctx* ctx) {
    struct lily_lde_nodo* nodo = instruccion->params->inicio;
    struct lily_lde_nodo* nodo_viejo;
    struct lily_dict_nodo* dict_nodo; // Puntero para consultas a identificadores
    struct lily_simbolo_simbolo* nuevo; // Puntero para símbolos recién creados
    struct lily_simbolo_simbolo* op1; // Punteros para operandos en uso
    struct lily_simbolo_simbolo* op2;
    char* cad; // Puntero para manejo de cadena
    bool reducido = true;
    size_t i = 0;
    while (nodo != NULL) {
        struct lily_simbolo_simbolo* simbolo = nodo->valor;
        switch (simbolo->tipo) {
            case SIMB_MNEMO:
            case SIMB_DIRECTIVA:
            case SIMB_ETI:
            case SIMB_FUNCION:
                // <debug>
                log_fatal("a_semantico: esto no debería pasar");
                ctx->codigo = COD_NO_IMPLEMENTADO;
                // </debug>
                break;
            case SIMB_OBJETO:
            case SIMB_NUMERO:
                // Estos objetos ya son terminales
                nodo = nodo->posterior;
                i++;
                break;
            case SIMB_VARIABLE:
                // Ver si existe la variable
                dict_nodo = lily_dict_get(identificadores, simbolo->valor);
                if (dict_nodo == NULL) {
                    // No existe, no podemos reducir
                    reducido = false;
                    break;
                }
                // Redefinimos tipo de símbolo
                simbolo->tipo = SIMB_NUMERO;
                free(simbolo->valor);
                simbolo->valor = malloc(sizeof(union lily_simbolo_numero));
                if (simbolo->valor == NULL) {
                    ctx->codigo = COD_MALLOC_FALLO;
                    ctx->ultimo = simbolo;
                    return false;
                }
                ((union lily_simbolo_numero*) simbolo->valor)->positivo = ((union lily_simbolo_numero*) dict_nodo->valor)->positivo;
                i++;
                break;
            case SIMB_CADENA_SIMPLE:
            case SIMB_CADENA_NUL:
                // Limpiar símbolo viejo
                cad = simbolo->valor; // Salvamos por un momento esto
                nodo_viejo = nodo;
                nodo = nodo->posterior;
                lily_lde_remove_node(instruccion->params, nodo_viejo);

                for (size_t j = 0; j < strlen(cad); j++) {
                    nuevo = lily_simbolo_simbolo_create();
                    if (nuevo == NULL) {
                        ctx->codigo = COD_MALLOC_FALLO;
                        ctx->ultimo = instruccion->simbolo;
                        return false;
                    }
                    nuevo->tipo = SIMB_NUMERO;
                    nuevo->valor = malloc(sizeof(union lily_simbolo_numero));
                    if (nuevo->valor == NULL) {
                        ctx->codigo = COD_MALLOC_FALLO;
                        ctx->ultimo = instruccion->simbolo;
                        return false;
                    }
                    ((union lily_simbolo_numero*) nuevo->valor)->positivo = ((char*) simbolo->valor)[j];
                    lily_lde_insert(instruccion->params, i, nuevo);
                    i++;
                }
                if (simbolo->tipo == SIMB_CADENA_NUL) {
                    nuevo = lily_simbolo_simbolo_create();
                    if (nuevo == NULL) {
                        ctx->codigo = COD_MALLOC_FALLO;
                        ctx->ultimo = instruccion->simbolo;
                        return false;
                    }
                    nuevo->tipo = SIMB_NUMERO;
                    nuevo->valor = malloc(sizeof(union lily_simbolo_numero));
                    if (nuevo->valor == NULL) {
                        ctx->codigo = COD_MALLOC_FALLO;
                        ctx->ultimo = instruccion->simbolo;
                        return false;
                    }
                    ((union lily_simbolo_numero*) nuevo->valor)->positivo = 0;
                    lily_lde_insert(instruccion->params, i, nuevo);
                    i++;
                }
                // Limpiar símbolo viejo II
                free(simbolo->valor);
                free(simbolo);
                i++;
                break;
            case SIMB_OPERADOR:
#define es(op) simbolo->subtipo == op
                nodo_viejo = nodo;
                nodo = nodo->posterior;
                if (lily_simbolo_aridad(simbolo->subtipo) == 1) {
                    op1 = (struct lily_simbolo_simbolo*) nodo_viejo->anterior->valor;
                    if (op1->tipo != SIMB_NUMERO) {
                        ctx->codigo = COD_A_SEMANTICO_OPERANDO_OBJETO;
                        ctx->ultimo = op1;
                        break;
                    }
                    // Operar!
                    if (es(OP_BIT_NOT))
                        ((union lily_simbolo_numero*) op1->valor)->positivo = ~(((union lily_simbolo_numero*) op1->valor)->positivo);
                    else if (es(OP_LOG_NEG))
                       ((union lily_simbolo_numero*) op1->valor)->positivo = !(((union lily_simbolo_numero*) op1->valor)->positivo);

                }
                else if (lily_simbolo_aridad(simbolo->subtipo) == 2) {
                    op2 = (struct lily_simbolo_simbolo*) nodo_viejo->anterior->valor;
                    op1 = (struct lily_simbolo_simbolo*) nodo_viejo->anterior->anterior->valor;
                    if (op2->tipo != SIMB_NUMERO || op1->tipo != SIMB_NUMERO) {
                        ctx->codigo = COD_A_SEMANTICO_OPERANDO_OBJETO;
                        ctx->ultimo = (op2->tipo != SIMB_NUMERO)?op2:op1;
                        break;
                    }
                    // Operar!
                    if (es(OP_SUMA))
                        ((union lily_simbolo_numero*) op1->valor)->positivo += ((union lily_simbolo_numero*) op2->valor)->positivo;
                    if (es(OP_RESTA))
                        ((union lily_simbolo_numero*) op1->valor)->positivo -= ((union lily_simbolo_numero*) op2->valor)->positivo;
                    if (es(OP_MULTI))
                        ((union lily_simbolo_numero*) op1->valor)->positivo *= ((union lily_simbolo_numero*) op2->valor)->positivo;
                    if (es(OP_DIV))
                        ((union lily_simbolo_numero*) op1->valor)->positivo /= ((union lily_simbolo_numero*) op2->valor)->positivo;
                    if (es(OP_MODULO))
                        ((union lily_simbolo_numero*) op1->valor)->positivo %= ((union lily_simbolo_numero*) op2->valor)->positivo;
                    if (es(OP_BIT_AND))
                        ((union lily_simbolo_numero*) op1->valor)->positivo &= ((union lily_simbolo_numero*) op2->valor)->positivo;
                    if (es(OP_BIT_OR))
                        ((union lily_simbolo_numero*) op1->valor)->positivo |= ((union lily_simbolo_numero*) op2->valor)->positivo;
                    if (es(OP_BIT_XOR))
                        ((union lily_simbolo_numero*) op1->valor)->positivo ^= ((union lily_simbolo_numero*) op2->valor)->positivo;
                    if (es(OP_LOG_AND))
                        ((union lily_simbolo_numero*) op1->valor)->positivo = ((union lily_simbolo_numero*) op1->valor)->positivo && ((union lily_simbolo_numero*) op2->valor)->positivo;
                    if (es(OP_LOG_OR))
                        ((union lily_simbolo_numero*) op1->valor)->positivo = ((union lily_simbolo_numero*) op1->valor)->positivo || ((union lily_simbolo_numero*) op2->valor)->positivo;
                    if (es(OP_DESP_IZQ))
                        ((union lily_simbolo_numero*) op1->valor)->positivo <<= ((union lily_simbolo_numero*) op2->valor)->positivo;
                    if (es(OP_DESP_DER))
                        ((union lily_simbolo_numero*) op1->valor)->positivo >>= ((union lily_simbolo_numero*) op2->valor)->positivo;
                    if (es(OP_MENOR_QUE))
                        ((union lily_simbolo_numero*) op1->valor)->positivo = ((union lily_simbolo_numero*) op1->valor)->positivo < ((union lily_simbolo_numero*) op2->valor)->positivo;
                    if (es(OP_MAYOR_QUE))
                        ((union lily_simbolo_numero*) op1->valor)->positivo = ((union lily_simbolo_numero*) op1->valor)->positivo > ((union lily_simbolo_numero*) op2->valor)->positivo;
                    if (es(OP_MENOR_IGUAL))
                        ((union lily_simbolo_numero*) op1->valor)->positivo = ((union lily_simbolo_numero*) op1->valor)->positivo <= ((union lily_simbolo_numero*) op2->valor)->positivo;
                    if (es(OP_MAYOR_IGUAL))
                        ((union lily_simbolo_numero*) op1->valor)->positivo = ((union lily_simbolo_numero*) op1->valor)->positivo >= ((union lily_simbolo_numero*) op2->valor)->positivo;
                    if (es(OP_IGUAL))
                        ((union lily_simbolo_numero*) op1->valor)->positivo = ((union lily_simbolo_numero*) op1->valor)->positivo == ((union lily_simbolo_numero*) op2->valor)->positivo;
                    if (es(OP_DIF))
                        ((union lily_simbolo_numero*) op1->valor)->positivo = ((union lily_simbolo_numero*) op1->valor)->positivo != ((union lily_simbolo_numero*) op2->valor)->positivo;
                    // Borrar op2
                    free(op2->valor);
                    free(op2);
                    lily_lde_remove_node(instruccion->params, nodo_viejo->anterior);
                }
                // Borrar operador
                free(((struct lily_simbolo_simbolo*) nodo_viejo->valor)->valor);
                free(nodo_viejo->valor);
                lily_lde_remove_node(instruccion->params, nodo_viejo);
                i++;
                break;
        }
        if (ctx->codigo != COD_OK) {
            return false;
        }
        if (!reducido) {
            log_debug_gen("a_semantico: no se ha reducido completamente %s", lily_simbolo_instruccion_print(instruccion));
            break;
        }
        ++i;
    }
    if (reducido) {
        log_debug_gen("a_semantico: se ha reducido %s", lily_simbolo_instruccion_print(instruccion));
    }
    return reducido;
}

static void lily_a_semantico_directiva(struct lily_simbolo_instruccion* instruccion, struct lily_dict_dict* identificadores, const size_t* pc, struct lily_ctx* ctx) {
    struct lily_simbolo_simbolo* simbolo_param;

    union lily_simbolo_numero* pc_numero = (union lily_simbolo_numero*) malloc(sizeof(union lily_simbolo_numero));
    if (pc_numero == NULL) {
        ctx->codigo = COD_MALLOC_FALLO;
        ctx->ultimo = instruccion->simbolo;
        return;
    }
    pc_numero->positivo = *pc;

    switch (instruccion->simbolo->subtipo) {
        case DIR_DB:
            if (instruccion->etiqueta == NULL) {
                ctx->codigo = COD_A_SEMANTICO_INSTRUCCION_SIN_ETI;
                ctx->ultimo = instruccion->simbolo;
                break;
            }
            instruccion->tam_bytes = lily_lde_size(instruccion->params);
            instruccion->bytes = (uint8_t*) calloc(lily_lde_size(instruccion->params), sizeof(uint8_t));
            if (instruccion->bytes == NULL) {
                ctx->codigo = COD_MALLOC_FALLO;
                ctx->ultimo = instruccion->simbolo;
                break;
            }
            for (size_t i = 0; i < lily_lde_size(instruccion->params); i++) {
                struct lily_simbolo_simbolo* simbolo = lily_lde_get(instruccion->params, i)->valor;
                lily_nums_codificar_num(instruccion->bytes, simbolo->valor, sizeof(uint8_t), simbolo->signo, true);
            }
            lily_a_semantico_anad_identificador(identificadores, instruccion->etiqueta->valor, pc_numero, true, ctx);
            break;
        case DIR_DWL:
        case DIR_DWM:
            if (instruccion->etiqueta == NULL) {
                ctx->codigo = COD_A_SEMANTICO_INSTRUCCION_SIN_ETI;
                ctx->ultimo = instruccion->simbolo;
                break;
            }
            instruccion->tam_bytes = lily_lde_size(instruccion->params) * sizeof(uint16_t);
            instruccion->bytes = (uint8_t*) malloc(instruccion->tam_bytes);
            if (instruccion->bytes == NULL) {
                ctx->codigo = COD_MALLOC_FALLO;
                ctx->ultimo = instruccion->simbolo;
                break;
            }
            for (size_t i = 0; i < lily_lde_size(instruccion->params); i++) {
                struct lily_simbolo_simbolo* simbolo = lily_lde_get(instruccion->params, i)->valor;
                lily_nums_codificar_num(instruccion->bytes, simbolo->valor, sizeof(uint16_t), false, instruccion->simbolo->subtipo == DIR_DWL);
            }
            lily_a_semantico_anad_identificador(identificadores, instruccion->etiqueta->valor, pc_numero, true, ctx);
            break;
        case DIR_DDL:
        case DIR_DDM:
            if (instruccion->etiqueta == NULL) {
                ctx->codigo = COD_A_SEMANTICO_INSTRUCCION_SIN_ETI;
                ctx->ultimo = instruccion->simbolo;
                break;
            }
            instruccion->tam_bytes = lily_lde_size(instruccion->params) * sizeof(uint32_t);
            instruccion->bytes = (uint8_t*) malloc(instruccion->tam_bytes);
            if (instruccion->bytes == NULL) {
                ctx->codigo = COD_MALLOC_FALLO;
                ctx->ultimo = instruccion->simbolo;
                break;
            }
            for (size_t i = 0; i < lily_lde_size(instruccion->params); i++) {
                struct lily_simbolo_simbolo* simbolo = lily_lde_get(instruccion->params, i)->valor;
                lily_nums_codificar_num(instruccion->bytes, simbolo->valor, sizeof(uint32_t), false, instruccion->simbolo->subtipo == DIR_DDL);
            }
            lily_a_semantico_anad_identificador(identificadores, instruccion->etiqueta->valor, pc_numero, true, ctx);
            break;
        case DIR_DQL:
        case DIR_DQM:
            if (instruccion->etiqueta == NULL) {
                ctx->codigo = COD_A_SEMANTICO_INSTRUCCION_SIN_ETI;
                ctx->ultimo = instruccion->simbolo;
                break;
            }
            instruccion->tam_bytes = lily_lde_size(instruccion->params) * sizeof(uint64_t);
            instruccion->bytes = (uint8_t*) malloc(instruccion->tam_bytes);
            if (instruccion->bytes == NULL) {
                ctx->codigo = COD_MALLOC_FALLO;
                ctx->ultimo = instruccion->simbolo;
                break;
            }
            for (size_t i = 0; i < lily_lde_size(instruccion->params); i++) {
                struct lily_simbolo_simbolo* simbolo = lily_lde_get(instruccion->params, i)->valor;
                lily_nums_codificar_num(instruccion->bytes, simbolo->valor, sizeof(uint64_t), false, instruccion->simbolo->subtipo == DIR_DQL);
            }
            lily_a_semantico_anad_identificador(identificadores, instruccion->etiqueta->valor, pc_numero, true, ctx);
            break;
        case DIR_DR:
        case DIR_DRD:
            ctx->codigo = COD_NO_IMPLEMENTADO;
            ctx->ultimo = instruccion->simbolo;
            break;
        case DIR_DFS:
            if (instruccion->etiqueta == NULL) {
                ctx->codigo = COD_A_SEMANTICO_INSTRUCCION_SIN_ETI;
                ctx->ultimo = instruccion->simbolo;
                break;
            }
            if (lily_lde_size(instruccion->params) != 1) {
                ctx->codigo = COD_A_SEMANTICO_DFS_MULTIPLES_PARAMS;
                ctx->ultimo = instruccion->simbolo;
                break;
            }
            simbolo_param = (lily_lde_get(instruccion->params, 0)->valor);
            if (simbolo_param->tipo != SIMB_NUMERO) {
                ctx->codigo = COD_A_SEMANTICO_DFS_PARAM_NO_ENTERO;
                ctx->ultimo = simbolo_param;
                break;
            }
            instruccion->tam_bytes = ((union lily_simbolo_numero*) simbolo_param->valor)->positivo;
            instruccion->bytes = (uint8_t*) calloc(instruccion->tam_bytes, sizeof(uint8_t));
            lily_a_semantico_anad_identificador(identificadores, instruccion->etiqueta->valor, pc_numero, true, ctx);
            break;
        case DIR_CONST:
        case DIR_VAR:
            simbolo_param = (lily_lde_get(instruccion->params, 0)->valor);
            if (lily_lde_size(instruccion->params) != 1) {
                ctx->codigo = COD_A_SEMANTICO_CONST_VAR_MULTIPLES_PARAMS;
                ctx->ultimo = instruccion->simbolo;
            }
            if (simbolo_param->tipo != SIMB_NUMERO) {
                ctx->codigo = COD_A_SEMANTICO_DFS_PARAM_NO_ENTERO;
                ctx->ultimo = simbolo_param;
                break;
            }
            if (instruccion->etiqueta == NULL) { // TODO: ¿pool de etiquetas?
                ctx->codigo = COD_A_SEMANTICO_INSTRUCCION_SIN_ETI;
                ctx->ultimo = instruccion->simbolo;
                break;
            }
            free(pc_numero);
            instruccion->direccion = *pc;
            instruccion->tam_bytes = 0;
            instruccion->bytes = (uint8_t*) calloc(instruccion->tam_bytes, sizeof(uint8_t));
            lily_a_semantico_anad_identificador(identificadores, instruccion->etiqueta->valor, ((union lily_simbolo_numero*) simbolo_param->valor), (instruccion->simbolo->subtipo == DIR_CONST), ctx);
            break;
        case DIR_IF:
        case DIR_IFDEF:
        case DIR_IFNDEF:
        case DIR_ELSE:
        case DIR_ELIFDEF:
        case DIR_ELIFNDEF:
        case DIR_WHILE:
        case DIR_LOOP:
        case DIR_INC:
            free(pc_numero);
            ctx->codigo = COD_NO_IMPLEMENTADO;
            ctx->ultimo = instruccion->simbolo;
            break;
        case DIR_CPU:
            free(pc_numero);
            ctx->codigo = COD_A_SEMANTICO_CPU_MULTIPLE;
            ctx->ultimo = instruccion->simbolo;
            break;
        case DIR_ORG:
        case DIR_STOP:
        case DIR_STRUCT:
        case DIR_UNION:
        case DIR_MACRO:
        case DIR_PROC:
        case DIR_END:
            free(pc_numero);
            ctx->codigo = COD_NO_IMPLEMENTADO;
            ctx->ultimo = instruccion->simbolo;
            break;
    }
}

char* lily_a_semantico_obt_arquitectura_declarada(struct lily_lde_lde* ast, struct lily_ctx* ctx) {
    struct lily_lde_nodo* nodo = lily_lde_get(ast, 0);
    struct lily_simbolo_instruccion *instruccion = nodo->valor;
    char* arquitectura = NULL;

    // CPU debe ser la primera instrucción si no se ha determinado aún la arquitectura a utilizar
    if (instruccion->simbolo->tipo == SIMB_DIRECTIVA && instruccion->simbolo->subtipo == DIR_CPU) {
        // Vemos si tiene parámetro cadena
        if (!lily_lde_size(instruccion->params) || ((struct lily_simbolo_simbolo*) instruccion->params->inicio->valor)->tipo != SIMB_CADENA_NUL) {
            ctx->codigo = COD_A_SINTACTICO_NO_CADENA_NUL;
            ctx->ultimo = instruccion->simbolo;
            return NULL;
        }
        if (lily_lde_size(instruccion->params) > 1) {
            ctx->codigo = COD_A_SEMANTICO_CPU_MULTIPLES_PARAMS;
            ctx->ultimo = instruccion->simbolo;
            return NULL;
        }
        arquitectura = (char*) ((struct lily_simbolo_simbolo*) instruccion->params->inicio->valor)->valor;
        // Liberar nodo
        free(instruccion->params->inicio->valor);
        lily_lde_remove_node(instruccion->params, instruccion->params->inicio);
        free(instruccion->params);
        free(instruccion);
        lily_lde_remove_node(ast, nodo);
    }
    return arquitectura;
}

uint8_t* lily_a_semantico(struct lily_lde_lde *ast, lua_State* L, size_t pc_inicial, size_t* tam, struct lily_ctx* ctx) {
    uint8_t* bytes = NULL;

    // Diccionario para identificadores
    struct lily_dict_dict* identificadores = lily_dict_create();
    if (identificadores == NULL) {
        ctx->codigo = COD_MALLOC_FALLO;
        return bytes;
    }

    // Contador de localidad
    uint64_t* pc = malloc(sizeof(size_t));
    if (pc == NULL) {
        ctx->codigo = COD_MALLOC_FALLO;
        return bytes;
    }
    struct lily_dict_nodo* nodo_dict = lily_dict_insert(identificadores, "pc", pc, NULL);
    if (nodo_dict == NULL) {
        ctx->codigo = COD_MALLOC_FALLO;
        return bytes;
    }
    size_t iteraciones = 0;
    size_t num_pendientes;

    struct lily_lde_nodo* nodo = lily_lde_get(ast, 0);
    struct lily_simbolo_instruccion *instruccion = nodo->valor;

    do {
        num_pendientes = 0;
        *pc = pc_inicial;
        nodo = lily_lde_get(ast, 0);
        ++iteraciones;
        log_debug_gen(_("a_semantico: Iteración %lu"), iteraciones);
        log_debug_gen(_("a_semantico: PC vale 0x%02lX"), *pc);

        // Por cada instrucción...
        while ( nodo != NULL) {
            instruccion = nodo->valor;
            if (instruccion->simbolo != NULL && !esta_definido(instruccion)) {
                // Si el símbolo no ha sido traducido, intentaremos traducirlo
                log_debug_gen(_("a_semantico: procesando %s."), lily_simbolo_instruccion_print(instruccion));
                bool reducido = lily_a_semantico_reducir(instruccion, identificadores, ctx);
                if (reducido) {
                    if (instruccion->simbolo->tipo == SIMB_DIRECTIVA) {
                        // Si ya está reducido, y es directiva...
                        lily_a_semantico_directiva(instruccion, identificadores, pc, ctx);
                        if (ctx->codigo != COD_OK) {
                            ctx->ultimo = instruccion->simbolo;
                            break;
                        }
                    }
                    else if (instruccion->simbolo->tipo == SIMB_MNEMO) {
                        // Si ya está reducido, y es mnemónico...
                        lily_lua_cpu_ensamblar(L, instruccion, ctx);
                        if (ctx->codigo != COD_OK) {
                            ctx->ultimo = instruccion->simbolo;
                            break;
                        }
                    }
                    // Imprimir array
                    char* cad = malloc(sizeof(char)*(instruccion->tam_bytes*6) -1);
                    if (cad == NULL) {
                        ctx->codigo = COD_MALLOC_FALLO;
                        break;
                    }
                    for (size_t i = 0; i < instruccion->tam_bytes; i++) {
                        sprintf(cad+(i*6), "0x%02X", instruccion->bytes[i]);
                        if (i+1 < instruccion->tam_bytes) sprintf(cad+(i*6)+4, ", ");
                    }
                    log_debug_gen("a_semantico: %s generó %lu bytes (%s)", lily_simbolo_simbolo_print(instruccion->simbolo) SEP instruccion->tam_bytes SEP cad);
                    free(cad);
                }
                else {
                    log_debug_gen("a_semantico: %s pospuesto", lily_simbolo_simbolo_print(instruccion->simbolo))
                    num_pendientes++;
                }
            }
            // Si hay etiqueta, establecemos su valor
            // FIX: esto redefinirá etiquetas definidas por otros medios
            if (instruccion->etiqueta != NULL) {
                struct lily_dict_nodo* etiqueta_nodo = lily_dict_get(identificadores, instruccion->etiqueta->valor);
                union lily_simbolo_numero* pc_numero;
                if (etiqueta_nodo != NULL) {
                    // Si ya la conocíamos, trabajamos con ese valor
                    pc_numero = ((struct lily_simbolo_identificador*) etiqueta_nodo->valor)->valor;
                    pc_numero->positivo = *pc;
                } else {
                    // Si no, lo añadimos
                    pc_numero = (union lily_simbolo_numero*) malloc(sizeof(union lily_simbolo_numero));
                    if (pc_numero == NULL) {
                        ctx->codigo = COD_MALLOC_FALLO;
                        ctx->ultimo = instruccion->etiqueta;
                        break;
                    }
                    pc_numero->positivo = *pc;
                    lily_a_semantico_anad_identificador(identificadores, instruccion->etiqueta->valor, pc_numero, true, ctx);
                    if (ctx->codigo != COD_OK) break;
                }
            }
            instruccion->direccion = *pc;
            // Actualizamos PC
            *pc += instruccion->tam_bytes;
            log_debug_gen("a_semantico: PC vale ahora 0x%02lX", *pc);
            nodo = nodo->posterior;
        }
        // ¿Salimos por un error?
        if (ctx->codigo != COD_OK) {
            *tam = 0;
            return bytes;
        }
    } while (num_pendientes > 0);

    // Cuando acabemos, crear el array...
    *tam = sizeof(uint8_t)*(*pc);
    bytes = malloc(*tam);
    if (bytes == NULL) {
        ctx->codigo = COD_MALLOC_FALLO;
        return bytes;
    }
    // ...escribir en array
    size_t desp = 0;
    nodo = lily_lde_get(ast, 0);
    while (nodo != NULL) {
        instruccion = (struct lily_simbolo_instruccion *) nodo->valor;
        memcpy(bytes+desp, instruccion->bytes, instruccion->tam_bytes);
        desp += instruccion->tam_bytes;
        nodo = nodo->posterior;
    }
    return bytes;
}