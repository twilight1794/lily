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
 * @file a_lexico_ctipos.h
 * Solo una tabla para determinar el tipo de un caracter con una sola operación de bit.
 */

#ifndef LILY_L_A_LEXICO_CTIPOS
#define LILY_L_A_LEXICO_CTIPOS

/*
 * Bit 1: Caracter blanco
 * Bit 2: Operador
 */
static char lily_a_lexico_ctipos[] = {
    0, // NUL
    0, // SH
    0, // STX
    0, // ETX
    0, // EOT
    0, // ENQ
    0, // ACK
    0, // BEL
    0, // BS
    1, // HT
    1, // LF
    0, // VT
    0, // FF
    1, // CR
    0, // SO
    0, // SI
    0, // DLE
    0, // DC1
    0, // DC2
    0, // DC3
    0, // DC4
    0, // NAK
    0, // SYN
    0, // ETB
    0, // CAN
    0, // EM
    0, // SUB
    0, // ESC
    0, // FS
    0, // GS
    0, // RS
    0, // US
    1, // SPACE
    2, // !
    0, // "
    0, // #
    0, // $
    0, // %
    2, // &
    0, // '
    2, // (
    2, // )
    2, // *
    2, // +
    2, // ,
    2, // -
    2, // .
    2, // /
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0-9
    0, // :
    0, // ;
    2, // <
    2, // =
    2, // >
    0, // ?
    0, // @
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // A-Z
    2, // [
    0, // '\'
    2, // ]
    2, // ^
    0, // _
    0, // `
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // a-z
    0, // {
    2, // |
    0, // }
    2, // ~
    0, // DEL
};

/**
 * Comprueba si un caracter \a c es un espacio (0x20), tabulador (0x9), salto de línea (0xa), o retorno de carro (0xd)
 * @param c Caracter a analizar
 */
#define lex_esblanco(c) (lily_a_lexico_ctipos[(unsigned char) c]&1)

/**
 * Comprueba si un caracter \a c puede ser parte de un operador
 * @param c Caracter a analizar
 */
#define lex_esoperador(c) (lily_a_lexico_ctipos[(unsigned char) c]&2)

#endif
