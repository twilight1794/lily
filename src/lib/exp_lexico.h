/**
 * @file exp_lexico.h
 * Analizador léxico de una expresión
 */

#ifndef LILY_L_EXP_LEX
#define LILY_L_EXP_LEX

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common/cadena.h"
#include "../common/error.h"
#include "../common/lde.h"

enum LILY_EXP_TIPO_VAL {
    // Terminales
    VAL_NUMERO = 0,
    VAL_REGISTRO,
    VAL_BANDERA,
    VAL_AMB_C, // Para Z80
    VAL_DIRECCION,
    VAL_DESPLAZAMIENTO,

    // No terminales
    VAL_OP,
    VAL_ETIQUETA,
    VAL_CADENA,
    VAL_PARENTESIS_AP,
    VAL_PARENTESIS_CI,
    VAL_DESP_AP,
    VAL_DESP_CI,
    VAL_SEPARADOR,
};

enum LILY_EXP_TIPO_OP {
    OP_SUMA = 0,
    OP_RESTA,
    OP_MULTI,
    OP_DIV,
    OP_MODULO,
    OP_MIEMBRO,
    OP_BIT_AND,
    OP_BIT_OR,
    OP_BIT_XOR,
    OP_COMP_2,
    OP_COMP_1,
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
    OP_DIF,
};

struct Exp_Simbolo {
    enum LILY_EXP_TIPO_VAL tipo;
    char* valor;
};

#endif
