/**
 * @file exp_lexico.h
 * Analizador léxico de una expresión
 */

#ifndef LILY_L_EXP_LEX
#define LILY_L_EXP_LEX

#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../common/cadena.h"
#include "../common/error.h"
#include "../common/lde.h"

enum LILY_EXP_TIPO_VAL {
    // Terminales
    VAL_REGISTRO = 0,
    VAL_BANDERA,
    VAL_AMB_C,
    VAL_NUMERO,
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
    void* valor;
};

char* isregister(char* blob, size_t* i);

char* isflag(char* blob, size_t* i);

/*
 * Devuelve el nombre del tipo de dato usando su identificador
 * @param v Tipo de valor
 * @return Cadena correspondiente al operador
 */
char* exp_cadena_operador(enum LILY_EXP_TIPO_OP v);

/* Analiza una expresión y genera los símbolos de la expresión
 * @param blob Cadena de texto que contiene una expresión en formato infijo
 * @param i Puntero al índice
 * @param simbolos Lista que va a almacenar los símbolos relevantes
 */
int exp_lexico(char* blob, size_t* i, struct LDE_LDE* simbolos);

#endif
