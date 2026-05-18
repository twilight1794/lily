/*
 * Copyright (C) 2026 Giovanni Alfredo Garciliano Diaz
 *
 * This file is part of Lily.
 *
 * Lily is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Lily is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Lily. If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * @file simbolo.h
 * Definiciones de los tipos de símbolos creados por el analizador léxico
 */

#ifndef LILY_L_SIMBOLO
#define LILY_L_SIMBOLO

#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common/cadena.h"
#include "../common/lde.h"
#include "mensajes.h"

/**
 * Tipo de símbolo
 */
enum lily_simbolo_tipo {
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
    SIMB_OPERADOR,
};

enum lily_simbolo_operador {
    OP_INDETERMINADO,
    SIMB_PARENTESIS_AP,
    SIMB_PARENTESIS_CI,
    SIMB_DESPLAZAMIENTO_AP,
    SIMB_DESPLAZAMIENTO_CI,
    SIMB_SEPARADOR,
    OP_SUMA,
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
};

enum lily_simbolo_directiva {
    DIR_INDETERMINADA,
    // Definicion de constantes
    DIR_DB,
    DIR_DWL,
    DIR_DWM,
    DIR_DDL,
    DIR_DDM,
    DIR_DQL,
    DIR_DQM,
    DIR_DR,
    DIR_DRD,
    DIR_DFS,
    // Definición de símbolos para el preprocesador
    DIR_CONST,
    DIR_VAR,
    /// Condicionales
    DIR_IF,
    DIR_IFDEF,
    DIR_IFNDEF,
    DIR_ELSE,
    DIR_ELIF,
    DIR_ELIFDEF,
    DIR_ELIFNDEF,
    // Bucles
    DIR_WHILE,
    DIR_LOOP,
    // Inclusión de archivos
    DIR_INC,
    // Control de ensamblado
    DIR_CPU,
    DIR_ORG,
    DIR_STOP,
    // Estructuras de datos
    DIR_STRUCT,
    DIR_UNION,
    // Subrutinas
    DIR_MACRO,
    DIR_PROC,
    // Miscelánea
    DIR_END
};

/**
 * Amalgama para números
 */
union lily_simbolo_numero {
    int64_t negativo; /**< Para números negativos */
    uint64_t positivo; /**< Para números positivos */
    uint8_t bytes[8]; /**< Array para acceder a su representación en bytes */
};

/**
 * Estructura para guardar un símbolo léxico
 */
struct lily_simbolo_simbolo {
    enum lily_simbolo_tipo tipo; /**< Tipo del símbolo */
    int subtipo; /**< Si aplica, tipo más específico del símbolo (para operadores y directivas) */
    size_t linea; /**< Línea donde se encuentra el inicio del símbolo */
    size_t linea_pos; /**< Índice del inicio de la línea \a linea en el blob */
    size_t pos; /**< Índice del inicio del símbolo en el blob */
    void* valor; /**< Valor representativo del símbolo */
    bool signo; /**< Para valores numéricos, signo del número representado: \c true para negativo, \c false para positivo */
};

/**
 * Crea un objeto para guardar un símbolo
 * @return Un objeto para símbolo nuevo
 */
struct lily_simbolo_simbolo *lily_simbolo_simbolo_create(void);

/**
 * Crea una copia de un símbolo y su valor
 * @param orig Símbolo a copiar
 * @return Un objeto para símbolo nuevo, copia de \a orig. \c NULL si hubo errores con \c malloc.
 */
struct lily_simbolo_simbolo* lily_simbolo_simbolo_clone(struct lily_simbolo_simbolo* orig);

/**
 * Genera una representación en texto de \a simbolo
 * @param simbolo Símbolo cuyos datos imprimir
 * @return Cadena de texto con la representación en texto del objeto
 */
char* lily_simbolo_simbolo_print(const struct lily_simbolo_simbolo* simbolo);

/**
 * Devuelve el nivel de precedencia de un operador
 * @param operador Operador en cuestión
 * @return Nivel de precedencia de \a operador
 */
int lily_simbolo_precedencia(const struct lily_simbolo_simbolo* operador);

/**
 * Devuelve la aridad de un operador
 * @param subtipo Tipo de operador
 * @return Aridad del operador indicado
 */
int lily_simbolo_aridad(enum lily_simbolo_operador subtipo);

/**
 * Estructura para guardar un símbolo sintáctico
 */
struct lily_simbolo_instruccion {
    struct lily_simbolo_simbolo* etiqueta; /**< Etiqueta asociada, si existe */
    struct lily_simbolo_simbolo* simbolo; /**< Mnemónico o directiva asociada, si existe */
    struct lily_lde_lde* params; /**< Lista de parámetros, o condiciones para bloques */
    struct lily_lde_lde* params_tmp; /**< Como \a params, pero para reducciones tentativas */
    struct lily_lde_lde* instrucciones; /**< Para macros que agrupan instrucciones */
    struct lily_lde_lde* instruccionesn; /**< Como \a instrucciones, pero para la rama opuesta */
    size_t direccion; /**< Dirección de memoria asociada */
    size_t tam_bytes; /**< Tamaño del array guardado en \a bytes */
    uint8_t* bytes; /**< Representación en bytes de la instrucción */
    bool reducido; /**< Si la instrucción ya se ha reducido exitosamente a una lista de bytes */
};

/**
 * Crea un objeto para guardar una instrucción
 * @return Un objeto para instrucción nuevo
 */
struct lily_simbolo_instruccion* lily_simbolo_instruccion_create(void);

/**
 * Genera una representación en texto de \a instrucción
 * @param instruccion Instrucción cuyos datos imprimir
 * @return Cadena de texto con la representación en texto del objeto
 */
char* lily_simbolo_instruccion_print(const struct lily_simbolo_instruccion* instruccion);

struct lily_simbolo_identificador {
    union lily_simbolo_numero* valor; /**< Número asociado al identificador */
    bool es_const; /**< Si se podrá modificar el valor asociado al identificador una vez asignado */
    bool definido; /**< Si el valor asociado a este identificador es temporal o definitivo */
};

/**
 * Crea un objeto para guardar un identificador
 * @return Un objeto para identificador nuevo
 */
struct lily_simbolo_identificador* lily_simbolo_identificador_create(void);
#endif
