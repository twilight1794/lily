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

#include "../lib/munit/munit.h"

#include "common/bitarray.h"
#include "common/dict.h"

// lily_dict
static MunitTest lily_dict_test_tests[] = {
    {
        "/lily_dict_insert_test",
        lily_dict_test_insertar_leer,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/lily_dict_leer_inexistente",
        lily_dict_test_leer_inexistente,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/lily_dict_insercion_eliminacion_tamano",
        lily_dict_test_insercion_eliminacion_tamano,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/lily_dict_insertar_existente_descarte",
        lily_dict_test_insertar_existente_descarte,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/lily_dict_insertar_existente_guardado",
        lily_dict_test_insertar_existente_guardado,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/lily_dict_eliminacion_inexistente",
        lily_dict_test_eliminacion_inexistente,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/lily_dict_iteracion",
        lily_dict_test_iteracion,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite lily_dict_test_suite = {
    "/lily_dict",
    lily_dict_test_tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE
  };


// lily_bitarray
static MunitTest lily_bitarray_test_tests[] = {
    {
        "/lily_bitarray_obtener_alineado",
        lily_bitarray_test_obtener_alineado,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/lily_bitarray_obtener_dentro_byte",
        lily_bitarray_test_obtener_dentro_byte,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/lily_bitarray_obtener_cruzando_bytes",
        lily_bitarray_test_obtener_cruzando_bytes,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/lily_bitarray_obtener_16_desalineado",
        lily_bitarray_test_obtener_16_desalineado,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/lily_bitarray_obtener_no_excede",
        lily_bitarray_test_obtener_no_excede,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/lily_bitarray_obtener_exhaustivo",
        lily_bitarray_test_obtener_exhaustivo,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/lily_bitarray_guardar_alineado",
        lily_bitarray_test_guardar_alineado,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/lily_bitarray_guardar_dentro_byte",
        lily_bitarray_test_guardar_dentro_byte,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/lily_bitarray_guardar_cruzando_bytes",
        lily_bitarray_test_guardar_cruzando_bytes,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/lily_bitarray_guardar_un_bit",
        lily_bitarray_test_guardar_un_bit,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/lily_bitarray_guardar_exhaustivo",
        lily_bitarray_test_guardar_exhaustivo,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    {
        "/lily_bitarray_ida_vuelta",
        lily_bitarray_test_ida_vuelta,
        NULL,
        NULL,
        MUNIT_TEST_OPTION_NONE,
        NULL,
    },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite lily_bitarray_test_suite = {
    "/lily_bitarray",
    lily_bitarray_test_tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* const argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
    MunitSuite lily_test_suites[] = {
        lily_bitarray_test_suite,
        lily_dict_test_suite,
        { NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE }
    };
    MunitSuite lily_test_suite = {
        "",
        NULL,
        lily_test_suites,
        1,
        MUNIT_SUITE_OPTION_NONE
    };
    return munit_suite_main(&lily_test_suite, NULL, argc, argv);
}
