#include "dict.h"

MunitResult lily_dict_test_insertar_leer(const MunitParameter params[], void *user_data) {
    struct lily_dict_dict* dict = lily_dict_create();
    munit_assert_not_null(dict);
    lily_dict_insert(dict, "nombre", "Nina", NULL);
    struct lily_dict_nodo* nodo = lily_dict_get(dict, "nombre");
    munit_assert_not_null(nodo);
    munit_assert_string_equal((char*) nodo->valor, "Nina");
    free(nodo);
    free(dict);
    return MUNIT_OK;
}

MunitResult lily_dict_test_leer_inexistente(const MunitParameter params[], void *user_data) {
    struct lily_dict_dict* dict = lily_dict_create();
    munit_assert_not_null(dict);
    struct lily_dict_nodo* nodo1 = lily_dict_insert(dict, "nombre", "Nina", NULL);
    munit_assert_not_null(nodo1);
    struct lily_dict_nodo* nodo2 = lily_dict_get(dict, "apellido");
    munit_assert_null(nodo2);
    free(nodo1);
    free(nodo2);
    free(dict);
    return MUNIT_OK;
}

MunitResult lily_dict_test_insercion_eliminacion_tamano(const MunitParameter params[], void *user_data) {
    struct lily_dict_dict* dict = lily_dict_create();
    munit_assert_not_null(dict);
    munit_assert_size(lily_dict_size(dict), ==, 0);
    struct lily_dict_nodo* nodo1 = lily_dict_insert(dict, "nombre", "Nina", NULL);
    munit_assert_not_null(nodo1);
    munit_assert_size(lily_dict_size(dict), ==, 1);
    struct lily_dict_nodo* nodo2 = lily_dict_insert(dict, "apellido", "Simonetti", NULL);
    munit_assert_not_null(nodo2);
    munit_assert_size(lily_dict_size(dict), ==, 2);
    struct lily_dict_nodo* nodo3 = lily_dict_insert(dict, "fnac", "30 ago 1996", NULL);
    munit_assert_not_null(nodo3);
    munit_assert_size(lily_dict_size(dict), ==, 3);

    munit_assert_int(LILY_DICT_OK, ==, lily_dict_remove(dict, "fnac"));
    munit_assert_size(lily_dict_size(dict), ==, 2);
    munit_assert_int(LILY_DICT_OK, ==, lily_dict_remove(dict, "apellido"));
    munit_assert_size(lily_dict_size(dict), ==, 1);
    munit_assert_int(LILY_DICT_OK, ==, lily_dict_remove(dict, "nombre"));
    munit_assert_size(lily_dict_size(dict), ==, 0);
    return MUNIT_OK;
}

MunitResult lily_dict_test_insertar_existente_descarte(const MunitParameter params[], void *user_data) {
    struct lily_dict_dict* dict = lily_dict_create();
    munit_assert_not_null(dict);
    struct lily_dict_nodo* nodo = lily_dict_insert(dict, "nombre", "Nina", NULL);
    munit_assert_not_null(nodo);
    munit_assert_string_equal((char*) nodo->valor, "Nina");
    nodo = lily_dict_insert(dict, "nombre", "Mina", NULL);
    munit_assert_string_equal((char*) nodo->valor, "Mina");
    free(nodo);
    free(dict);
    return MUNIT_OK;
}

MunitResult lily_dict_test_insertar_existente_guardado(const MunitParameter params[], void *user_data) {
    struct lily_dict_dict* dict = lily_dict_create();
    munit_assert_not_null(dict);
    struct lily_dict_nodo* nodo = lily_dict_insert(dict, "nombre", "Nina", NULL);
    munit_assert_not_null(nodo);
    munit_assert_string_equal((char*) nodo->valor, "Nina");
    char* anterior;
    nodo = lily_dict_insert(dict, "nombre", "Mina", ((void *) &anterior));
    munit_assert_string_equal((char*) nodo->valor, "Mina");
    munit_assert_string_equal((char*) anterior, "Nina");
    free(nodo);
    free(dict);
    return MUNIT_OK;
}

MunitResult lily_dict_test_eliminacion_inexistente(const MunitParameter params[], void *user_data) {
    struct lily_dict_dict* dict = lily_dict_create();
    munit_assert_not_null(dict);
    struct lily_dict_nodo* nodo = lily_dict_insert(dict, "nombre", "Nina", NULL);
    munit_assert_not_null(nodo);
    size_t tam_antes = lily_dict_size(dict);
    lily_dict_remove(dict, "escuela");
    size_t tam_despues = lily_dict_size(dict);
    munit_assert_size(tam_antes, ==, tam_despues);
    free(nodo);
    free(dict);
    return MUNIT_OK;
}