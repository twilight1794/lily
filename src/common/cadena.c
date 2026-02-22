#include "cadena.h"

char* lily_cadena_create(void) {
    return calloc(1, 1);
}

char* lily_cadena_add(char* cad, const char* c) {
    size_t tcar, tcad, i = 0;
    char* ncad;
    if (cad == NULL) return cad;
    tcar = lily_cadena_size_char(c);
    tcad = strlen(cad);
    ncad = realloc(cad, tcad + tcar + 1);
    if (ncad == NULL) return cad;
    while (i<tcar) {
      ncad[tcad+i] = c[i];
      i++;
    }
    ncad[tcad+tcar] = 0;
    return ncad;
}

char* lily_cadena_add_byte(char* cad, const char c) {
    size_t tcad;
    char* ncad;
    if (cad == NULL) return cad;
    tcad = strlen(cad);
    ncad = realloc(cad, tcad + 1);
    if (ncad == NULL) return cad;
    ncad[tcad] = c;
    ncad[tcad+1] = 0;
    return ncad;
}

char* lily_cadena_concat(char* cad, const char* cad2) {
    size_t tcad, tcad2, i = 0;
    char* ncad;
    if (cad == NULL) return cad;
    tcad = strlen(cad);
    tcad2 = strlen(cad2);
    ncad = realloc(cad, tcad + tcad2 + 1);
    if (ncad == NULL) return cad;
    while (i<tcad2) {
      ncad[tcad+i] = cad2[i];
      i++;
    }
    ncad[tcad+tcad2] = 0;
    return ncad;
}

char* lily_cadena_remove(char* cad) {
    size_t tcad;
    char* ncad;
    if (cad == NULL) return cad;
    tcad = strlen(cad);
    ncad = realloc(cad, tcad);
    if (ncad == NULL) return cad;
    ncad[tcad-1] = 0;
    return ncad;
}

size_t lily_cadena_size_char(const char* c) {
  size_t n;
  if ((unsigned char) *c == 0xC0 || (unsigned char) *c == 0xC1 || (unsigned char) *c >= 0xF5) n=0;
  else if ((unsigned char) *c >= 0xF0) n=4;
  else if ((unsigned char) *c >= 0xE0) n=3;
  else if ((unsigned char) *c >= 0xC2) n=2;
  else n=1;
  return n;
}
