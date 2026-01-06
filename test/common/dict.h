#ifndef LILY_T_DICT
#define LILY_T_DICT

#include "../../lib/munit/munit.h"
#include "../../src/common/dict.h"

MunitResult lily_dict_test_insertar_leer(const MunitParameter params[], void *user_data);
MunitResult lily_dict_test_leer_inexistente(const MunitParameter params[], void *user_data);
MunitResult lily_dict_test_insercion_eliminacion_tamano(const MunitParameter params[], void *user_data);
MunitResult lily_dict_test_insertar_existente_descarte(const MunitParameter params[], void *user_data);
MunitResult lily_dict_test_insertar_existente_guardado(const MunitParameter params[], void *user_data);
MunitResult lily_dict_test_eliminacion_inexistente(const MunitParameter params[], void *user_data);

#endif
