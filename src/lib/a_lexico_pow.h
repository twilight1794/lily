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

static long long pow10_vals[] = {
    1, 10, 100, 1000, 10000,
    100000, 1000000, 10000000, 100000000, 1000000000,
    10000000000LL, 100000000000LL, 1000000000000LL,
    10000000000000LL, 100000000000000LL, 1000000000000000LL,
    10000000000000000LL, 100000000000000000LL
};

/**
 * Eleva \c n a la potencia 2
 * @param n Valor a elevar
 */
#define pow2(n) (1<<((int) n))

/**
 * Eleva \c n a la potencia 8
 * @param n Valor a elevar
 */
#define pow8(n) (1<<(((int) n)*3))

/**
 * Eleva \c n a la potencia 10
 * @param n Valor a elevar
 */
#define pow10(n) pow10_vals[(int) n]

/**
 * Eleva \c n a la potencia 16
 * @param n Valor a elevar
 */
#define pow16(n) (1<<(((int) n)*4))

#endif
