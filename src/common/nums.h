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
 * Codifica el número \a val con los parámetros correctos
 * @param buf Array donde escribir los valores
 * @param val Valor primitivo
 * @param tam Número de bytes del valor: 1, 2, 4 o 8
 * @param signo El rango de valores admitidos según su signo: 0 para positivos, 1 para negativos, 2 para cubrir todo el rango posible para el tamaño
 * @param endianness Endianness de la representación: \c true para little-endian, \c false para big-endian
 * @return Código de operación resultante
 */
enum lily_estado lily_nums_codificar_num(uint8_t* buf, void* val, const size_t tam, const int signo, const bool endianness);

#endif
