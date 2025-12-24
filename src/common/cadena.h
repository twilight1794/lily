/**
 * @file cadena.h
 * Funciones para manejar cadenas de texto dinámicas
 */

#ifndef LILY_C_CAD
#define LILY_C_CAD

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/**
 * Crea una cadena de texto vacía
 * @return Puntero a una cadena
 */
char* lily_cadena_create(void);

/**
 * Añade un punto de código UTF-8 a la cadena de caracteres, comprobando que la secuencia de bytes sea válida
 * @param cad Cadena de texto a manipular
 * @param c Caracter a añadir
 * @return Nuevo puntero a la cadena modificada
 */
char* lily_cadena_add(char* cad, const char* c);

/**
 * Concatena una cadena a la cadena de caracteres
 * @param cad Cadena de texto a manipular
 * @param cad2 Cadena de texto a concatenar
 * @return Nuevo puntero a la cadena modificada
 */
char* lily_cadena_concat(char* cad, const char* cad2);

/**
 * Quita un caracter de la cadena de caracteres
 * @param cad Cadena de texto a manipular
 * @return Nuevo puntero a la cadena modificada
 */
char* lily_cadena_remove(char* cad);

/**
 * Dado un caracter, indica cuántos bytes ocupa si es codificado en UTF-8
 * @param c Puntero a caracter
 * @return Tamaño en bytes del caracter
 */
size_t lily_cadena_size_char(const char* c);

#endif
