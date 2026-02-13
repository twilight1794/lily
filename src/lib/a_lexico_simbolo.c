#include "a_lexico_simbolo.h"

struct lily_a_lexico_simbolo* lily_a_lexico_simbolo_create(void) {
    return calloc(1, sizeof(struct lily_a_lexico_simbolo));
}