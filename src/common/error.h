/**
 * @file error.h
 * Códigos de error de Lily
 */

#ifndef LILY_C_ERROR
#define LILY_C_ERROR

enum lily_error {
    COD_OK,
    // Generales
    COD_MALLOC_FALLO, /**< [mc]alloc falló al reservar memoria */
    COD_CARACTER_INVALIDO, /**< Caracter inválido en el archivo */
    COD_CTX_NULO, /**< El contexto pasado a una función es nulo */
    // lib/a_lexico.h
    COD_A_LEXICO_RECON_ERRONEO, /**< La cadena no corresponde a un tipo, durante un estado de ambigüedad */
    COD_A_LEXICO_DIRECTIVA_INVALIDA, /**< La directiva especificada no existe */
    COD_A_LEXICO_MNEMONICO_INVALIDO, /**< El mnemónico es inválido */
    COD_A_LEXICO_CARACTER_INVALIDO, /**< Caracter inválido durante el procesamiento de un tipo */
    COD_A_LEXICO_FIN_INESPERADO, /**< El archivo terminó antes de lo esperado */
    // lib/a_sintactico.h
    COD_A_SINTACTICO_SIN_SIMBOLOS, /**< No hubo símbolos reconocidos en el análisis léxico */
    COD_A_SINTACTICO_FIN_INESPERADO, /**< La lista de símbolos terminó antes de lo esperado */
    COD_A_SINTACTICO_SIMBOLO_INICIAL_INVALIDO, /**< El símbolo al inicio no es un mnemónico, una etiqueta, o una directiva */
    COD_A_SINTACTICO_ETIQUETA_MULTIPLE_LINEA, /**< Hay más de una definición de etiqueta en la misma línea */
    COD_A_SINTACTICO_NO_CADENA_NUL, /**< Se esperaba una cadena nul, pero el símbolo no era */
    COD_A_SINTACTICO_PARENTESIS_DESBALANCEADOS, /**< Los paréntesis en la expresión no están balanceaados correctamente */
    COD_A_SINTACTICO_PARENTESIS_VACIOS, /**< Los paréntesis no están agrupando ningún símbolo */
    // lib/a_semantico.h
    COD_A_SEMANTICO_CPU_NO_ESPECIFICADO, /**< No se ha especificado la directiva CPU cuando se ha requerido */
    // lib/lua_cpu.h
    COD_LUA_CPU_DESC_NO_TABLA, /**< El objeto de descripción devuelto por el script no es una tabla */
    COD_LUA_CPU_DESC_NO_ID, /**< El miembro "id" no existe en el objeto de descripción */
    COD_LUA_CPU_DESC_ID_NO_CADENA, /**< El miembro "cadena" del objeto de descripción no es una cadena */
    COD_LUA_CPU_DESC_NO_REGISTROS, /**< El miembro "registros" no existe en el objeto de descripción */
    COD_LUA_CPU_DESC_REGISTROS_NO_TABLA, /**< El miembro "registros" del objeto de descripción no es una tabla */
    COD_LUA_CPU_DESC_NO_OPERANDOS, /**< El miembro "operandos" no existe en el objeto de descripción */
    COD_LUA_CPU_DESC_OPERANDOS_NO_TABLA, /**< El miembro "operandos" del objeto de descripción no es una tabla */
    COD_LUA_CPU_DESC_NO_ENSAMBLE, /**< El miembro "ensamble" no existe en el objeto de descripción */
    COD_LUA_CPU_DESC_ENSAMBLE_NO_TABLA, /**< El miembro "ensamble" del objeto de descripción no es una tabla */
    COD_LUA_CPU_DESC_NO_OPCODES, /**< El miembro "opcodes" no existe en el objeto de descripción */
    COD_LUA_CPU_DESC_OPCODES_NO_TABLA, /**< El miembro "opcodes" del objeto de descripción no es una tabla */
    COD_LUA_CPU_DESC_NO_DESENSAMBLE, /**< El miembro "desensamble" no existe en el objeto de descripción */
    COD_LUA_CPU_DESC_DESENSAMBLE_NO_TABLA, /**< El miembro "desensamble" del objeto de descripción no es una tabla */
};

#endif
