/*
 * @file a_lexico.h
 * Analizador léxico del preprocesador Lily
 */

#ifndef LILY_C_A_LEX
#define LILY_C_A_LEX

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../common/cadena.h"
#include "../common/error.h"
#include "../common/lde.h"
#include "../common/pow.h"

#include "../lib/a_lexico_ctipos.h"
#include "../lib/a_lexico_directivas.h"

/**
 * @def lex_esblanco(c)
 * Comprueba si un caracter \a c es un espacio (0x20), tabulador (0x9), salto de línea (0xa), o retorno de carro (0xd)
 * @param c Caracter a analizar
 */
#define lex_esblanco(c) (lex_ctipos[(unsigned char) c]&1)

/**
 * @def lex_esoperador(c)
 * Comprueba si un caracter \a c puede ser parte de un operador
 * @param c Caracter a analizar
 */
#define lex_esoperador(c) (lex_ctipos[(unsigned char) c]&2)

// Tipos para símbolos

/**
 * Tipo de símbolo
 */
enum Lex_Tipo_Simbolo {
    // Terminal
    SIMB_MNEMO = 1,
    SIMB_DIRECTIVA,
    SIMB_ETI,
    SIMB_OBJETO,
    SIMB_NUMERO,
    SIMB_FUNCION,

    // No terminal
    SIMB_INDETERMINADO,
    SIMB_VARIABLE,
    SIMB_CADENA_SIMPLE,
    SIMB_CADENA_NUL,
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
    OP_DIF
};

/**
 * Estructura para guardar un símbolo léxico
 * Un
 */
struct Lex_Simbolo {
    enum Lex_Tipo_Simbolo tipo;
    void* valor;
};

/**
 * Crea un objeto para símbolos
 * @return Un objeto para símbolo nuevo
 */
struct Lex_Simbolo* lex_simbolo_create(void);

// Modos del analizador léxico
/**
 * @brief Modo comentario
 * Descarta todo hasta encontrar un salto de línea
 * @param blob Cadena a analizar
 * @param i Índice del puntero
 */
void lex_modo_comentario(const char* blob, size_t* i);

/**
 * @brief Modo directiva
 * Comprueba si la cadena precedida por '.' es una directiva reconocida por Lily
 * @param blob Cadena a analizar
 * @param i Índice del puntero
 * @param sim Estructura para almacenar el símbolo de la directiva, o NULL si no es una directiva válida
 * @return Código de error de la operación
 */
enum Lily_Error lex_modo_directiva(const char* blob, size_t* i, struct Lex_Simbolo** sim);

/**
 * @brief Modo referencia a etiqueta
 * Comprueba si la cadena precedida por '$' es una referencia a etiqueta válida
 * @param blob Cadena a analizar
 * @param i Índice del puntero
 * @param sim Estructura para almacenar el símbolo de la referencia, o NULL si no es una referencia a etiqueta válida
 * @return Código de error de la operación
 */
enum Lily_Error lex_modo_r_etiqueta(const char* blob, size_t* i, struct Lex_Simbolo** sim);

/*
 * @brief Modo objeto
 * Comprueba si la cadena precedida por '%' es un identificador válido para un objeto reservado
 * @param blob Cadena a analizar
 * @param i Índice del puntero
 * @param sim Estructura para almacenar el símbolo del objeto, o NULL si no es un identificador de objeto válido
 * @return Código de error de la operación
 */
enum Lily_Error lex_modo_objeto(const char* blob, size_t* i, struct Lex_Simbolo** sim);

/*
 * @brief Modo cadena
 * Comprueba si la cadena precedida por ''' o '"' es una cadena válida
 * @param blob Cadena a analizar
 * @param i Índice del puntero
 * @param sim Estructura para almacenar el símbolo de la cadena, o NULL si no es un identificador de objeto válido
 * @param tipo Caracter que abrió la cadena: comilla simple o doble
 * @return Código de error de la operación
 */
enum Lily_Error lex_modo_cadena(const char* blob, size_t* i, struct Lex_Simbolo** sim);

/*
 * @brief Modo número
 * Comprueba si una cadena es una constante numérica válida
 * @param blob Cadena a analizar
 * @param i Índice del puntero
 * @param sim Estructura para almacenar el símbolo creado, o NULL si no es una constante numérica válido
 * @param tipo Caracter que marca el tipo del número recibido: 'x', 'o', 'b', o 0
 * @return Código de error de la operación
 */
enum Lily_Error lex_modo_numero(const char* blob, size_t* i, struct Lex_Simbolo** sim, const char tipo);

/*
 * @brief Modo operador
 * Comprueba si una cadena representa un operador válido
 * @param blob Cadena a analizar
 * @param i Índice del puntero
 * @param sim Estructura para almacenar el símbolo creado, o NULL si no es una constante numérica válido
 * @return Código de error de la operación
 */
enum Lily_Error lex_modo_operador(const char* blob, size_t* i, struct Lex_Simbolo** sim);

/**
 * @brief Modo ambiguo
 * Comprueba si la cadena puede ser un número hexadecimal de la forma /[:xdigit:]h/, o identificador válido para un mnemónico, definición de etiqueta
 * @param blob Cadena a analizar
 * @param i Índice del puntero
 * @param sim Estructura para almacenar el símbolo creado, o NULL si no es un valor válido
 * @return Código de error de la operación
 */
enum Lily_Error lex_modo_ambiguo(const char* blob, size_t* i, struct Lex_Simbolo** sim);

/**
 * Función de entrada del analizador léxico
 * @param blob Cadena de texto que contiene el código fuente del archivo principal
 * @param simbolos Lista de símbolos encontrados
 * @return Código de error de la operación
 */
int lex_lexico(const char* blob, struct LDE_LDE* simbolos);

#endif