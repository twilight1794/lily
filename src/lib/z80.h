/**
 * @file z80.h
 * Analizador léxico, sintáctico y semántico del ensamblador Z80
 */

#ifndef LILY_L_Z80
#define LILY_L_Z80

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../common/cadena.h"
#include "../common/lde.h"
#include "../common/error.h"
#include "z80_exp.h"

// Tipos para símbolos

/**
 * Tipo de símbolo
 */
enum Z80_Tipo_Simbolo {
    SIMB_INDETERMINADO = 0,
    SIMB_INSTRUCCION,
    SIMB_ETI,
};

/**
 * Estructura para guardar un símbolo léxico
 */
struct Z80_Lex_Simbolo {
    enum Z80_Tipo_Simbolo tipo;
    char* valor;
    char* etiqueta;
    struct LDE_LDE* expresion;
};

/**
 * Crea un objeto para símbolos
 * @returns Un objeto para símbolo nuevo
 */
struct Z80_Lex_Simbolo* Z80_Lex_Simbolo_Create();

// Modos del analuzador léxico

/**
 * Modo comentario
 * Descarta todo hasta encontrar un salto de línea
 */
void modo_comentario(char* blob, size_t* i);

/**
 * Modo ambiguo
 * Aquí podemos tener definición de etiqueta, mnemónico o directiva
 */
int modo_ambiguo(char* blob, size_t* i, struct Z80_Lex_Simbolo* sim);

/**
 * Función de entrada del analizador léxico
 * @param blob Cadena de texto que contiene el código fuente del archivo principal
 * @param simbolos Lista de símbolos encontrados
 * @return Puntero para guardar el código de error
 */
int z80_lexico(char* blob, struct LDE_LDE* simbolos);

#endif
