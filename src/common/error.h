
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
    COD_CARACTER_INVALIDO,
    // lib/a_lexico.h
    COD_A_LEXICO_CARACTER_INVALIDO, /**< Caracter inválido */
};

#endif
