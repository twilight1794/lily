/*
 * @file lua_int.h
 * Funciones para implementar enteros de tamaño exacto
 */

#ifndef LILY_L_LUA_INT
#define LILY_L_LUA_INT

#include "../common/defs.h"
#include "../common/nums.h"
#include "simbolo.h"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define lua_registertable(L, n, f) \
    (lua_pushcfunction(L, f), lua_setfield(L, -2, n))

// Definiciones de tipos
enum lily_lua_tipo {
    LILY_LUA_TNIL = 0,
    LILY_LUA_TBOOLEAN,
    LILY_LUA_TLIGHTUSERDATA, // Ignorado
    LILY_LUA_TNUMBER,
    LILY_LUA_TSTRING,
    LILY_LUA_TTABLE,
    LILY_LUA_TFUNCTION,
    LILY_LUA_TUSERDATA,
    LILY_LUA_TTHREAD, // Ignorado por ahora
    LILY_LUA_TINT8,
    LILY_LUA_TUINT8,
    LILY_LUA_TSINT8,
    LILY_LUA_TINT16B,
    LILY_LUA_TINT16L,
    LILY_LUA_TUINT16B,
    LILY_LUA_TUINT16L,
    LILY_LUA_TSINT16B,
    LILY_LUA_TSINT16L,
    LILY_LUA_TINT32B,
    LILY_LUA_TINT32L,
    LILY_LUA_TUINT32B,
    LILY_LUA_TUINT32L,
    LILY_LUA_TSINT32B,
    LILY_LUA_TSINT32L,
    LILY_LUA_TINT64B,
    LILY_LUA_TINT64L,
    LILY_LUA_TUINT64B,
    LILY_LUA_TUINT64L,
    LILY_LUA_TSINT64B,
    LILY_LUA_TSINT64L,
    // Esto no va al principio porque es conveniente que este enum sea compatible con las constantes ya definidas por Lua
    LILY_LUA_TINDETERMINADO
};

/**
 * Devuelve el tipo de un objeto de Lua, incluyendo los tipos definidos por Lily
 * @param L Sesión de Lua
 * @return Entero que representa al tipo
 */
enum lily_lua_tipo lily_lua_int_obj(lua_State* L);

/**
 * Devuelve la cadena que representa el tipo de un objeto de Lua, incluyendo los tipos definidos por Lily
 * @param tipo Constante que representa el tipo
 * @return Cadena, representación canónica del tipo
 */
char* lily_lua_int_tipo_cad(enum lily_lua_tipo tipo);

/**
 * Configura valores en variables según el tipo pasado
 * @paran tipo Constante que representa al tipo
 * @param [out] tamano Tamaño en bytes del tipo
 * @param [out] signo Rango de valores admitido según su signo: 0 para tipos positivos, 1 para negativos, 2 para el rango completo representable por un tamaño
 * @param [out] endianness 0 para big-endian, 1 para little-endian
 */
void lily_lua_int_tipo_valores(enum lily_lua_tipo tipo, size_t* tamano, int* signo, bool* endiannes);

// Constructores para enteros exactos

/**
 * @brief Comprueba si se puede crear un nuevo entero exacto con los datos proporcionados.
 * @param L Sesión de Lua
 * @param [out] bytes Buffer para escribir el valor codificado
 * @param tipo Tipo del entero a crear
 * @return Código de estado al término de la operación. \c COD_OK si se puede crear el tipo.
 */
enum lily_estado lily_lua_int_comp_argumentos(lua_State* L, uint8_t* bytes, enum lily_lua_tipo tipo);

/**
 * Crea un nuevo entero exacto.
 * El objeto creado queda en la pila.
 * @param L Sesión de Lua
 * @param bytes Valor a guardar en el entero, ya codificado
 * @param tipo Tipo del entero a crear
 */
void lily_lua_int_crear(lua_State* L, uint8_t* bytes, enum lily_lua_tipo tipo);
int lily_lua_int_int8(lua_State* L);
int lily_lua_int_uint8(lua_State* L);
int lily_lua_int_sint8(lua_State* L);
int lily_lua_int_int16b(lua_State* L);
int lily_lua_int_int16l(lua_State* L);
int lily_lua_int_uint16b(lua_State* L);
int lily_lua_int_uint16l(lua_State* L);
int lily_lua_int_sint16b(lua_State* L);
int lily_lua_int_sint16l(lua_State* L);
int lily_lua_int_int32b(lua_State* L);
int lily_lua_int_int32l(lua_State* L);
int lily_lua_int_uint32b(lua_State* L);
int lily_lua_int_uint32l(lua_State* L);
int lily_lua_int_sint32b(lua_State* L);
int lily_lua_int_sint32l(lua_State* L);
int lily_lua_int_int64b(lua_State* L);
int lily_lua_int_int64l(lua_State* L);
int lily_lua_int_uint64b(lua_State* L);
int lily_lua_int_uint64l(lua_State* L);
int lily_lua_int_sint64b(lua_State* L);
int lily_lua_int_sint64l(lua_State* L);

// Metamétodos para enteros exactos

/**
 * @brief Comprueba si los operandos son aptos para la operación
 * Los tipos exactos solo pueden operarse con otro tipo exacto de la misma clase, o con un entero nativo de Lua
 * @param L Sesión de Lua
 * @param[out] op1 Valor numérico del primer operando
 * @param[out] op2 Valor numérico del segundo operando, para operadores binarios
 * @return Tipo del entero exacto resultante de la operación
 */
enum lily_lua_tipo lily_lua_int_comp_operandos(lua_State* L, uint64_t* op1, uint64_t* op2);
int lily_lua_int_mt_add(lua_State* L);
int lily_lua_int_mt_sub(lua_State* L);
int lily_lua_int_mt_mul(lua_State* L);
//int lily_lua_int_mt_idiv(lua_State* L); // FIX: implementar división con decimales
int lily_lua_int_mt_mod(lua_State* L);
int lily_lua_int_mt_pow(lua_State* L);
int lily_lua_int_mt_unm(lua_State* L);
int lily_lua_int_mt_idiv(lua_State* L);
int lily_lua_int_mt_band(lua_State* L);
int lily_lua_int_mt_bor(lua_State* L);
int lily_lua_int_mt_bxor(lua_State* L);
int lily_lua_int_mt_bnot(lua_State* L);
int lily_lua_int_mt_shl(lua_State* L);
int lily_lua_int_mt_shr(lua_State* L);
//int lily_lua_int_mt_concat(lua_State* L); // FIX: implementar aplanado de tablas, conversión de cadenas
int lily_lua_int_mt_len(lua_State* L);
int lily_lua_int_mt_eq(lua_State* L);
int lily_lua_int_mt_lt(lua_State* L);
int lily_lua_int_mt_le(lua_State* L);
int lily_lua_int_mt_index(lua_State* L);
int lily_lua_int_mt_tostring(lua_State* L);

void lily_lua_int_preparar(lua_State* L);

#endif
