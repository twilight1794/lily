/**
 * This file is free and unencumbered software released into the public
 * domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <https://unlicense.org/>
 */

#include "bitarray.h"

// Los bits se numeran desde el menos significativo de cada byte: el bit n
// está en el byte n/8, en la posición n%8.

#define TAM_ARRAY 12
#define TAM_VALOR 8
#define MAX_DESPLAZAMIENTO 24
#define MAX_TAMANO 32

// Implementaciones de referencia, bit por bit
static int obt_bit(const uint8_t* array, size_t n) {
    return (array[n >> 3] >> (n & 7)) & 1;
}

static void est_bit(uint8_t* array, size_t n, int bit) {
    if (bit) array[n >> 3] |= (uint8_t) (1 << (n & 7));
    else array[n >> 3] &= (uint8_t) ~(1 << (n & 7));
}

static void ref_obtener(const uint8_t* array, size_t desplazamiento, size_t tamano, uint8_t* valor) {
    for (size_t i = 0; i < tamano; i++)
        est_bit(valor, i, obt_bit(array, desplazamiento + i));
}

static void ref_guardar(uint8_t* array, size_t desplazamiento, size_t tamano, const uint8_t* valor) {
    for (size_t i = 0; i < tamano; i++)
        est_bit(array, desplazamiento + i, obt_bit(valor, i));
}

MunitResult lily_bitarray_test_obtener_alineado(const MunitParameter params[], void *user_data) {
    const uint8_t array[] = { 0xab, 0xcd, 0xef };
    uint8_t valor[TAM_VALOR] = { 0 };
    lily_bitarray_obtener(array, 0, 8, valor);
    munit_assert_uint8(valor[0], ==, 0xab);
    munit_assert_uint8(valor[1], ==, 0x00);

    memset(valor, 0, TAM_VALOR);
    lily_bitarray_obtener(array, 8, 16, valor);
    munit_assert_uint8(valor[0], ==, 0xcd);
    munit_assert_uint8(valor[1], ==, 0xef);
    munit_assert_uint8(valor[2], ==, 0x00);
    return MUNIT_OK;
}

MunitResult lily_bitarray_test_obtener_dentro_byte(const MunitParameter params[], void *user_data) {
    // 1011 0100 -> bits 2..5 = 1101
    const uint8_t array[] = { 0xb4, 0x00 };
    uint8_t valor[TAM_VALOR] = { 0 };
    lily_bitarray_obtener(array, 2, 4, valor);
    munit_assert_uint8(valor[0], ==, 0x0d);
    munit_assert_uint8(valor[1], ==, 0x00);
    return MUNIT_OK;
}

MunitResult lily_bitarray_test_obtener_cruzando_bytes(const MunitParameter params[], void *user_data) {
    // 0xa53c >> 5 = 0x529 -> 6 bits = 0x29
    const uint8_t array[] = { 0x3c, 0xa5, 0x00 };
    uint8_t valor[TAM_VALOR] = { 0 };
    lily_bitarray_obtener(array, 5, 6, valor);
    munit_assert_uint8(valor[0], ==, 0x29);
    munit_assert_uint8(valor[1], ==, 0x00);
    return MUNIT_OK;
}

MunitResult lily_bitarray_test_obtener_16_desalineado(const MunitParameter params[], void *user_data) {
    // 0x654321 >> 4 = 0x65432 -> 16 bits = 0x5432
    const uint8_t array[] = { 0x21, 0x43, 0x65, 0x00 };
    uint8_t valor[TAM_VALOR] = { 0 };
    lily_bitarray_obtener(array, 4, 16, valor);
    munit_assert_uint8(valor[0], ==, 0x32);
    munit_assert_uint8(valor[1], ==, 0x54);
    munit_assert_uint8(valor[2], ==, 0x00);
    return MUNIT_OK;
}

MunitResult lily_bitarray_test_obtener_no_excede(const MunitParameter params[], void *user_data) {
    // Con todo el origen en 1, solo deben aparecer los bits pedidos
    const uint8_t array[] = { 0xff, 0xff, 0xff, 0xff };
    uint8_t valor[TAM_VALOR] = { 0 };
    lily_bitarray_obtener(array, 3, 12, valor);
    munit_assert_uint8(valor[0], ==, 0xff);
    munit_assert_uint8(valor[1], ==, 0x0f);
    munit_assert_uint8(valor[2], ==, 0x00);
    return MUNIT_OK;
}

MunitResult lily_bitarray_test_obtener_exhaustivo(const MunitParameter params[], void *user_data) {
    uint8_t array[TAM_ARRAY];
    munit_rand_memory(TAM_ARRAY, array);
    for (size_t desplazamiento = 0; desplazamiento < MAX_DESPLAZAMIENTO; desplazamiento++) {
        for (size_t tamano = 1; tamano <= MAX_TAMANO; tamano++) {
            uint8_t esperado[TAM_VALOR] = { 0 };
            uint8_t valor[TAM_VALOR] = { 0 };
            ref_obtener(array, desplazamiento, tamano, esperado);
            lily_bitarray_obtener(array, desplazamiento, tamano, valor);
            if (memcmp(esperado, valor, TAM_VALOR) != 0)
                munit_errorf("desplazamiento=%zu, tamano=%zu", desplazamiento, tamano);
        }
    }
    return MUNIT_OK;
}

MunitResult lily_bitarray_test_guardar_alineado(const MunitParameter params[], void *user_data) {
    uint8_t array[] = { 0x00, 0x00, 0x00, 0x00 };
    const uint8_t valor[] = { 0x34, 0x12 };
    lily_bitarray_guardar(array, 8, 16, valor);
    munit_assert_uint8(array[0], ==, 0x00);
    munit_assert_uint8(array[1], ==, 0x34);
    munit_assert_uint8(array[2], ==, 0x12);
    munit_assert_uint8(array[3], ==, 0x00);
    return MUNIT_OK;
}

MunitResult lily_bitarray_test_guardar_dentro_byte(const MunitParameter params[], void *user_data) {
    // Bits 2..4 <- 010, los demás se conservan
    uint8_t array[] = { 0xff, 0xff };
    const uint8_t valor[] = { 0x02 };
    lily_bitarray_guardar(array, 2, 3, valor);
    munit_assert_uint8(array[0], ==, 0xeb);
    munit_assert_uint8(array[1], ==, 0xff);
    return MUNIT_OK;
}

MunitResult lily_bitarray_test_guardar_cruzando_bytes(const MunitParameter params[], void *user_data) {
    // Limpiar bits 6..11 en un array lleno
    uint8_t array1[] = { 0xff, 0xff, 0xff };
    const uint8_t ceros[] = { 0x00 };
    lily_bitarray_guardar(array1, 6, 6, ceros);
    munit_assert_uint8(array1[0], ==, 0x3f);
    munit_assert_uint8(array1[1], ==, 0xf0);
    munit_assert_uint8(array1[2], ==, 0xff);

    // Activar bits 6..11 en un array vacío
    uint8_t array2[] = { 0x00, 0x00, 0x00 };
    const uint8_t unos[] = { 0x3f };
    lily_bitarray_guardar(array2, 6, 6, unos);
    munit_assert_uint8(array2[0], ==, 0xc0);
    munit_assert_uint8(array2[1], ==, 0x0f);
    munit_assert_uint8(array2[2], ==, 0x00);
    return MUNIT_OK;
}

MunitResult lily_bitarray_test_guardar_un_bit(const MunitParameter params[], void *user_data) {
    // Banderas de 1 bit, como NF, ZF y PF del LC3 (bits 144, 145 y 146)
    uint8_t array[19] = { 0 };
    const uint8_t uno = 1, cero = 0;
    uint8_t valor;

    lily_bitarray_guardar(array, 144, 1, &uno);
    munit_assert_uint8(array[18], ==, 0x01);
    lily_bitarray_guardar(array, 146, 1, &uno);
    munit_assert_uint8(array[18], ==, 0x05);
    lily_bitarray_guardar(array, 144, 1, &cero);
    munit_assert_uint8(array[18], ==, 0x04);

    valor = 0;
    lily_bitarray_obtener(array, 144, 1, &valor);
    munit_assert_uint8(valor, ==, 0);
    valor = 0;
    lily_bitarray_obtener(array, 145, 1, &valor);
    munit_assert_uint8(valor, ==, 0);
    valor = 0;
    lily_bitarray_obtener(array, 146, 1, &valor);
    munit_assert_uint8(valor, ==, 1);
    return MUNIT_OK;
}

MunitResult lily_bitarray_test_guardar_exhaustivo(const MunitParameter params[], void *user_data) {
    uint8_t original[TAM_ARRAY];
    uint8_t valor[TAM_VALOR];
    munit_rand_memory(TAM_ARRAY, original);
    munit_rand_memory(TAM_VALOR, valor);
    for (size_t desplazamiento = 0; desplazamiento < MAX_DESPLAZAMIENTO; desplazamiento++) {
        for (size_t tamano = 1; tamano <= MAX_TAMANO; tamano++) {
            uint8_t esperado[TAM_ARRAY];
            uint8_t array[TAM_ARRAY];
            memcpy(esperado, original, TAM_ARRAY);
            memcpy(array, original, TAM_ARRAY);
            ref_guardar(esperado, desplazamiento, tamano, valor);
            lily_bitarray_guardar(array, desplazamiento, tamano, valor);
            if (memcmp(esperado, array, TAM_ARRAY) != 0)
                munit_errorf("desplazamiento=%zu, tamano=%zu", desplazamiento, tamano);
        }
    }
    return MUNIT_OK;
}

MunitResult lily_bitarray_test_ida_vuelta(const MunitParameter params[], void *user_data) {
    uint8_t array[TAM_ARRAY];
    uint8_t valor[TAM_VALOR];
    munit_rand_memory(TAM_ARRAY, array);
    munit_rand_memory(TAM_VALOR, valor);
    for (size_t desplazamiento = 0; desplazamiento < MAX_DESPLAZAMIENTO; desplazamiento++) {
        for (size_t tamano = 1; tamano <= MAX_TAMANO; tamano++) {
            uint8_t esperado[TAM_VALOR] = { 0 };
            uint8_t leido[TAM_VALOR] = { 0 };
            ref_obtener(valor, 0, tamano, esperado);
            lily_bitarray_guardar(array, desplazamiento, tamano, valor);
            lily_bitarray_obtener(array, desplazamiento, tamano, leido);
            if (memcmp(esperado, leido, TAM_VALOR) != 0)
                munit_errorf("desplazamiento=%zu, tamano=%zu", desplazamiento, tamano);
        }
    }
    return MUNIT_OK;
}
