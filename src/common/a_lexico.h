/*
 * @file a_lexico.h
 * Analizador léxico del preprocesador Lily
 */

#ifndef LILY_C_A_LEX
#define LILY_C_A_LEX

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../common/cadena.h"
#include "../common/lde.h"
#include "../common/error.h"

// Tipos para símbolos

/**
 * Tipo de símbolo
 */
enum Exp_Tipo_Simbolo {
    // Terminal
    SIMB_MNEMO,
    SIMB_DIRECTIVA,
    SIMB_ETI,
    VAL_OBJETO,
    VAL_NUMERO,
    VAL_FUNCION,

    // No terminal
    SIMB_INDETERMINADO,
    VAL_OP,
    VAL_VARIABLE,
    VAL_CADENA,
    VAL_PARENTESIS_AP,
    VAL_PARENTESIS_CI,
    VAL_SEPARADOR
};

enum Exp_Tipo_Op {
    OP_SUMA,
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

/**
 * Estructura para guardar un símbolo léxico
 * Un
 */
struct Exp_Lex_Simbolo {
    enum Exp_Tipo_Simbolo tipo;
    char* valor;
};

/**
 * Crea un objeto para símbolos
 * @returns Un objeto para símbolo nuevo
 */
struct Exp_Lex_Simbolo* Exp_Lex_Simbolo_Create();

// Modos del analizador léxico
/**
 * Modo comentario
 * Descarta todo hasta encontrar un salto de línea
 */
void exp_modo_comentario(char* blob, size_t* i);

/**
 * Modo ambiguo
 * Aquí podemos tener definición de etiqueta, mnemónico o directiva
 */
int exp_modo_ambiguo(char* blob, size_t* i, struct Exp_Lex_Simbolo* sim);

/*
 * Comprueba si una cadena es un identificador válido para un símbolo reservado
 * @param blob Cadena a analizar
 * @param i Índice del puntero
 * @return Cadena del identificador, o NULL si no es un identificador válido
 */
char* exp_esobjeto(const char* blob, size_t* i);

/*
 * Comprueba si una cadena es una constante numérica válida
 * @param blob Cadena a analizar
 * @param i Índice del puntero
 * @return Número representado, o NULL si no es una constante numérica válida
 */
long* exp_esnumero(const char* blob, size_t* i);

/*
 * Comprueba si una cadena es un identificador válido para una función definida
 * @param blob Cadena a analizar
 * @param i Índice del puntero
 * @return Cadena del identificador, o NULL si no es un identificador válido
 */
char* exp_esfuncion(const char* blob, size_t* i);

/*
 * Devuelve el nombre del tipo de dato usando su identificador
 * @param v Tipo de valor
 * @return Cadena correspondiente al operador
 */
char* exp_cadena_operador(enum Exp_Tipo_Op v);

/**
 * Función de entrada del analizador léxico
 * @param blob Cadena de texto que contiene el código fuente del archivo principal
 * @param simbolos Lista de símbolos encontrados
 * @return Código de error
 */
int exp_lexico(char* blob, struct LDE_LDE* simbolos);

#endif