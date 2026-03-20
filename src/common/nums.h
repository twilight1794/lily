/*
 * @file nums.h
 * Funciones para trabajar con valores numéricos
 */

#ifndef LILY_C_NUMS
#define LILY_C_NUMS

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "estado.h"
#include "../lib/simbolo.h"

/**
 * Codifica el número \a val en una secuencia de bytes
 * @param [out] buf Array donde escribir los bytes
 * @param val Valor primitivo
 * @param tam Número de bytes del valor: 1, 2, 4 u 8
 * @param signo El rango de valores admitidos según su signo: 0 para un tipo
 * positivo, 1 para un tipo negativo, 2 para cubrir todo el rango posible para
 * el tamaño
 * @param endianness Endianness de la representación: \c true para
 * little-endian, \c false para big-endian
 * @return Código de operación resultante
 */
enum lily_estado lily_nums_codificar_num(uint8_t* buf, uint64_t* val, const size_t tam, const int signo, const bool endianness);

/**
 * Decodifica la secuencia de bytes \a bytes a un número uint64_t
 * @param [out] val Valor resultante
 * @param bytes Array desde donde leer los bytes
 * @param tam Número de bytes del valor: 1, 2, 4 u 8
 * @param signo El rango de valores admitidos según su signo: 0 para un tipo
 * positivo, 1 para un tipo negativo, 2 para cubrir t
 * @param endianness Endianness de la representación: \c true para
 * little-endian, \c false para big-endian
 * @return Código de operación resultante
 */
enum lily_estado lily_nums_decodificar_num(union lily_simbolo_numero* val, uint8_t* bytes, const size_t tam, const int signo, const bool endianness);

#endif
