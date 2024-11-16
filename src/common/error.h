
/**
 * @file error.h
 * Códigos de error de Lily
 */

#ifndef LILY_C_ERROR
#define LILY_C_ERROR

enum Lily_Error {
    COD_OK,
    // Generales
    COD_MALLOC_FALLO,
    // lib/z80_lexico.h
    COD_Z80_LEXICO_CARACTER_INVALIDO, /**< Caracter inválido */
};

#endif
