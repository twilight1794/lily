#include "a_semantico.h"

/**
 * Añade un nuevo identificador a la lista de identificadores conocidos
 * @param identificadores Tabla de símbolos
 * @param identificador Identificador a añadir
 * @param valor Entero asociado al identificador
 * @param es_const Si el identificador será constante o variable
 * @param [out] ctx Contexto del estado de la operación
 */
static struct lily_simbolo_identificador* lily_a_semantico_anad_identificador(struct lily_dict_dict* identificadores, char* identificador, union lily_simbolo_numero* valor, bool es_const, enum lily_estado* estado, struct lily_a_semantico_ctx* ctx);

/**
 * Reduce una lista de expresiones, aplicando las operaciones y reemplazos pertinentes
 * @param simbolo_principal Directiva o mnemónico a procesar
 * @param params Lista que contiene los símbolos a reducir
 * @param identificadores Tabla de símbolos
 * @param [out] ctx Contexto del estado de la operación
 * @return Tipo de reducción llevada a cabo sobre la lista de símbolos
 */
static enum lily_a_semantico_reduccion lily_a_semantico_reducir(struct lily_simbolo_simbolo* simbolo_principal, struct lily_lde_lde* params, struct lily_dict_dict* identificadores, enum lily_estado* estado, struct lily_a_semantico_ctx* ctx);

/**
 * Ensambla una directiva
 * @param instruccion Directiva a procesar
 * @param identificadores Tabla de símbolos
 * @param pc Contador de instrucción
 * @param [out] ctx Contexto del estado de la operación
 */
static void lily_a_semantico_directiva(struct lily_simbolo_instruccion* instruccion, struct lily_dict_dict* identificadores, const union lily_simbolo_numero* pc, f_mensajes_ptr enviar_mensaje, enum lily_estado* estado, struct lily_a_semantico_ctx* ctx);

static struct lily_simbolo_identificador* lily_a_semantico_anad_identificador(struct lily_dict_dict* identificadores, char* identificador, union lily_simbolo_numero* valor, bool es_const, enum lily_estado* estado, struct lily_a_semantico_ctx* ctx) {
    struct lily_dict_nodo* nodo = lily_dict_get(identificadores, identificador);
    if (nodo != NULL) {
        if (((struct lily_simbolo_identificador*) nodo->valor)->es_const)
            *estado = COD_A_SEMANTICO_REDEF_CONSTANTE;
        else
            *estado = COD_A_SEMANTICO_REDEF_VARIABLE;
        return NULL;
    }
    struct lily_simbolo_identificador* val = lily_simbolo_identificador_create();
    if (val == NULL) *estado = COD_MALLOC_FALLO;
    else {
        val->valor = valor;
        val->es_const = es_const;
        nodo = lily_dict_insert(identificadores, identificador, val, NULL);
        if (nodo == NULL) *estado = COD_MALLOC_FALLO;
    }
    return val;
}

static enum lily_a_semantico_reduccion lily_a_semantico_reducir(struct lily_simbolo_simbolo* simbolo_principal, struct lily_lde_lde* params, struct lily_dict_dict* identificadores, enum lily_estado* estado, struct lily_a_semantico_ctx* ctx) {
    struct lily_lde_nodo* nodo = params->inicio;
    struct lily_lde_nodo* nodo_viejo;
    struct lily_dict_nodo* dict_nodo; // Puntero para consultas a identificadores
    struct lily_simbolo_simbolo* nuevo; // Puntero para símbolos recién creados
    struct lily_simbolo_simbolo* op1; // Punteros para operandos en uso
    struct lily_simbolo_simbolo* op2;
    char* cad; // Puntero para manejo de cadena
    enum lily_a_semantico_reduccion reducido = REDUCCION_DEFINITIVA;
    size_t i = 0;
    while (nodo != NULL) {
        struct lily_simbolo_simbolo* simbolo = nodo->valor;
        switch (simbolo->tipo) {
            case SIMB_FUNCION:
                *estado = COD_NO_IMPLEMENTADO;
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
                    reducido = SIN_REDUCCION;
                    break;
                }
                struct lily_simbolo_identificador* identificador_obj = dict_nodo->valor;
                if (!(identificador_obj->definido)) reducido = REDUCCION_TEMPORAL;
                // Redefinimos tipo de símbolo
                simbolo->tipo = SIMB_NUMERO;
                free(simbolo->valor);
                simbolo->valor = malloc(sizeof(union lily_simbolo_numero));
                if (simbolo->valor == NULL) {
                    *estado = COD_MALLOC_FALLO;
                    ctx->ultimo = simbolo;
                    return false;
                }
                ((union lily_simbolo_numero*) simbolo->valor)->positivo = ((union lily_simbolo_numero*) identificador_obj->valor)->positivo;
                i++;
                break;
            case SIMB_CADENA_SIMPLE:
            case SIMB_CADENA_NUL:
                // Limpiar símbolo viejo
                cad = simbolo->valor; // Salvamos por un momento esto
                nodo_viejo = nodo;
                nodo = nodo->posterior;
                lily_lde_remove_node(params, nodo_viejo);

                for (size_t j = 0; j < strlen(cad); j++) {
                    nuevo = lily_simbolo_simbolo_create();
                    if (nuevo == NULL) {
                        *estado = COD_MALLOC_FALLO;
                        ctx->ultimo = simbolo_principal;
                        return false;
                    }
                    nuevo->tipo = SIMB_NUMERO;
                    nuevo->valor = malloc(sizeof(union lily_simbolo_numero));
                    if (nuevo->valor == NULL) {
                        *estado = COD_MALLOC_FALLO;
                        ctx->ultimo = simbolo_principal;
                        return false;
                    }
                    ((union lily_simbolo_numero*) nuevo->valor)->positivo = ((char*) simbolo->valor)[j];
                    lily_lde_insert(params, i, nuevo);
                    i++;
                }
                if (simbolo->tipo == SIMB_CADENA_NUL) {
                    nuevo = lily_simbolo_simbolo_create();
                    if (nuevo == NULL) {
                        *estado = COD_MALLOC_FALLO;
                        ctx->ultimo = simbolo_principal;
                        return false;
                    }
                    nuevo->tipo = SIMB_NUMERO;
                    nuevo->valor = malloc(sizeof(union lily_simbolo_numero));
                    if (nuevo->valor == NULL) {
                        *estado = COD_MALLOC_FALLO;
                        ctx->ultimo = simbolo_principal;
                        return false;
                    }
                    ((union lily_simbolo_numero*) nuevo->valor)->positivo = 0;
                    lily_lde_insert(params, i, nuevo);
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
                if (lily_simbolo_aridad(simbolo->subtipo) == 0 && simbolo->subtipo == SIMB_DESPLAZAMIENTO_AP) {
                    lily_a_semantico_reducir(simbolo_principal, (struct lily_lde_lde*) simbolo->valor, identificadores, estado, ctx);
                    i++;
                    break; // <- Este operador no será eliminado como los demás
                }
                else if (lily_simbolo_aridad(simbolo->subtipo) == 1) {
                    op1 = (struct lily_simbolo_simbolo*) nodo_viejo->anterior->valor;
                    if (op1->tipo != SIMB_NUMERO) {
                        *estado = COD_A_SEMANTICO_OPERANDO_OBJETO;
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
                        *estado = COD_A_SEMANTICO_OPERANDO_OBJETO;
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
                    lily_lde_remove_node(params, nodo_viejo->anterior);
                }
                // Borrar operador
                free(((struct lily_simbolo_simbolo*) nodo_viejo->valor)->valor);
                free(nodo_viejo->valor);
                lily_lde_remove_node(params, nodo_viejo);
                i++;
                break;
        }
        if (*estado != COD_OK) {
            reducido = SIN_REDUCCION;
            break;
        }
        // Las variables no definidas se resuelven fuera de aquí
        if (!reducido && simbolo->tipo == SIMB_VARIABLE) break;
        ++i;
    }
    return reducido;
}

static void lily_a_semantico_directiva(struct lily_simbolo_instruccion* instruccion, struct lily_dict_dict* identificadores, const union lily_simbolo_numero* pc, f_mensajes_ptr enviar_mensaje, enum lily_estado* estado, struct lily_a_semantico_ctx* ctx) {
    char* msg_buf;
    struct lily_simbolo_simbolo* simbolo_param;
    union lily_simbolo_numero* pc_numero = (union lily_simbolo_numero*) malloc(sizeof(union lily_simbolo_numero));
    if (pc_numero == NULL) {
        *estado = COD_MALLOC_FALLO;
        ctx->ultimo = instruccion->simbolo;
        return;
    }
    pc_numero->positivo = pc->positivo;

    switch (instruccion->simbolo->subtipo) {
        case DIR_DB:
            if (instruccion->etiqueta == NULL) {
                enviar_mensaje(LILY_MENSAJE_TADVERTENCIA, COD_A_SEMANTICO_INSTRUCCION_SIN_ETI, "a_semantico_directiva", instruccion->simbolo);
                // FIX: Por ahora, todas las advertencias son ignorables
            }
            instruccion->tam_bytes = lily_lde_size(instruccion->params);
            instruccion->bytes = (uint8_t*) calloc(lily_lde_size(instruccion->params), sizeof(uint8_t));
            if (instruccion->bytes == NULL) {
                *estado = COD_MALLOC_FALLO;
                ctx->ultimo = instruccion->simbolo;
                break;
            }
            for (size_t i = 0; i < lily_lde_size(instruccion->params_tmp); i++) {
                struct lily_simbolo_simbolo* simbolo = lily_lde_get(instruccion->params_tmp, i)->valor;
                lily_nums_codificar_num(instruccion->bytes, simbolo->valor, sizeof(uint8_t), 2, true);
            }
            if (instruccion->etiqueta != NULL)
                lily_a_semantico_anad_identificador(identificadores, instruccion->etiqueta->valor, pc_numero, true, estado, ctx);
            break;
        case DIR_DWL:
        case DIR_DWM:
            if (instruccion->etiqueta == NULL)
                enviar_mensaje(LILY_MENSAJE_TADVERTENCIA, COD_A_SEMANTICO_INSTRUCCION_SIN_ETI, "a_semantico_directiva", instruccion->simbolo);
            instruccion->tam_bytes = lily_lde_size(instruccion->params_tmp) * sizeof(uint16_t);
            instruccion->bytes = (uint8_t*) malloc(instruccion->tam_bytes);
            if (instruccion->bytes == NULL) {
                *estado = COD_MALLOC_FALLO;
                ctx->ultimo = instruccion->simbolo;
                break;
            }
            for (size_t i = 0; i < lily_lde_size(instruccion->params_tmp); i++) {
                struct lily_simbolo_simbolo* simbolo = lily_lde_get(instruccion->params_tmp, i)->valor;
                lily_nums_codificar_num(instruccion->bytes, simbolo->valor, sizeof(uint16_t), 2, instruccion->simbolo->subtipo == DIR_DWL);
            }
            if (instruccion->etiqueta != NULL)
                lily_a_semantico_anad_identificador(identificadores, instruccion->etiqueta->valor, pc_numero, true, estado, ctx);
            break;
        case DIR_DDL:
        case DIR_DDM:
            if (instruccion->etiqueta == NULL)
                enviar_mensaje(LILY_MENSAJE_TADVERTENCIA, COD_A_SEMANTICO_INSTRUCCION_SIN_ETI, "a_semantico_directiva", instruccion->simbolo);
            instruccion->tam_bytes = lily_lde_size(instruccion->params_tmp) * sizeof(uint32_t);
            instruccion->bytes = (uint8_t*) malloc(instruccion->tam_bytes);
            if (instruccion->bytes == NULL) {
                *estado = COD_MALLOC_FALLO;
                ctx->ultimo = instruccion->simbolo;
                break;
            }
            for (size_t i = 0; i < lily_lde_size(instruccion->params_tmp); i++) {
                struct lily_simbolo_simbolo* simbolo = lily_lde_get(instruccion->params_tmp, i)->valor;
                lily_nums_codificar_num(instruccion->bytes, simbolo->valor, sizeof(uint32_t), 2, instruccion->simbolo->subtipo == DIR_DDL);
            }
            if (instruccion->etiqueta != NULL)
                lily_a_semantico_anad_identificador(identificadores, instruccion->etiqueta->valor, pc_numero, true, estado, ctx);
            break;
        case DIR_DQL:
        case DIR_DQM:
            if (instruccion->etiqueta == NULL)
                enviar_mensaje(LILY_MENSAJE_TADVERTENCIA, COD_A_SEMANTICO_INSTRUCCION_SIN_ETI, "a_semantico_directiva", instruccion->simbolo);
            instruccion->tam_bytes = lily_lde_size(instruccion->params_tmp) * sizeof(uint64_t);
            instruccion->bytes = (uint8_t*) malloc(instruccion->tam_bytes);
            if (instruccion->bytes == NULL) {
                *estado = COD_MALLOC_FALLO;
                ctx->ultimo = instruccion->simbolo;
                break;
            }
            for (size_t i = 0; i < lily_lde_size(instruccion->params_tmp); i++) {
                struct lily_simbolo_simbolo* simbolo = lily_lde_get(instruccion->params, i)->valor;
                lily_nums_codificar_num(instruccion->bytes, simbolo->valor, sizeof(uint64_t), 2, instruccion->simbolo->subtipo == DIR_DQL);
            }
            if (instruccion->etiqueta != NULL)
                lily_a_semantico_anad_identificador(identificadores, instruccion->etiqueta->valor, pc_numero, true, estado, ctx);
            break;
        case DIR_DR:
        case DIR_DRD:
            *estado = COD_NO_IMPLEMENTADO;
            ctx->ultimo = instruccion->simbolo;
            break;
        case DIR_DFS:
            if (instruccion->etiqueta == NULL)
                enviar_mensaje(LILY_MENSAJE_TADVERTENCIA, COD_A_SEMANTICO_INSTRUCCION_SIN_ETI, "a_semantico_directiva", instruccion->simbolo);
            if (lily_lde_size(instruccion->params_tmp) != 1) {
                *estado = COD_A_SEMANTICO_DFS_MULTIPLES_PARAMS;
                ctx->ultimo = instruccion->simbolo;
                break;
            }
            simbolo_param = (lily_lde_get(instruccion->params_tmp, 0)->valor);
            if (simbolo_param->tipo != SIMB_NUMERO) {
                *estado = COD_A_SEMANTICO_DFS_PARAM_NO_ENTERO;
                ctx->ultimo = simbolo_param;
                break;
            }
            instruccion->tam_bytes = ((union lily_simbolo_numero*) simbolo_param->valor)->positivo;
            instruccion->bytes = (uint8_t*) calloc(instruccion->tam_bytes, sizeof(uint8_t));
            if (instruccion->etiqueta != NULL)
                lily_a_semantico_anad_identificador(identificadores, instruccion->etiqueta->valor, pc_numero, true, estado, ctx);
            break;
        case DIR_CONST:
        case DIR_VAR:
            simbolo_param = (lily_lde_get(instruccion->params_tmp, 0)->valor);
            if (lily_lde_size(instruccion->params) != 1) {
                *estado = COD_A_SEMANTICO_CONST_VAR_MULTIPLES_PARAMS;
                ctx->ultimo = instruccion->simbolo;
            }
            if (simbolo_param->tipo != SIMB_NUMERO) {
                *estado = COD_A_SEMANTICO_DFS_PARAM_NO_ENTERO;
                ctx->ultimo = simbolo_param;
                break;
            }
            if (instruccion->etiqueta == NULL) { // TODO: ¿pool de etiquetas?
                *estado = COD_A_SEMANTICO_INSTRUCCION_SIN_ETI;
                ctx->ultimo = instruccion->simbolo;
                break;
            }
            instruccion->direccion = pc->positivo;
            instruccion->tam_bytes = 8; // Estamos guardando un long int
            instruccion->bytes = (uint8_t*) calloc(8, sizeof(uint8_t));
            lily_a_semantico_anad_identificador(identificadores, instruccion->etiqueta->valor, ((union lily_simbolo_numero*) simbolo_param->valor), (instruccion->simbolo->subtipo == DIR_CONST), estado, ctx);
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
            *estado = COD_NO_IMPLEMENTADO;
            ctx->ultimo = instruccion->simbolo;
            break;
        case DIR_CPU:
            *estado = COD_A_SEMANTICO_CPU_MULTIPLE;
            ctx->ultimo = instruccion->simbolo;
            break;
        case DIR_ORG:
        case DIR_STOP:
        case DIR_STRUCT:
        case DIR_UNION:
        case DIR_MACRO:
        case DIR_PROC:
        case DIR_END:
            *estado = COD_NO_IMPLEMENTADO;
            ctx->ultimo = instruccion->simbolo;
            break;
    }
    free(pc_numero);
}

#define cctx ((struct lily_a_semantico_ctx*) (*ctx))
char* lily_a_semantico_obt_arquitectura_declarada(struct lily_lde_lde* ast, enum lily_estado* estado, void** ctx) {
    *ctx = calloc(1, sizeof(struct lily_a_semantico_ctx));
    struct lily_lde_nodo* nodo = lily_lde_get(ast, 0);
    struct lily_simbolo_instruccion *instruccion = nodo->valor;
    char* arquitectura = NULL;

    // CPU debe ser la primera instrucción si no se ha determinado aún la arquitectura a utilizar
    if (instruccion->simbolo->tipo == SIMB_DIRECTIVA && instruccion->simbolo->subtipo == DIR_CPU) {
        // Vemos si tiene parámetro cadena
        if (!lily_lde_size(instruccion->params) || ((struct lily_simbolo_simbolo*) instruccion->params->inicio->valor)->tipo != SIMB_CADENA_NUL) {
            *estado = COD_A_SINTACTICO_NO_CADENA_NUL;
            cctx->ultimo = instruccion->simbolo;
            return NULL;
        }
        if (lily_lde_size(instruccion->params) > 1) {
            *estado = COD_A_SEMANTICO_CPU_MULTIPLES_PARAMS;
            cctx->ultimo = instruccion->simbolo;
            return NULL;
        }
        arquitectura = (char*) ((struct lily_simbolo_simbolo*) instruccion->params->inicio->valor)->valor;
        // Liberar nodo
        free(instruccion->params->inicio->valor);
        lily_lde_remove_node(instruccion->params, instruccion->params->inicio);
        free(instruccion->params);
        free(instruccion->params_tmp);
        free(instruccion);
        lily_lde_remove_node(ast, nodo);
    }
    free(*ctx);
    return arquitectura;
}

uint8_t* lily_a_semantico(struct lily_lde_lde *ast, lua_State* L, size_t pc_inicial, size_t* tam, struct lily_dict_dict* opciones, f_mensajes_ptr enviar_mensaje, enum lily_estado* estado, void** ctx) {
    *ctx = calloc(1, sizeof(struct lily_a_semantico_ctx));
    char* msg_buf;
    uint8_t* bytes = NULL;

    // Diccionario para identificadores
    struct lily_dict_dict* identificadores = lily_dict_create();
    if (identificadores == NULL) {
        *estado = COD_MALLOC_FALLO;
        return bytes;
    }

    // Contador de localidad
    union lily_simbolo_numero* pc_obj = (union lily_simbolo_numero*) malloc(sizeof(union lily_simbolo_numero));
    if (pc_obj == NULL) {
        *estado = COD_MALLOC_FALLO;
        return bytes;
    }
    struct lily_simbolo_identificador* pc_identificador = lily_a_semantico_anad_identificador(identificadores, "pc", pc_obj, true, estado, *ctx);
    if (pc_identificador == NULL) {
        *estado = COD_MALLOC_FALLO;
        return bytes;
    }

    // Iteraciones máximas
    size_t it_max_op = 10;
    struct lily_dict_nodo* it_max_nodo = lily_dict_get(opciones, "iteraciones_max");
    if (it_max_nodo != NULL) {
        it_max_op = atoi(it_max_nodo->valor);
        free(it_max_nodo);
    }

    size_t iteraciones = 0;
    size_t num_indeterminadas;
    size_t num_temporales;

    struct lily_lde_nodo* nodo = lily_lde_get(ast, 0);
    struct lily_simbolo_instruccion *instruccion = nodo->valor;

    do {
        num_indeterminadas = 0;
        num_temporales = 0;
        pc_obj->positivo = pc_inicial;
        pc_identificador->definido = true;
        nodo = lily_lde_get(ast, 0);
        ++iteraciones;
        msg_buf = d_printf("Iteración %lu", iteraciones);
        enviar_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "a_semantico", msg_buf);
        free(msg_buf);
        msg_buf = d_printf("PC vale 0x%02zX", pc_inicial);
        enviar_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "a_semantico", msg_buf);
        free(msg_buf);

        // Por cada instrucción...
        while (nodo != NULL) {
            instruccion = nodo->valor;
            if (instruccion->simbolo != NULL && !instruccion->reducido) {
                // Si el símbolo no ha sido traducido, intentaremos traducirlo
                msg_buf = d_printf("Procesando %s", lily_simbolo_instruccion_print(instruccion));
                enviar_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "a_semantico", msg_buf);
                free(msg_buf);
                // Limpiar lista temporal
                if (lily_lde_size(instruccion->params_tmp)) {
                    lily_lde_foreachm(instruccion->params_tmp, nodo_limp, nodo_limp_sig) {
                        struct lily_simbolo_simbolo* simbolo_limp = nodo_limp->valor;
                        // Si es desplazamiento, aparte hay qué borrar su contenido
                        if (simbolo_limp->subtipo == SIMB_DESPLAZAMIENTO_AP) {
                            lily_lde_foreachm(simbolo_limp->valor, nodo_limp_d, nodo_limp_d_sig) {
                                struct lily_simbolo_simbolo* simbolo_limp_d = nodo_limp_d->valor;
                                free(simbolo_limp_d->valor);
                                free(simbolo_limp_d);
                                lily_lde_remove_node(nodo_limp->valor, nodo_limp_d);
                            }
                        }
                        else {
                            free(simbolo_limp->valor);
                            free(simbolo_limp);
                            lily_lde_remove_node(instruccion->params_tmp, nodo_limp);
                        }
                    }
                }
                // Copiar símbolos a lista temporal
                lily_lde_foreach(instruccion->params, nodo_orig) {
                    struct lily_simbolo_simbolo* simbolo_orig = nodo_orig->valor;
                    struct lily_simbolo_simbolo* simbolo_copia = lily_simbolo_simbolo_clone(simbolo_orig);
                    if (simbolo_copia == NULL) {
                        *estado = COD_MALLOC_FALLO;
                        break;
                    }
                    if (lily_lde_push(instruccion->params_tmp, simbolo_copia) == NULL) {
                        // FIX: borrar resto de elementos ya añadidos a la lista
                        free(simbolo_copia);
                        return NULL;
                    }
                }
                if (*estado != COD_OK) {
                    break;
                }
                // Reducir
                enum lily_a_semantico_reduccion reducido = lily_a_semantico_reducir(instruccion->simbolo, instruccion->params_tmp, identificadores, estado, *ctx);
                // Analizar reducción
                if (reducido != SIN_REDUCCION) {
                    msg_buf = d_printf("se ha reducido %s%s", (reducido == REDUCCION_TEMPORAL)?"temporalmente ":"", lily_simbolo_instruccion_print(instruccion));
                    enviar_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "a_semantico", msg_buf);
                    free(msg_buf);
                    // Bandera de actualización temporal
                    if (reducido == REDUCCION_TEMPORAL) num_temporales++;
                    // Marcar instruccion como reducida definitivamente si es el caso
                    if (reducido == REDUCCION_DEFINITIVA) instruccion->reducido = true;
                    // Si ya está reducida, al menos temporalmente...
                    if (instruccion->simbolo->tipo == SIMB_DIRECTIVA) {
                        // ...y es directiva...
                        lily_a_semantico_directiva(instruccion, identificadores, pc_obj, enviar_mensaje, estado, *ctx);
                        if (*estado != COD_OK) {
                            cctx->ultimo = instruccion->simbolo;
                            break;
                        }
                    }
                    else if (instruccion->simbolo->tipo == SIMB_MNEMO) {
                        // ...y es mnemónico...
                        lily_lua_cpu_ensamblar(L, instruccion, enviar_mensaje, estado, ctx);
                        if (*estado != COD_OK) {
                            cctx->ultimo = instruccion->simbolo;
                            break;
                        }
                    }
                    // Guardar dirección de memoria
                    instruccion->direccion = pc_obj->positivo;
                    // Imprimir array
                    char* cad = malloc(sizeof(char)*(instruccion->tam_bytes*6) -1);
                    if (cad == NULL) {
                        *estado = COD_MALLOC_FALLO;
                        break;
                    }
                    for (size_t i = 0; i < instruccion->tam_bytes; i++) {
                        sprintf(cad+(i*6), "0x%02X", instruccion->bytes[i]);
                        if (i+1 < instruccion->tam_bytes) sprintf(cad+(i*6)+4, ", ");
                    }
                    msg_buf = d_printf("%s generó %lu bytes (%s)", lily_simbolo_simbolo_print(instruccion->simbolo), instruccion->tam_bytes, cad);
                    enviar_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "a_semantico", msg_buf);
                    free(msg_buf);
                    free(cad);
                }
                else {
                    msg_buf = d_printf("Reducción no completada: %s", lily_simbolo_instruccion_print(instruccion));
                    enviar_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "a_semantico", msg_buf);
                    free(msg_buf);
                    num_indeterminadas++;
                    pc_identificador->definido = false;
                }
            }
            else if (instruccion->simbolo != NULL && instruccion->reducido) {
                msg_buf = d_printf("Saltando instrucción ya reducida: %s", lily_simbolo_instruccion_print(instruccion));
                enviar_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "a_semantico", msg_buf);
                free(msg_buf);
            }
            // Si hay etiqueta, establecemos su valor
            // FIX: esto redefinirá etiquetas definidas por otros medios
            if (instruccion->etiqueta != NULL) {
                struct lily_dict_nodo* etiqueta_nodo = lily_dict_get(identificadores, instruccion->etiqueta->valor);
                struct lily_simbolo_identificador* identificador;
                union lily_simbolo_numero* pc_numero;
                if (etiqueta_nodo != NULL) {
                    // Si ya la conocíamos, trabajamos con ese valor
                    identificador = etiqueta_nodo->valor;
                    pc_numero = identificador->valor;
                    pc_numero->positivo = pc_obj->positivo;
                }
                else {
                    // Si no, lo añadimos
                    pc_numero = (union lily_simbolo_numero*) malloc(sizeof(union lily_simbolo_numero));
                    if (pc_numero == NULL) {
                        *estado = COD_MALLOC_FALLO;
                        cctx->ultimo = instruccion->etiqueta;
                        break;
                    }
                    pc_numero->positivo = pc_obj->positivo;
                    identificador = lily_a_semantico_anad_identificador(identificadores, instruccion->etiqueta->valor, pc_numero, true, estado, *ctx);
                    if (*estado != COD_OK) break;
                }
                if (!num_indeterminadas) identificador->definido = true;
            }
            // Actualizamos PC
            pc_obj->positivo += instruccion->tam_bytes;

            msg_buf = d_printf("PC vale ahora 0x%02zX", pc_obj->positivo);
            enviar_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "a_semantico", msg_buf);
            free(msg_buf);
            nodo = nodo->posterior;
        }
        // ¿Salimos por un error?
        if (*estado != COD_OK) {
            *tam = 0;
            return bytes;
        }
        msg_buf = d_printf("Instrucciones no reducidas: %zu, temporales: %zu", num_indeterminadas, num_temporales);
        enviar_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "a_semantico", msg_buf);
        free(msg_buf);
    } while (num_indeterminadas + num_temporales > 0 && iteraciones < it_max_op);

    if (iteraciones >= it_max_op) {
        *estado = COD_A_SEMANTICO_MAXIMO_ITERACIONES;
        return bytes;
    }

    // Cuando acabemos, crear el array...
    *tam = sizeof(uint8_t) * pc_obj->positivo;
    bytes = malloc(*tam);
    if (bytes == NULL) {
        *estado = COD_MALLOC_FALLO;
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
    free(*ctx);
    return bytes;
}
