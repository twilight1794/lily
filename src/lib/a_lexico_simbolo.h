/**
 * @file a_lexico_simbolo.h
 * Definiciones de los tipos de símbolos creados por el analizador léxico
 */

#ifndef LILY_L_A_LEXICO_SIMBOLO
#define LILY_L_A_LEXICO_SIMBOLO

#include <stdlib.h>

/**
 * Tipo de símbolo
 */
enum lily_a_lexico_tipo_simbolo {
    SIMB_INDETERMINADO,

    // Terminal
    SIMB_MNEMO = 1,
    SIMB_DIRECTIVA,
    SIMB_ETI,
    SIMB_OBJETO,
    SIMB_NUMERO,
    SIMB_FUNCION,

    // No terminal
    SIMB_VARIABLE,
    SIMB_CADENA_SIMPLE,
    SIMB_CADENA_NUL,
    SIMB_OPERADOR, //< Superclase para todo lo siguiente
    SIMB_PARENTESIS_AP,
    SIMB_PARENTESIS_CI,
    SIMB_SEPARADOR,

    // Operadores
    OP_SUMA = 32,
    OP_RESTA,
    OP_MULTI,
    OP_DIV,
    OP_MODULO,
    OP_MIEMBRO,
    OP_BIT_AND,
    OP_BIT_OR,
    OP_BIT_XOR,
    OP_BIT_NOT,
    OP_LOG_AND,
    OP_LOG_OR,
    OP_LOG_NEG,
    OP_DESP_IZQ,
    OP_DESP_DER,
    OP_MENOR_QUE,
    OP_MAYOR_QUE,
    OP_MENOR_IGUAL,
    OP_MAYOR_IGUAL,
    OP_IGUAL,
    OP_DIF
};

/**
 * Estructura para guardar un símbolo léxico
 */
struct lily_a_lexico_simbolo {
    enum lily_a_lexico_tipo_simbolo tipo;
    enum lily_a_lexico_tipo_simbolo subtipo;
    void* valor;
};

/**
 * Crea un objeto para símbolos
 * @return Un objeto para símbolo nuevo
 */
struct lily_a_lexico_simbolo* lily_a_lexico_simbolo_create(void);

#endif
