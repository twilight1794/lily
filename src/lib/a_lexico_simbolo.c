#include "a_lexico_simbolo.h"

extern char* lily_a_lexico_directivas[];
extern char* lily_a_lexico_operadores[];

struct lily_a_lexico_simbolo* lily_a_lexico_simbolo_create(void) {
    struct lily_a_lexico_simbolo* obj = (struct lily_a_lexico_simbolo*) malloc(sizeof(struct lily_a_lexico_simbolo));
    if (obj != NULL) {
        obj->tipo = SIMB_INDETERMINADO;
        obj->subtipo = SIMB_INDETERMINADO;
        obj->linea = 0;
        obj->linea_pos = 0;
        obj->pos = 0;
        obj->valor = NULL;
        obj->signo = false;
    }
    return obj;
}

struct lily_a_sintactico_instruccion* lily_a_sintactico_instruccion_create(void) {
    struct lily_a_sintactico_instruccion* obj = (struct lily_a_sintactico_instruccion*) calloc(1, sizeof(struct lily_a_sintactico_instruccion));
    if (obj != NULL) {
        obj->etiqueta = NULL;
        obj->simbolo = NULL;
        obj->params = lily_lde_create();
        if (obj->params == NULL) {
            free(obj);
            return NULL;
        }
        obj->instrucciones = NULL;
        obj->instruccionesn = NULL;
        obj->direccion = SIZE_MAX;
        obj->tam_bytes = 0;
        obj->bytes = NULL;
    }
    return obj;
}

char* lily_a_lexico_simbolo_print(const struct lily_a_lexico_simbolo* simbolo) {
    #define LC simbolo->linea, (simbolo->pos-simbolo->linea_pos+1)
    char* patron;
    char* buff;
    switch (simbolo->tipo) {
        case SIMB_MNEMO:
            patron = "(%lu:%lu) Mnemo %s";
            buff = (char*) malloc(snprintf(NULL, 0, patron, LC, simbolo->valor) + 1);
            if (buff == NULL) return NULL;
            sprintf(buff, patron, LC, simbolo->valor);
            break;
        case SIMB_DIRECTIVA:
            patron = "(%lu:%lu) Directiva %s";
            buff = (char*) malloc(snprintf(NULL, 0, patron, LC, lily_a_lexico_directivas[simbolo->subtipo-64]) + 1);
            if (buff == NULL) return NULL;
            sprintf(buff, patron, LC, lily_a_lexico_directivas[simbolo->subtipo-64]);
            break;
        case SIMB_ETI:
            patron = "(%lu:%lu) Etiqueta %s";
            buff = (char*) malloc(snprintf(NULL, 0, patron, LC, simbolo->valor) + 1);
            if (buff == NULL) return NULL;
            sprintf(buff, patron, LC, simbolo->valor);
            break;
        case SIMB_OBJETO:
            patron = "(%lu:%lu) Objeto %s";
            buff = (char*) malloc(snprintf(NULL, 0, patron, LC, simbolo->valor) + 1);
            if (buff == NULL) return NULL;
            sprintf(buff, patron, LC, simbolo->valor);
            break;
        case SIMB_NUMERO: {
            union lily_a_lexico_numero* num = (union lily_a_lexico_numero*) simbolo->valor;
            if (simbolo->signo)
                patron = "(%lu:%lu) Número %" PRIi64;
            else
                patron = "(%lu:%lu) Número %" PRIu64;
            buff = (char*) malloc(snprintf(NULL, 0, patron, LC, (simbolo->signo)?num->negativo:num->positivo) + 1);
            if (buff == NULL) return NULL;
            sprintf(buff, patron, LC, (simbolo->signo)?num->negativo:num->positivo);
            break;
        }
        case SIMB_FUNCION:
            patron = "(%lu:%lu) Función %s";
            buff = (char*) malloc(snprintf(NULL, 0, patron, LC, simbolo->valor) + 1);
            if (buff == NULL) return NULL;
            sprintf(buff, patron, LC, simbolo->valor);
            break;
        case SIMB_VARIABLE:
            patron = "(%lu:%lu) Variable %s";
            buff = (char*) malloc(snprintf(NULL, 0, patron, LC, simbolo->valor) + 1);
            if (buff == NULL) return NULL;
            sprintf(buff, patron, LC, simbolo->valor);
            break;
        case SIMB_CADENA_SIMPLE:
            patron = "(%lu:%lu) Cadena '%s'";
            buff = (char*) malloc(snprintf(NULL, 0, patron, LC, simbolo->valor) + 1);
            if (buff == NULL) return NULL;
            sprintf(buff, patron, LC, simbolo->valor);
            break;
        case SIMB_CADENA_NUL:
            patron = "(%lu:%lu) CadenaZ \"%s\"";
            buff = (char*) malloc(snprintf(NULL, 0, patron, LC, simbolo->valor) + 1);
            if (buff == NULL) return NULL;
            sprintf(buff, patron, LC, simbolo->valor);
            break;
        case SIMB_OPERADOR:
            patron = "(%lu:%lu) Operador %s";
            buff = (char*) malloc(snprintf(NULL, 0, patron, LC, lily_a_lexico_operadores[simbolo->subtipo-32]) + 1);
            if (buff == NULL) return NULL;
            sprintf(buff, patron, LC, lily_a_lexico_operadores[simbolo->subtipo-32]);
            break;
        case SIMB_PARENTESIS_AP:
            patron = "(%lu:%lu) ParéntesisAp";
            buff = (char*) malloc(snprintf(NULL, 0, patron, LC) + 1);
            if (buff == NULL) return NULL;
            sprintf(buff, patron, LC);
            break;
        case SIMB_PARENTESIS_CI:
            patron = "(%lu:%lu) ParéntesisCi";
            buff = (char*) malloc(snprintf(NULL, 0, patron, LC) + 1);
            if (buff == NULL) return NULL;
            sprintf(buff, patron, LC);
            break;
        case SIMB_DESPLAZAMIENTO_AP:
            if (simbolo->valor == NULL) {
                patron = "(%lu:%lu) DesplazamientoAp";
                buff = (char*) malloc(snprintf(NULL, 0, patron, LC) + 1);
                if (buff == NULL) return NULL;
                sprintf(buff, patron, LC);
            } else {
                patron = "(%lu:%lu) DesplazamientoAp { %s }";
                char* cad = lily_cadena_create();
                if (cad == NULL) return NULL;
                for (struct lily_lde_nodo* nodo = ((struct lily_lde_lde*) simbolo->valor)->inicio; nodo != NULL; nodo = nodo->posterior) {
                    char* cad_cont = lily_a_lexico_simbolo_print(nodo->valor);
                    if (cad_cont == NULL) return NULL;
                    char* cad_tmp = lily_cadena_concat(cad, cad_cont);
                    if (cad_tmp == NULL) return NULL;
                    cad = cad_tmp;
                    free(cad_cont);
                    if (nodo->posterior != NULL) {
                        cad_tmp = lily_cadena_concat(cad, ", ");
                        if (cad_tmp == NULL) return NULL;
                        cad = cad_tmp;
                    }
                }
                buff = (char*) malloc(snprintf(NULL, 0, patron, LC, cad) + 1);
                if (buff == NULL) return NULL;
                sprintf(buff, patron, LC, cad);
            }
            break;
        case SIMB_DESPLAZAMIENTO_CI:
            patron = "(%lu:%lu) DesplazamientoCi";
            buff = (char*) malloc(snprintf(NULL, 0, patron, LC) + 1);
            if (buff == NULL) return NULL;
            sprintf(buff, patron, LC);
            break;
        case SIMB_SEPARADOR:
            patron = "(%lu:%lu) Separador";
            buff = (char*) malloc(snprintf(NULL, 0, patron, LC) + 1);
            if (buff == NULL) return NULL;
            sprintf(buff, patron, LC);
            break;
        default:
            patron = "(%lu:%lu) Indeterminado (%d)";
            buff = (char*) malloc(snprintf(NULL, 0, patron, LC, simbolo->tipo) + 1);
            if (buff == NULL) return NULL;
            sprintf(buff, patron, LC);
            break;
    }
    return buff;
}

char* lily_a_sintactico_instruccion_print(const struct lily_a_sintactico_instruccion* instruccion) {
    char *patron, *buff_local, *buff_tmp;
    char* buff = lily_cadena_create();
    if (buff == NULL) return NULL;
    // Imprimir dirección
    if (esta_definido(instruccion)) {
        patron = "(%lu)";
        buff_local = (char*) malloc(snprintf(NULL, 0, patron, instruccion->direccion) + 1);
        if (buff_local == NULL) return NULL;
        sprintf(buff_local, patron, instruccion->direccion);
        buff_tmp = lily_cadena_concat(buff, buff_local);
        if (buff_tmp == NULL) return NULL;
        buff = buff_tmp;
        free(buff_local);
    } else {
        buff_tmp = lily_cadena_concat(buff, "(—)");
        if (buff_tmp == NULL) return NULL;
        buff = buff_tmp;
    }
    // Imprimir etiqueta, si la hay
    if (instruccion->etiqueta != NULL) {
        patron = " [%s:]";
        buff_local = (char*) malloc(snprintf(NULL, 0, patron, (char*) instruccion->etiqueta->valor) + 1);
        if (buff_local == NULL) return NULL;
        sprintf(buff_local, patron, (char*) instruccion->etiqueta->valor);
        buff_tmp = lily_cadena_concat(buff, buff_local);
        if (buff_tmp == NULL) return NULL;
        buff = buff_tmp;
        free(buff_local);
    }
    // Imprimir tipo
    char* dumb = " }";
    if (instruccion->simbolo != NULL) {
        buff_tmp = lily_cadena_add(buff, dumb);
        if (buff_tmp == NULL) return NULL;
        buff = buff_tmp;
        char* cad_tipo = lily_a_lexico_simbolo_print(instruccion->simbolo);
        if (cad_tipo == NULL) return NULL;
        buff_tmp = lily_cadena_concat(buff, cad_tipo);
        if (buff_tmp == NULL) return NULL;
        buff = buff_tmp;
        buff_tmp = lily_cadena_concat(buff, " { ");
        if (buff_tmp == NULL) return NULL;
        buff = buff_tmp;
        // Imprimir parámetros
        for (size_t i = 0; i < lily_lde_size(instruccion->params); i++) {
            struct lily_a_lexico_simbolo* sim = lily_lde_get(instruccion->params, i)->valor;
            cad_tipo = lily_a_lexico_simbolo_print(sim);
            if (cad_tipo == NULL) {
                free(cad_tipo);
                return NULL;
            }
            buff_tmp = lily_cadena_concat(buff, cad_tipo);
            if (buff_tmp == NULL) return NULL;
            buff = buff_tmp;
            free(cad_tipo);
            if (lily_lde_get(instruccion->params, i)->posterior != NULL) {
                buff_tmp = lily_cadena_concat(buff, ", ");
                if (buff_tmp == NULL) return NULL;
                buff = buff_tmp;
            }
        }
        buff_tmp = lily_cadena_concat(buff, dumb);
        if (buff_tmp == NULL) return NULL;
        buff = buff_tmp;
    }
    return buff;
}

struct lily_a_semantico_identificador* lily_a_semantico_identificador_create(void) {
    struct lily_a_semantico_identificador* obj = (struct lily_a_semantico_identificador*) malloc(sizeof(struct lily_a_semantico_identificador));
    if (obj != NULL) {
        obj->valor = 0;
        obj->es_const = false;
    }
    return obj;
}