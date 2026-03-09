/**
* @file lily.h
 * Funciones de entrada para los procesos principales
 */

#ifndef LILY_L_LILY
#define LILY_L_LILY

#include "a_lexico.h"
#include "a_semantico.h"
#include "a_sintactico.h"
#include "../common/estado.h"

struct lily_lily_archivo {
    void* obj; /** Objeto subyacente del sistema */
    char* archivo; /** Puntero al contenido */
    int tipo; /** Tipo de archivo */
};

uint8_t* lily_lily_ensamble(const char* datos_entrada, char* arquitectura, struct lily_lily_archivo* (fun_abrir_archivo)(const char*, int), int (fun_cerrar_archivo)(struct lily_lily_archivo*), size_t* tam_salida, struct lily_ctx* ctx);

//uint8_t* lily_lily_desensamble(uint8_t* bytes, struct lily_lily_archivo* (fun_abrir_archivo)(const char*, int), int (fun_cerrar_archivo)(struct lily_lily_archivo*), struct lily_ctx* ctx);

#endif
