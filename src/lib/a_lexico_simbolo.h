/**
 * @file a_lexico_simbolo.h
 * Definiciones de los tipos de símbolos creados por el analizador léxico
 */

#ifndef LILY_L_A_LEXICO_SIMBOLO
#define LILY_L_A_LEXICO_SIMBOLO

#include <stdlib.h>

#include "../common/lde.h"

/**
 * Tipo de símbolo
 */
enum lily_a_lexico_tipo_simbolo {
    SIMB_INDETERMINADO,

    // Terminal
    SIMB_MNEMO,
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
    OP_DIF,

    // Directivas
    /// Definicion de constantes
    DIR_DB = 64,
    DIR_DWL,
    DIR_DWM,
    DIR_DDL,
    DIR_DDM,
    DIR_DQL,
    DIR_DQM,
    DIR_DR,
    DIR_DRD,
    DIR_DFS,
    /// Definición de símbolos para el preprocesador
    DIR_CONST,
    DIR_VAR,
    /// Condicionales
    DIR_IF,
    DIR_IFDEF,
    DIR_IFNDEF,
    DIR_ELSE,
    DIR_ELIF,
    /// Bucles
    DIR_WHILE,
    DIR_LOOP,
    /// Inclusión de archivos
    DIR_INC,
    /// Control de ensamblado
    DIR_CPU,
    DIR_ORG,
    DIR_STOP,
    /// Estructuras de datos
    DIR_STRUCT,
    DIR_UNION,
    /// Subrutinas
    DIR_MACRO,
    DIR_PROC,
    /// Miscelánea
    DIR_END
};

/**
 * Estructura para guardar un símbolo léxico
 */
struct lily_a_lexico_simbolo {
    enum lily_a_lexico_tipo_simbolo tipo; /**< Tipo del símbolo */
    enum lily_a_lexico_tipo_simbolo subtipo; /**< Si aplica, tipo más específico del símbolo (para operadores y directivas) */
    size_t linea; /**< Línea donde se encuentra el inicio del símbolo */
    size_t linea_pos; /**< Índice del inicio de la línea \a linea en el blob */
    size_t pos; /**< Índice del inicio del símbolo en el blob */
    void* valor; /**< Valor representativo del símbolo */
};

/**
 * Crea un objeto para guardar un símbolo
 * @return Un objeto para símbolo nuevo
 */
struct lily_a_lexico_simbolo* lily_a_lexico_simbolo_create(void);

/**
 * Estructura para guardar un símbolo sintáctico
 */
struct lily_a_sintactico_instruccion {
    struct lily_a_lexico_simbolo* etiqueta; /**< Etiqueta asociada, si existe */
    struct lily_a_lexico_simbolo* simbolo; /**< Mnemónico o directiva asociada, si existe */
    struct lily_lde_lde* params; /**< Lista de parámetros */
    struct lily_lde_lde* instrucciones; /**< Para macros que agrupan instrucciones */
    struct lily_lde_lde* instruccionesn; /**< Como \a instrucciones, pero para la rama opuesta */
    size_t dirección; /**< Dirección de memoria asociada */
};

/**
 * Crea un objeto para guardar una instrucción
 * @return Un objeto para instrucción nuevo
 */
struct lily_a_sintactico_instruccion* lily_a_sintactico_instruccion_create(void);

#endif
