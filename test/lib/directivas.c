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

#include "directivas.h"

// Las directivas de datos no necesitan un esquema de procesador

struct caso {
    const char* fuente;
    size_t tam;
    uint8_t bytes[16];
};

static int mensaje_nulo(enum lily_lily_mensaje_tipo tipo, int subtipo, char* modulo, void* obj) {
    (void) tipo; (void) subtipo; (void) modulo; (void) obj;
    return 0;
}

// Ensambla \a fuente solo con directivas, y devuelve los bytes generados
static uint8_t* ensamblar(const char* fuente, size_t* tam, enum lily_estado* estado) {
    // Libera todo lo que reservan las etapas, igual que lily_lily_ensamble
    void* ctx = NULL;
    uint8_t* bytes = NULL;
    struct lily_lde_lde* ast = NULL;
    struct lily_dict_dict* opciones = NULL;
    *estado = COD_OK;
    *tam = 0;
    struct lily_lde_lde* simbolos = lily_a_lexico(fuente, mensaje_nulo, estado, &ctx);
    if (*estado != COD_OK) goto fin;
    ast = lily_a_sintactico(simbolos, mensaje_nulo, estado, &ctx);
    free(ctx);
    ctx = NULL;
    if (*estado != COD_OK) goto fin;
    opciones = lily_dict_create();
    munit_assert_not_null(opciones);
    bytes = lily_a_semantico(ast, NULL, 0, tam, opciones, mensaje_nulo, estado, &ctx);
    // Con un error, el contexto del semántico se conserva, con su mensaje de Lua
    if (ctx != NULL) free(((struct lily_a_semantico_ctx*) ctx)->lua_msg);
fin:
    free(ctx);
    lily_dict_destroy(opciones, NULL);
    lily_simbolo_instrucciones_destroy(ast);
    lily_simbolo_lista_destroy(simbolos);
    return bytes;
}

static MunitResult probar_casos(const struct caso* casos, size_t n) {
    for (size_t i = 0; i < n; i++) {
        enum lily_estado estado;
        size_t tam;
        uint8_t* bytes = ensamblar(casos[i].fuente, &tam, &estado);
        if (estado != COD_OK)
            munit_errorf("\"%s\": estado %d", casos[i].fuente, estado);
        if (tam != casos[i].tam)
            munit_errorf("\"%s\": %zu bytes, se esperaban %zu", casos[i].fuente, tam, casos[i].tam);
        if (memcmp(bytes, casos[i].bytes, tam))
            munit_errorf("\"%s\": bytes distintos a los esperados", casos[i].fuente);
        free(bytes);
    }
    return MUNIT_OK;
}

#define probar(casos) probar_casos((casos), sizeof(casos) / sizeof(casos[0]))

// Comprueba que \a fuente sea rechazada con el código \a codigo
static void assert_rechaza(const char* fuente, enum lily_estado codigo) {
    enum lily_estado estado;
    size_t tam;
    free(ensamblar(fuente, &tam, &estado));
    if (estado != codigo)
        munit_errorf("\"%s\": estado %d, se esperaba %d", fuente, estado, codigo);
}

MunitResult lily_directivas_test_db(const MunitParameter params[], void *user_data) {
    (void) params; (void) user_data;
    static const struct caso casos[] = {
        { "v: .DB 1, 2, 3\n", 3, { 0x01, 0x02, 0x03 } },
        // El número de símbolos en postfijo no es el número de datos
        { "v: .DB 1 + 1, 2 * 3, -1\n", 3, { 0x02, 0x06, 0xff } },
        { "v: .DB 255, -128\n", 2, { 0xff, 0x80 } },
        { "v: .DB \"ab\"\n", 3, { 'a', 'b', 0x00 } },
    };
    return probar(casos);
}

MunitResult lily_directivas_test_dw_dd_dq(const MunitParameter params[], void *user_data) {
    (void) params; (void) user_data;
    static const struct caso casos[] = {
        { "v: .DWL 0x1234, 0x5678\n", 4, { 0x34, 0x12, 0x78, 0x56 } },
        { "v: .DWM 0x1234, 0x5678\n", 4, { 0x12, 0x34, 0x56, 0x78 } },
        { "v: .DDL 1, -2\n", 8, { 0x01, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0xff } },
        { "v: .DDM 0x11223344\n", 4, { 0x11, 0x22, 0x33, 0x44 } },
        { "v: .DQL 1, 2\n", 16, { 0x01, 0, 0, 0, 0, 0, 0, 0, 0x02, 0, 0, 0, 0, 0, 0, 0 } },
        { "v: .DQM 1 + 1\n", 8, { 0, 0, 0, 0, 0, 0, 0, 0x02 } },
        // Literales de más de 32 bits
        { "v: .DQL 0x100000000\n", 8, { 0, 0, 0, 0, 0x01, 0, 0, 0 } },
        { "v: .DQM 0x0123456789abcdef\n", 8, { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef } },
        { "v: .DQL 18446744073709551615\n", 8, { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff } },
        { "v: .DQL 0b100000000000000000000000000000000\n", 8, { 0, 0, 0, 0, 0x01, 0, 0, 0 } },
        { "v: .DQL 0o40000000000\n", 8, { 0, 0, 0, 0, 0x01, 0, 0, 0 } },
    };
    return probar(casos);
}

MunitResult lily_directivas_test_referencia_adelantada(const MunitParameter params[], void *user_data) {
    (void) params; (void) user_data;
    static const struct caso casos[] = {
        { "v: .DB $x\nx: .DB 7\n", 2, { 0x01, 0x07 } },
        { "v: .DWL $x, $x\nw: .DB 9\nx: .DB 7\n", 6, { 0x05, 0x00, 0x05, 0x00, 0x09, 0x07 } },
        { "v: .DFS 3\nw: .DB $w\n", 4, { 0x00, 0x00, 0x00, 0x03 } },
        // Una etiqueta tras una instrucción con valor temporal no es definitiva, pues el tamaño de
        // esa instrucción aún puede cambiar: l no se fija mientras .DFS $m mide 0
        { "z: .DFS $m\nl: .DB $l\na: .DFS $n\nb: .DB 9\nm: .CONST $b - $a\nn: .CONST 2\n", 6, { 0x00, 0x00, 0x02, 0x00, 0x00, 0x09 } },
        // Las referencias mutuas se resuelven cuando los valores dejan de cambiar
        { "v: .DB $w\nw: .DB $v\n", 2, { 0x01, 0x00 } },
    };
    return probar(casos);
}

MunitResult lily_directivas_test_errores(const MunitParameter params[], void *user_data) {
    (void) params; (void) user_data;
    assert_rechaza("v: .DB 256\n", COD_NUMS_MAS_8);
    assert_rechaza("v: .DB -129\n", COD_NUMS_MAS_8);
    assert_rechaza("v: .DWL 1, 0x10000\n", COD_NUMS_MAS_16);
    assert_rechaza("v: .DDM 0x100000000\n", COD_NUMS_MAS_32);
    assert_rechaza("v: .DB %A\n", COD_A_SEMANTICO_OPERANDO_OBJETO);
    return MUNIT_OK;
}

MunitResult lily_directivas_test_const_var(const MunitParameter params[], void *user_data) {
    (void) params; (void) user_data;
    static const struct caso casos[] = {
        { "v: .DDL $x\nx: .CONST 5\n", 4, { 0x05, 0, 0, 0 } },
        { "v: .DDL $x\nx: .VAR 3\n", 4, { 0x03, 0, 0, 0 } },
        // Una constante cuyo valor depende de otra definida antes o después
        { "v: .DDL $y\nx: .CONST 5\ny: .CONST $x + 1\n", 4, { 0x06, 0, 0, 0 } },
        { "v: .DDL $y\ny: .CONST $x + 1\nx: .CONST 5\n", 4, { 0x06, 0, 0, 0 } },
        // Una constante cuyo valor depende de una etiqueta de localidad posterior
        { "x: .CONST $e\nv: .DB 1, 2\ne: .DB 1\nw: .DB $x\n", 4, { 0x01, 0x02, 0x01, 0x02 } },
        // CONST y VAR no generan bytes ni avanzan PC
        { "x: .CONST 5\n", 0, { 0 } },
        { "x: .CONST 5\nv: .DB $v, $x\n", 2, { 0x00, 0x05 } },
        { "v: .DB 1\nx: .VAR 2\nw: .DB $w\n", 2, { 0x01, 0x01 } },
        // Una directiva que no genera bytes tampoco es un error
        { "v: .DFS 0\nw: .DB $w\n", 1, { 0x00 } },
    };
    return probar(casos);
}

MunitResult lily_directivas_test_redefinicion(const MunitParameter params[], void *user_data) {
    (void) params; (void) user_data;
    assert_rechaza("x: .CONST 5\nx: .CONST 6\n", COD_A_SEMANTICO_REDEF_CONSTANTE);
    assert_rechaza("v: .DDL $x\nx: .CONST 5\nx: .CONST 6\n", COD_A_SEMANTICO_REDEF_CONSTANTE);
    assert_rechaza("x: .VAR 5\nx: .VAR 6\n", COD_A_SEMANTICO_REDEF_VARIABLE);
    assert_rechaza("x: .CONST 5\nx: .DB 1\n", COD_A_SEMANTICO_REDEF_CONSTANTE);
    assert_rechaza("x: .DB 1\nx: .CONST 5\n", COD_A_SEMANTICO_REDEF_CONSTANTE);
    assert_rechaza("x: .DB 1\nx: .DB 2\n", COD_A_SEMANTICO_REDEF_CONSTANTE);
    // pc es el identificador del contador de programa
    assert_rechaza("pc: .DB 1\n", COD_A_SEMANTICO_REDEF_CONSTANTE);
    // Una definición circular nunca se reduce
    assert_rechaza("x: .CONST $x + 1\n", COD_A_SEMANTICO_MAXIMO_ITERACIONES);
    // Un valor que solo se usó temporalmente debe caber cuando se toma como definitivo
    assert_rechaza("v: .DB $x\nw: .DFS 300\nx: .DB 1\n", COD_NUMS_MAS_8);
    return MUNIT_OK;
}
