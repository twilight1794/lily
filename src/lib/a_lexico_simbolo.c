#include "a_lexico_simbolo.h"

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