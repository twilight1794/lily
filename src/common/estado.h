/**
 * @file estado.h
 * Códigos de estado de Lily
 */

#ifndef LILY_C_ESTADO
#define LILY_C_ESTADO

#include "../lib/simbolo.h"

enum lily_estado {
    COD_OK, /**< Todo bien */
    // Generales
    COD_MALLOC_FALLO, /**< [mc]alloc falló al reservar memoria */
    COD_CARACTER_INVALIDO, /**< Caracter inválido en el archivo */
    COD_CTX_NULO, /**< El contexto pasado a una función es nulo */
    COD_NO_IMPLEMENTADO, /**< Se encontró una situación que aún no ha sido implementada */
    // common/nums.h
    COD_NUMS_MAS_8, /**< El número que se intenta guardar no cabe en 8 bits */
    COD_NUMS_MAS_16, /**< El número que se intenta guardar no cabe en 16 bits */
    COD_NUMS_MAS_32, /**< El número que se intenta guardar no cabe en 32 bits */
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
    COD_A_SINTACTICO_DESPLAZAMIENTO_ANIDADO, /**< Se ha encontrado un operador de apertura de desplazamiento dentro de un desplazamiento */
    COD_A_SINTACTICO_PARENTESIS_DESBALANCEADOS, /**< Los paréntesis en la expresión no están balanceaados correctamente */
    COD_A_SINTACTICO_PARENTESIS_VACIOS, /**< Los paréntesis no están agrupando ningún símbolo */
    COD_A_SINTACTICO_SIMBOLO_IMPROCEDENTE, /**< No se esperaba un símbolo de este tipo */
    // lib/a_semantico.h
    COD_A_SEMANTICO_CPU_NO_ESPECIFICADO, /**< No se ha especificado la directiva CPU cuando se ha requerido */
    COD_A_SEMANTICO_CPU_MULTIPLES_PARAMS, /**< La directiva CPU ha recibido más de un parámetro */
    COD_A_SEMANTICO_CPU_MULTIPLE, /**< Se ha utilizado la directiva CPU en otro punto diferente al inicio del archivo */
    COD_A_SEMANTICO_INSTRUCCION_SIN_ETI, /**< La directiva procesada necesita una etiqueta, y no ha sido provista */
    COD_A_SEMANTICO_DFS_MULTIPLES_PARAMS, /**< La directiva DFS ha recibido más de un parámetro */
    COD_A_SEMANTICO_DFS_PARAM_NO_ENTERO, /**< La directiva DFS ha recibido un parámetro que no es entero */
    COD_A_SEMANTICO_CONST_VAR_MULTIPLES_PARAMS, /**< La directiva CONST o VAR ha recibido más de un parámetro */
    COD_A_SEMANTICO_CONST_VAR_PARAM_NO_ENTERO, /**< La directiva CONST o VAR ha recibido un parámetro que no es entero */
    COD_A_SEMANTICO_REDEF_CONSTANTE, /**< Se intentó redefinir un identificador declarado como constante */
    COD_A_SEMANTICO_REDEF_VARIABLE, /**< Se intentó redefinir un identificador declarado con una etiqueta de localidad */
    COD_A_SEMANTICO_FIN_USUARIO, /**< Una directiva STOP ha detenido el proceso de ensamble */
    COD_A_SEMANTICO_OPERANDOS_INSUFICIENTES, /**< Hace falta operandos en la pila para un operador  */
    COD_A_SEMANTICO_OPERANDO_OBJETO, /**< Se intentó operar sobre un objeto */
    // lib/lily.h
    COD_LILY_SIN_ESQUEMA, /**< No se pudo encontrar un esquema de procesador para realizar la operación */
    // lib/lua_cpu.h
    COD_LUA_CPU_LUA_ERR, /**< Error generado por la máquina virtual de Lua */
    COD_LUA_CPU_DESC_NO_TABLA, /**< El objeto de descripción devuelto por el script no es una tabla */
    COD_LUA_CPU_DESC_NO_ID, /**< El miembro "id" no existe en el objeto de descripción */
    COD_LUA_CPU_DESC_ID_NO_CADENA, /**< El miembro "cadena" del objeto de descripción no es una cadena */
    COD_LUA_CPU_DESC_NO_REGISTROS, /**< El miembro "registros" no existe en el objeto de descripción */
    COD_LUA_CPU_DESC_REGISTROS_NO_TABLA, /**< El miembro "registros" del objeto de descripción no es una tabla */
    COD_LUA_CPU_DESC_NO_TIPOS, /**< El miembro "tipos" no existe en el objeto de descripción */
    COD_LUA_CPU_DESC_TIPOS_NO_TABLA, /**< El miembro "tipos" del objeto de descripción no es una tabla */
    COD_LUA_CPU_DESC_NO_ENSAMBLE, /**< El miembro "ensamble" no existe en el objeto de descripción */
    COD_LUA_CPU_DESC_ENSAMBLE_NO_TABLA, /**< El miembro "ensamble" del objeto de descripción no es una tabla */
    COD_LUA_CPU_MNEMO_REDIRECCION_MULTIPLE, /**< Se ha redirigido a un mnemónico más de una vez */
    COD_LUA_CPU_DESC_NO_OPCODES, /**< El miembro "opcodes" no existe en el objeto de descripción */
    COD_LUA_CPU_DESC_OPCODES_NO_TABLA, /**< El miembro "opcodes" del objeto de descripción no es una tabla */
    COD_LUA_CPU_DESC_NO_DESENSAMBLE, /**< El miembro "desensamble" no existe en el objeto de descripción */
    COD_LUA_CPU_DESC_DESENSAMBLE_NO_TABLA, /**< El miembro "desensamble" del objeto de descripción no es una tabla */
    COD_LUA_CPU_MNEMO_INEXISTENTE, /**< El mnemónico provisto no existe en el objeto de descripción */
    COD_LUA_CPU_CONJUNTO_ARGS_INEXISTENTE, /**< No existe una definición para procesar el conjunto de parámetros provisto para ese mnemónico */
    COD_LUA_CPU_TIPO_INEXISTENTE, /**< El identificador de tipo provisto no está definido */
    COD_LUA_CPU_TIPO_FUNCION_NO_BOOLEANO, /**< La función de comprobación de tipo no devolvió un valor booleano */
    COD_LUA_CPU_RES_ENSAMBLE_NO_TABLA, /**< El resultado de la función de ensamble no es una tabla */
    COD_LUA_CPU_RES_ENSAMBLE_VACIO, /**< La tabla devuelta por la función de ensamble no contiene ningún byte */
    COD_LUA_CPU_RES_ENSAMBLE_NO_ENTERO, /**< Uno de los elementos de la tabla devuelta por la función de ensamble no es un entero */
};

#endif
