/*
 * @file pow.h
 * Pequeñas macros para calcular potencias enteras conocidas rápidamente
 */

#ifndef LILY_C_POW
#define LILY_C_POW

long pow10_vals[] = {
    1, 10, 100, 1000, 10000,
    100000, 1000000, 10000000, 100000000, 1000000000,
    10000000000, 100000000000, 1000000000000,
    10000000000000, 100000000000000,1000000000000000,
    10000000000000000, 100000000000000000
};

#define pow2(n) (1<<((int) n))
#define pow8(n) (1<<(((int) n)*3))
#define pow10(n) pow10_vals[(int) n]
#define pow16(n) (1<<(((int) n)*4))

#endif
