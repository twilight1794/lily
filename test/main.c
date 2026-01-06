#include "../lib/munit/munit.h"

#include "common/dict.h"

/*MunitSuite lily_test_suites[] = {
    lily_dict_test_suite,
    NULL
};

MunitSuite lily_test_suite = {
    "",
    NULL,
    lily_test_suites,
    1,
    MUNIT_SUITE_OPTION_NONE
};*/

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
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static MunitSuite lily_dict_test_suite = {
    "/lily_dict",
    lily_dict_test_tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE
  };

int main(int argc, char* const argv[MUNIT_ARRAY_PARAM(argc + 1)]) {
    return munit_suite_main(&lily_dict_test_suite, NULL, argc, argv);
}