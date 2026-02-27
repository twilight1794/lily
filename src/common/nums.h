/*
 * @file nums.h
 * Funciones para trabajar con valores numéricos
 */

#ifndef LILY_C_NUMS
#define LILY_C_NUMS

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "error.h"

/**
 * Codifica el número \a val con los parámetros correctos
 * @param buf Array donde escribir los valores
 * @param val Valor primitivo
 * @param tam Número de bytes del valor: 1, 2, 4 o 8
 * @param negativo Si se admiten números negativos: \c true si sí, \c false si no
 * @param endianness Endianness de la representación: \c true para little-endian, \c false para big-endian
 * @return Código de operación resultante
 */
enum lily_error lily_nums_codificar_num(uint8_t* buf, void* val, const size_t tam, const bool negativo, const bool endianness);

#endif
