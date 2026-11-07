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

#include "expresiones.h"

// Las expresiones se evalúan con la directiva DDL, que no necesita un esquema
// de procesador: el resultado son 4 bytes en little endian.

struct caso {
    const char* expresion;
    int64_t esperado;
};

static int mensaje_nulo(enum lily_lily_mensaje_tipo tipo, int subtipo, char* modulo, void* obj) {
    (void) tipo; (void) subtipo; (void) modulo; (void) obj;
    return 0;
}

// Ensambla \a fuente solo con directivas, y devuelve los bytes generados
static uint8_t* ensamblar(const char* fuente, size_t* tam, enum lily_estado* estado) {
    void* ctx = NULL;
    *estado = COD_OK;
    struct lily_lde_lde* simbolos = lily_a_lexico(fuente, mensaje_nulo, estado, &ctx);
    if (*estado != COD_OK) return NULL;
    struct lily_lde_lde* ast = lily_a_sintactico(simbolos, mensaje_nulo, estado, &ctx);
    if (*estado != COD_OK) return NULL;
    struct lily_dict_dict* opciones = lily_dict_create();
    munit_assert_not_null(opciones);
    return lily_a_semantico(ast, NULL, 0, tam, opciones, mensaje_nulo, estado, &ctx);
}

// Evalúa \a expresion y devuelve su valor como entero de 32 bits con signo
static enum lily_estado evaluar(const char* expresion, int64_t* resultado) {
    char fuente[128];
    snprintf(fuente, sizeof(fuente), "v: .DDL %s\n", expresion);
    enum lily_estado estado;
    size_t tam = 0;
    uint8_t* bytes = ensamblar(fuente, &tam, &estado);
    if (estado != COD_OK) return estado;
    munit_assert_size(tam, ==, 4);
    *resultado = (int32_t) ((uint32_t) bytes[0] | (uint32_t) bytes[1] << 8 | (uint32_t) bytes[2] << 16 | (uint32_t) bytes[3] << 24);
    free(bytes);
    return COD_OK;
}

static MunitResult probar_casos(const struct caso* casos, size_t n) {
    for (size_t i = 0; i < n; i++) {
        int64_t resultado = 0;
        enum lily_estado estado = evaluar(casos[i].expresion, &resultado);
        if (estado != COD_OK)
            munit_errorf("\"%s\": estado %d", casos[i].expresion, estado);
        if (resultado != casos[i].esperado)
            munit_errorf("\"%s\" = %" PRId64 ", se esperaba %" PRId64, casos[i].expresion, resultado, casos[i].esperado);
    }
    return MUNIT_OK;
}

#define probar(casos) probar_casos((casos), sizeof(casos) / sizeof(casos[0]))

MunitResult lily_expresiones_test_postfija_negativo(const MunitParameter params[], void *user_data) {
    // "1 - -2" debe quedar como 1 2 NEG - en notación postfija
    void* ctx = NULL;
    enum lily_estado estado = COD_OK;
    struct lily_lde_lde* simbolos = lily_a_lexico("v: .DDL 1 - -2\n", mensaje_nulo, &estado, &ctx);
    munit_assert_int(estado, ==, COD_OK);
    struct lily_lde_lde* ast = lily_a_sintactico(simbolos, mensaje_nulo, &estado, &ctx);
    munit_assert_int(estado, ==, COD_OK);
    munit_assert_size(lily_lde_size(ast), ==, 1);
    struct lily_simbolo_instruccion* instruccion = lily_lde_get(ast, 0)->valor;
    munit_assert_size(lily_lde_size(instruccion->params), ==, 4);
    const enum lily_simbolo_tipo tipos[] = { SIMB_NUMERO, SIMB_NUMERO, SIMB_OPERADOR, SIMB_OPERADOR };
    const int subtipos[] = { 0, 0, OP_NEGATIVO, OP_RESTA };
    for (size_t i = 0; i < 4; i++) {
        struct lily_simbolo_simbolo* simbolo = lily_lde_get(instruccion->params, i)->valor;
        munit_assert_int(simbolo->tipo, ==, tipos[i]);
        if (tipos[i] == SIMB_OPERADOR) munit_assert_int(simbolo->subtipo, ==, subtipos[i]);
    }
    return MUNIT_OK;
}

MunitResult lily_expresiones_test_precedencia_asociatividad(const MunitParameter params[], void *user_data) {
    static const struct caso casos[] = {
        { "10 - 2 - 3", 5 },
        { "16 / 4 / 2", 2 },
        { "2 * 3 + 1", 7 },
        { "1 + 2 * 3", 7 },
        { "(1 + 1) * 3", 6 },
        { "3 * (1 + 1)", 6 },
        { "100 - (20 - 5)", 85 },
        { "1 << 2 + 1", 8 },
    };
    return probar(casos);
}

MunitResult lily_expresiones_test_negativos(const MunitParameter params[], void *user_data) {
    static const struct caso casos[] = {
        { "-1", -1 },
        { "-0x80", -128 },
        { "-(2 + 3)", -5 },
        { "4 - -1", 5 },
        { "--5", 5 },
        { "-2 * 3", -6 },
        { "2 * -3", -6 },
        { "-7 / 2", -3 },
        { "0 - 1", -1 },
        { "1 - 3 + 1", -1 },
        { "-1 + 5", 4 },
    };
    return probar(casos);
}

MunitResult lily_expresiones_test_comparaciones(const MunitParameter params[], void *user_data) {
    static const struct caso casos[] = {
        { "1 < 2", 1 },
        { "2 < 1", 0 },
        { "1 <= 1", 1 },
        { "2 >= 3", 0 },
        { "2 > 1", 1 },
        { "1 = 1", 1 },
        { "1 != 2", 1 },
        { "-1 < 0", 1 },
        { "-1 > -2", 1 },
        { "(0 - 1) < 0", 1 },
    };
    return probar(casos);
}

MunitResult lily_expresiones_test_bits_logicos(const MunitParameter params[], void *user_data) {
    static const struct caso casos[] = {
        { "1 << 3", 8 },
        { "0x80 >> 3", 16 },
        { "3 & 6", 2 },
        { "3 | 4", 7 },
        { "3 ^ 5", 6 },
        { "~0", -1 },
        { "~~5", 5 },
        { "!0", 1 },
        { "1 && 0", 0 },
        { "1 || 0", 1 },
        { "-1 & 0xff", 255 },
    };
    return probar(casos);
}

MunitResult lily_expresiones_test_constante_negativa(const MunitParameter params[], void *user_data) {
    // El signo de una constante debe conservarse al usarla en otra expresión
    enum lily_estado estado;
    size_t tam = 0;
    uint8_t* bytes = ensamblar("x: .CONST -5\nv: .DDL $x * 2\nw: .DDL $x < 0\n", &tam, &estado);
    munit_assert_int(estado, ==, COD_OK);
    // CONST reserva 8 bytes, seguidos de los dos DDL
    munit_assert_size(tam, ==, 16);
    munit_assert_memory_equal(8, bytes + 8, ((uint8_t[]) { 0xf6, 0xff, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00 }));
    free(bytes);
    return MUNIT_OK;
}

MunitResult lily_expresiones_test_resta_objeto(const MunitParameter params[], void *user_data) {
    // Un "-" tras un operando sigue siendo una resta, aunque el operando sea un objeto
    int64_t resultado;
    munit_assert_int(evaluar("%A - 1", &resultado), ==, COD_A_SEMANTICO_OPERANDO_OBJETO);
    return MUNIT_OK;
}
