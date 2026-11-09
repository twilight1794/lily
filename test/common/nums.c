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

#include "nums.h"

// Modos de signo de lily_nums_codificar_num
#define SIN_SIGNO 0
#define CON_SIGNO 1
#define TODO 2

// Codifica \a valor en little endian, y devuelve el estado
static enum lily_estado codificar(int64_t valor, size_t tam, int signo, uint8_t* buf) {
    union lily_simbolo_numero n = { .negativo = valor };
    return lily_nums_codificar_num(buf, &n.positivo, tam, signo, true);
}

// Comprueba que \a valor se codifique sin errores como \a esperado
#define assert_codifica(valor, tam, signo, ...) do { \
        const uint8_t esperado[] = { __VA_ARGS__ }; \
        uint8_t buf[8] = { 0 }; \
        munit_assert_int(codificar((valor), (tam), (signo), buf), ==, COD_OK); \
        munit_assert_memory_equal((tam), buf, esperado); \
    } while (0)

// Comprueba que \a valor sea rechazado con el código \a codigo
#define assert_rechaza(valor, tam, signo, codigo) do { \
        uint8_t buf[8] = { 0 }; \
        munit_assert_int(codificar((valor), (tam), (signo), buf), ==, (codigo)); \
    } while (0)

MunitResult lily_nums_test_sin_signo(const MunitParameter params[], void *user_data) {
    assert_codifica(0, 1, SIN_SIGNO, 0x00);
    assert_codifica(255, 1, SIN_SIGNO, 0xff);
    assert_rechaza(256, 1, SIN_SIGNO, COD_NUMS_MAS_8);
    assert_rechaza(-1, 1, SIN_SIGNO, COD_NUMS_MAS_8);
    assert_codifica(65535, 2, SIN_SIGNO, 0xff, 0xff);
    assert_rechaza(65536, 2, SIN_SIGNO, COD_NUMS_MAS_16);
    assert_codifica(4294967295, 4, SIN_SIGNO, 0xff, 0xff, 0xff, 0xff);
    assert_rechaza(4294967296, 4, SIN_SIGNO, COD_NUMS_MAS_32);
    return MUNIT_OK;
}

MunitResult lily_nums_test_con_signo(const MunitParameter params[], void *user_data) {
    assert_codifica(-128, 1, CON_SIGNO, 0x80);
    assert_codifica(127, 1, CON_SIGNO, 0x7f);
    assert_codifica(-1, 1, CON_SIGNO, 0xff);
    assert_rechaza(128, 1, CON_SIGNO, COD_NUMS_MAS_8);
    assert_rechaza(-129, 1, CON_SIGNO, COD_NUMS_MAS_8);
    assert_codifica(-32768, 2, CON_SIGNO, 0x00, 0x80);
    assert_codifica(32767, 2, CON_SIGNO, 0xff, 0x7f);
    assert_rechaza(32768, 2, CON_SIGNO, COD_NUMS_MAS_16);
    assert_rechaza(-32769, 2, CON_SIGNO, COD_NUMS_MAS_16);
    assert_codifica(INT32_MIN, 4, CON_SIGNO, 0x00, 0x00, 0x00, 0x80);
    assert_rechaza((int64_t) INT32_MAX + 1, 4, CON_SIGNO, COD_NUMS_MAS_32);
    assert_rechaza((int64_t) INT32_MIN - 1, 4, CON_SIGNO, COD_NUMS_MAS_32);
    return MUNIT_OK;
}

MunitResult lily_nums_test_todo(const MunitParameter params[], void *user_data) {
    // Negativos hasta el mínimo con signo, positivos hasta el máximo sin signo
    assert_codifica(-1, 1, TODO, 0xff);
    assert_codifica(-128, 1, TODO, 0x80);
    assert_codifica(255, 1, TODO, 0xff);
    assert_rechaza(-129, 1, TODO, COD_NUMS_MAS_8);
    assert_rechaza(256, 1, TODO, COD_NUMS_MAS_8);
    assert_codifica(-1, 2, TODO, 0xff, 0xff);
    assert_codifica(-32768, 2, TODO, 0x00, 0x80);
    assert_codifica(65535, 2, TODO, 0xff, 0xff);
    assert_rechaza(-32769, 2, TODO, COD_NUMS_MAS_16);
    assert_rechaza(65536, 2, TODO, COD_NUMS_MAS_16);
    assert_codifica(-1, 4, TODO, 0xff, 0xff, 0xff, 0xff);
    assert_codifica(4294967295, 4, TODO, 0xff, 0xff, 0xff, 0xff);
    assert_rechaza((int64_t) INT32_MIN - 1, 4, TODO, COD_NUMS_MAS_32);
    assert_rechaza(4294967296, 4, TODO, COD_NUMS_MAS_32);
    // 8 bytes: todo cabe
    assert_codifica(-2, 8, TODO, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff);
    return MUNIT_OK;
}

MunitResult lily_nums_test_endianness(const MunitParameter params[], void *user_data) {
    uint8_t buf[8] = { 0 };
    union lily_simbolo_numero n = { .positivo = 0x12345678 };
    munit_assert_int(lily_nums_codificar_num(buf, &n.positivo, 4, SIN_SIGNO, true), ==, COD_OK);
    munit_assert_memory_equal(4, buf, ((uint8_t[]) { 0x78, 0x56, 0x34, 0x12 }));
    munit_assert_int(lily_nums_codificar_num(buf, &n.positivo, 4, SIN_SIGNO, false), ==, COD_OK);
    munit_assert_memory_equal(4, buf, ((uint8_t[]) { 0x12, 0x34, 0x56, 0x78 }));
    n.negativo = -2;
    munit_assert_int(lily_nums_codificar_num(buf, &n.positivo, 2, TODO, false), ==, COD_OK);
    munit_assert_memory_equal(2, buf, ((uint8_t[]) { 0xff, 0xfe }));
    return MUNIT_OK;
}

MunitResult lily_nums_test_error_no_escribe(const MunitParameter params[], void *user_data) {
    // Un valor rechazado no debe modificar el búfer
    uint8_t buf[4] = { 0xaa, 0xaa, 0xaa, 0xaa };
    munit_assert_int(codificar(-32769, 2, TODO, buf), ==, COD_NUMS_MAS_16);
    munit_assert_memory_equal(4, buf, ((uint8_t[]) { 0xaa, 0xaa, 0xaa, 0xaa }));
    return MUNIT_OK;
}

// Comprueba que \a bytes se decodifique como \a esperado
#define assert_decodifica(esperado, tam, signo, endianness, ...) do { \
        uint8_t bytes[] = { __VA_ARGS__ }; \
        union lily_simbolo_numero n = { .positivo = 0xa5a5a5a5a5a5a5a5 }; \
        munit_assert_int(lily_nums_decodificar_num(&n, bytes, (tam), (signo), (endianness)), ==, COD_OK); \
        munit_assert_int64(n.negativo, ==, (esperado)); \
    } while (0)

MunitResult lily_nums_test_decodificar(const MunitParameter params[], void *user_data) {
    (void) params; (void) user_data;
    // Sin signo: se rellena con ceros
    assert_decodifica(255, 1, SIN_SIGNO, true, 0xff);
    assert_decodifica(0xfffe, 2, SIN_SIGNO, true, 0xfe, 0xff);
    // Con signo: se extiende el bit más alto, y solo si está activo
    assert_decodifica(5, 1, CON_SIGNO, true, 0x05);
    assert_decodifica(-5, 1, CON_SIGNO, true, 0xfb);
    assert_decodifica(127, 1, CON_SIGNO, true, 0x7f);
    assert_decodifica(-128, 1, CON_SIGNO, true, 0x80);
    assert_decodifica(0x1234, 2, CON_SIGNO, true, 0x34, 0x12);
    assert_decodifica(-2, 4, CON_SIGNO, true, 0xfe, 0xff, 0xff, 0xff);
    // Todo el rango: se interpreta sin signo
    assert_decodifica(255, 1, TODO, true, 0xff);
    assert_decodifica(0x8000, 2, TODO, true, 0x00, 0x80);
    // Big endian: el byte más significativo es el primero
    assert_decodifica(-2, 2, CON_SIGNO, false, 0xff, 0xfe);
    assert_decodifica(0x12345678, 4, SIN_SIGNO, false, 0x12, 0x34, 0x56, 0x78);
    assert_decodifica(0x7f00, 2, CON_SIGNO, false, 0x7f, 0x00);
    // 8 bytes: no hay relleno
    assert_decodifica(-1, 8, CON_SIGNO, true, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff);
    assert_decodifica(INT64_MIN, 8, CON_SIGNO, false, 0x80, 0, 0, 0, 0, 0, 0, 0);
    // Ida y vuelta con signo, en ambas endianness
    static const int64_t valores[] = { -32768, -129, -1, 0, 1, 127, 32767 };
    for (size_t i = 0; i < sizeof(valores) / sizeof(valores[0]); i++) {
        for (int e = 0; e < 2; e++) {
            union lily_simbolo_numero n = { .negativo = valores[i] }, m;
            uint8_t buf[2];
            munit_assert_int(lily_nums_codificar_num(buf, &n.positivo, 2, CON_SIGNO, e), ==, COD_OK);
            munit_assert_int(lily_nums_decodificar_num(&m, buf, 2, CON_SIGNO, e), ==, COD_OK);
            munit_assert_int64(m.negativo, ==, valores[i]);
        }
    }
    return MUNIT_OK;
}
