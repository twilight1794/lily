
/**
 * @file error.h
 * Códigos de error de Lily
 */

#ifndef LILY_C_ERROR
#define LILY_C_ERROR

enum Lily_Error {
    COD_OK,
    // Generales
    COD_MALLOC_FALLO, /**< [mc]alloc falló al reservar memoria */
    COD_CARACTER_INVALIDO, /**< Caracter inválido en el archivo */
    // lib/a_lexico.h
    COD_A_LEXICO_RECON_ERRONEO, /**< La cadena no corresponde a un tipo, durante un estado de ambigüedad */
    COD_A_LEXICO_DIRECTIVA_INVALIDA, /**< La directiva especificada no existe */
    COD_A_LEXICO_CARACTER_INVALIDO, /**< Caracter inválido durante el procesamiento de un tipo */
    COD_A_LEXICO_FIN_INESPERADO, /**< El archivo terminó antes de lo esperado */
};

#endif
