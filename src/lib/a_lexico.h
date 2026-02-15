/*
 * @file a_lexico.h
 * Analizador léxico del preprocesador Lily
 */

#ifndef LILY_L_A_LEXICO
#define LILY_L_A_LEXICO

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../common/cadena.h"
#include "../common/defs.h"
#include "../common/error.h"
#include "../common/lde.h"
#include "../common/log.h"
#include "../common/pow.h"

#include "../lib/a_lexico_ctipos.h"
#include "../lib/a_lexico_simbolo.h"

#include <libintl.h>

/**
 * Estructura para almacenar detalles sobre la ejecución del análisis léxico
 */
struct lily_a_lexico_ctx {
    enum lily_a_lexico_tipo_simbolo tipo; //< Tipo tentativo del símbolo que se estaba generando
    size_t i_inicial; //< Posición en \a blob del símbolo tentativo
    size_t i_desp; //< Desplazamiento desde \a i_inicial hasta el valor actual de \a i
};

// Modos del analizador léxico
/**
 * @brief Modo comentario
 * Descarta todo hasta encontrar un salto de línea
 * @param blob Cadena a analizar
 * @param i Índice del puntero
 */
void lily_a_lexico_modo_comentario(const char* blob, size_t* i);

/**
 * @brief Modo directiva
 * Comprueba si la cadena precedida por '.' es una directiva reconocida por Lily
 * @param blob Cadena a analizar
 * @param i Índice del puntero
 * @param sim Estructura para almacenar el símbolo de la directiva, o NULL si no es una directiva válida
 * @return Código de error de la operación
 */
enum lily_error lily_a_lexico_modo_directiva(const char* blob, size_t* i, struct lily_a_lexico_simbolo** sim);

/**
 * @brief Modo referencia a etiqueta
 * Comprueba si la cadena precedida por '$' es una referencia a etiqueta válida
 * @param blob Cadena a analizar
 * @param i Índice del puntero
 * @param sim Estructura para almacenar el símbolo de la referencia, o NULL si no es una referencia a etiqueta válida
 * @return Código de error de la operación
 */
enum lily_error lily_a_lexico_modo_r_etiqueta(const char* blob, size_t* i, struct lily_a_lexico_simbolo** sim);

/*
 * @brief Modo objeto
 * Comprueba si la cadena precedida por '%' es un identificador válido para un objeto reservado
 * @param blob Cadena a analizar
 * @param i Índice del puntero
 * @param sim Estructura para almacenar el símbolo del objeto, o NULL si no es un identificador de objeto válido
 * @return Código de error de la operación
 */
enum lily_error lily_a_lexico_modo_objeto(const char* blob, size_t* i, struct lily_a_lexico_simbolo** sim);

/*
 * @brief Modo cadena
 * Comprueba si la cadena precedida por ''' o '"' es una cadena válida
 * @param blob Cadena a analizar
 * @param i Índice del puntero
 * @param sim Estructura para almacenar el símbolo de la cadena, o NULL si no es un identificador de objeto válido
 * @param tipo Caracter que abrió la cadena: comilla simple o doble
 * @return Código de error de la operación
 */
enum lily_error lily_a_lexico_modo_cadena(const char* blob, size_t* i, struct lily_a_lexico_simbolo** sim);

/*
 * @brief Modo número
 * Comprueba si una cadena es una constante numérica válida
 * @param blob Cadena a analizar
 * @param i Índice del puntero
 * @param sim Estructura para almacenar el símbolo creado, o NULL si no es una constante numérica válido
 * @param tipo Caracter que marca el tipo del número recibido: 'x', 'o', 'b', o 0
 * @return Código de error de la operación
 */
enum lily_error lily_a_lexico_modo_numero(const char* blob, size_t* i, struct lily_a_lexico_simbolo** sim, const char tipo);

/*
 * @brief Modo operador
 * Comprueba si una cadena representa un operador válido
 * @param blob Cadena a analizar
 * @param i Índice del puntero
 * @param sim Estructura para almacenar el símbolo creado
 * @return Código de error de la operación
 */
enum lily_error lily_a_lexico_modo_operador(const char* blob, size_t* i, struct lily_a_lexico_simbolo** sim);

/**
 * @brief Modo ambiguo
 * Comprueba si la cadena puede ser un identificador válido para un mnemónico, definición de etiqueta
 * @param blob Cadena a analizar
 * @param i Índice del puntero
 * @param sim Estructura para almacenar el símbolo creado, o NULL si no es un valor válido
 * @return Código de error de la operación
 */
enum lily_error lily_a_lexico_modo_ambiguo(const char* blob, size_t* i, struct lily_a_lexico_simbolo** sim);

/**
 * Función de entrada del analizador léxico
 * @param blob Cadena de texto que contiene el código fuente del archivo principal
 * @param [out] simbolos Lista de símbolos encontrados
 * @param [out] ctx Estado de la ejecución al momento de salir de la función, si no es \c NULL
 * @return Código de estado de la operación
 */
enum lily_error lily_a_lexico(const char* blob, struct lily_lde_lde* simbolos, struct lily_a_lexico_ctx* ctx);

#endif