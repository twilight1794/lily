/*
 * Copyright (C) 2025-2026 Giovanni Alfredo Garciliano Diaz
 *
 * This file is part of Lily.
 *
 * Lily is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Lily is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Lily. If not, see <https://www.gnu.org/licenses/>.
 */

/*
 * @file a_lexico_a_lexico_pow.h
 * Pequeñas macros para calcular potencias enteras conocidas rápidamente
 */

#ifndef LILY_L_A_LEXICO_POW
#define LILY_L_A_LEXICO_POW

#include <stdint.h>

static const uint64_t pow10_vals[] = {
    1, 10, 100, 1000, 10000,
    100000, 1000000, 10000000, 100000000, 1000000000,
    10000000000ULL, 100000000000ULL, 1000000000000ULL,
    10000000000000ULL, 100000000000000ULL, 1000000000000000ULL,
    10000000000000000ULL, 100000000000000000ULL, 1000000000000000000ULL,
    10000000000000000000ULL
};

// Las potencias se calculan en 64 bits; las que no caben en 64 bits valen 0

/**
 * Eleva 2 a la potencia \c n
 * @param n Exponente
 */
#define pow2(n) ((n) < 64 ? UINT64_C(1) << (n) : 0)

/**
 * Eleva 8 a la potencia \c n
 * @param n Exponente
 */
#define pow8(n) ((n) < 22 ? UINT64_C(1) << ((n)*3) : 0)

/**
 * Eleva 10 a la potencia \c n
 * @param n Exponente
 */
#define pow10(n) ((n) < 20 ? pow10_vals[(n)] : 0)

/**
 * Eleva 16 a la potencia \c n
 * @param n Exponente
 */
#define pow16(n) ((n) < 16 ? UINT64_C(1) << ((n)*4) : 0)

#endif
