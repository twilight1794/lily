/*
 * @file a_lexico_ctipos.h
 * Solo una tabla para determinar el tipo de un caracter con una sola operación de bit.
 */

#ifndef LILY_C_A_LEXICO_CTIPOS
#define LILY_C_A_LEXICO_CTIPOS

/*
 * Bit 1: Caracter blanco
 * Bit 2: Operador
 */
char lex_ctipos[] = {
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
    0, // [
    0, // '\'
    0, // ]
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

#endif