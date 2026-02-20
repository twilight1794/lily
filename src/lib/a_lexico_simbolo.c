#include "a_lexico_simbolo.h"

#include "../common/log.h"

extern char* lily_a_lexico_directivas[];
extern char* lily_a_lexico_operadores[];

struct lily_a_lexico_simbolo* lily_a_lexico_simbolo_create(void) {
    return calloc(1, sizeof(struct lily_a_lexico_simbolo));
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
    }
    return obj;
}

char* lily_a_lexico_simbolo_print(struct lily_a_lexico_simbolo* simbolo) {
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
            sprintf(buff, patron, LC);
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
        case SIMB_NUMERO:
            patron = "(%lu:%lu) Número %d";
            buff = (char*) malloc(snprintf(NULL, 0, patron, LC, simbolo->valor) + 1);
            if (buff == NULL) return NULL;
            sprintf(buff, patron, LC, simbolo->valor);
            break;
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
            patron = "(%lu:%lu) DesplazamientoAp";
            buff = (char*) malloc(snprintf(NULL, 0, patron, LC) + 1);
            if (buff == NULL) return NULL;
            sprintf(buff, patron, LC);
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
            patron = "(%lu:%lu) Indeterminado";
            buff = (char*) malloc(snprintf(NULL, 0, patron, LC) + 1);
            if (buff == NULL) return NULL;
            sprintf(buff, patron, LC);
            break;
    }
    return buff;
}