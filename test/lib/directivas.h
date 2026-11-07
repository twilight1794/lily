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


#ifndef LILY_T_DIRECTIVAS
#define LILY_T_DIRECTIVAS

#include <stdint.h>
#include <string.h>

#include "../../lib/munit/munit.h"
#include "../../src/lib/a_lexico.h"
#include "../../src/lib/a_semantico.h"
#include "../../src/lib/a_sintactico.h"

MunitResult lily_directivas_test_db(const MunitParameter params[], void *user_data);
MunitResult lily_directivas_test_dw_dd_dq(const MunitParameter params[], void *user_data);
MunitResult lily_directivas_test_referencia_adelantada(const MunitParameter params[], void *user_data);
MunitResult lily_directivas_test_errores(const MunitParameter params[], void *user_data);

#endif
