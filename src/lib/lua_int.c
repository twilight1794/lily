/*
 * Copyright (C) 2026 Giovanni Alfredo Garciliano Diaz
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

#include "lua_int.h"

/*
 * Indica si el valor en \a idx es un entero exacto, es decir, si tiene su metatabla
 */
static bool lily_lua_int_es_exacto(lua_State* L, int idx) {
    if (!lua_getmetatable(L, idx)) return false;
    lua_getglobal(L, "__imt");
    const bool exacto = lua_rawequal(L, -1, -2);
    lua_pop(L, 2);
    return exacto;
}

enum lily_lua_tipo lily_lua_int_obj(lua_State* L) {
    enum lily_lua_tipo tipo = LILY_LUA_TINDETERMINADO;
    int t = lua_type(L, -1);
    if (t == LUA_TUSERDATA) {
        // Primero vemos si es uno de nuestros tipos enteros
        if (!lily_lua_int_es_exacto(L, -1)) return LILY_LUA_TINDETERMINADO;
        lua_getiuservalue(L, -1, 1);
        const size_t tamano = (size_t) lua_tointeger(L, -1);
        lua_pop(L, 1);
        lua_getiuservalue(L, -1, 2);
        const int signo = (int) lua_tointeger(L, -1);
        lua_pop(L, 1);
        // Se guarda como entero: lua_toboolean consideraría verdadero también al 0
        lua_getiuservalue(L, -1, 3);
        const bool endianness = lua_tointeger(L, -1) != 0;
        lua_pop(L, 1);
        for (int i = LILY_LUA_TINT8; i <= LILY_LUA_TSINT64L; i++) {
            size_t t_tamano; int t_signo; bool t_endianness;
            lily_lua_int_tipo_valores((enum lily_lua_tipo) i, &t_tamano, &t_signo, &t_endianness);
            if (t_tamano == tamano && t_signo == signo && t_endianness == endianness) {
                tipo = (enum lily_lua_tipo) i;
                break;
            }
        }
    }
    // Si no, probablemente se trate de un tipo integrado
    else if (t == LUA_TNIL)      tipo = LILY_LUA_TNIL;
    else if (t == LUA_TNUMBER)   tipo = LILY_LUA_TNUMBER;
    else if (t == LUA_TBOOLEAN)  tipo = LILY_LUA_TBOOLEAN;
    else if (t == LUA_TSTRING)   tipo = LILY_LUA_TSTRING;
    else if (t == LUA_TTABLE)    tipo = LILY_LUA_TTABLE;
    else if (t == LUA_TFUNCTION) tipo = LILY_LUA_TFUNCTION;
    return tipo;
}

char* lily_lua_int_tipo_cad(enum lily_lua_tipo tipo) {
    char* cad = NULL;
    if (tipo == LILY_LUA_TNIL)           cad = "nil";
    else if (tipo == LILY_LUA_TBOOLEAN)  cad = "boolean";
    else if (tipo == LILY_LUA_TNUMBER)   cad = "number";
    else if (tipo == LILY_LUA_TSTRING)   cad = "string";
    else if (tipo == LILY_LUA_TTABLE)    cad = "table";
    else if (tipo == LILY_LUA_TFUNCTION) cad = "function";
    else if (tipo == LILY_LUA_TINT8)     cad = "int8";
    else if (tipo == LILY_LUA_TUINT8)    cad = "uint8";
    else if (tipo == LILY_LUA_TSINT8)    cad = "sint8";
    else if (tipo == LILY_LUA_TINT16B)   cad = "int16b";
    else if (tipo == LILY_LUA_TINT16L)   cad = "int16l";
    else if (tipo == LILY_LUA_TUINT16B)  cad = "uint16b";
    else if (tipo == LILY_LUA_TUINT16L)  cad = "uint16l";
    else if (tipo == LILY_LUA_TSINT16B)  cad = "sint16b";
    else if (tipo == LILY_LUA_TSINT16L)  cad = "sint16l";
    else if (tipo == LILY_LUA_TINT32B)   cad = "int32b";
    else if (tipo == LILY_LUA_TINT32L)   cad = "int32l";
    else if (tipo == LILY_LUA_TUINT32B)  cad = "uint32b";
    else if (tipo == LILY_LUA_TUINT32L)  cad = "uint32l";
    else if (tipo == LILY_LUA_TSINT32B)  cad = "sint32b";
    else if (tipo == LILY_LUA_TSINT32L)  cad = "sint32l";
    else if (tipo == LILY_LUA_TINT64B)   cad = "int64b";
    else if (tipo == LILY_LUA_TINT64L)   cad = "int64l";
    else if (tipo == LILY_LUA_TUINT64B)  cad = "uint64b";
    else if (tipo == LILY_LUA_TUINT64L)  cad = "uint64l";
    else if (tipo == LILY_LUA_TSINT64B)  cad = "sint64b";
    else if (tipo == LILY_LUA_TSINT64L)  cad = "sint64l";
    return cad;
}

void lily_lua_int_tipo_valores(enum lily_lua_tipo tipo, size_t* tamano, int* signo, bool* endianness) {
#define opc(VAR, VALOR) if (VAR != NULL) { *VAR = VALOR; }
    if (tipo == LILY_LUA_TINT8)         { opc(tamano, 1) opc(signo, 2) opc(endianness, 1) }
    else if (tipo == LILY_LUA_TUINT8)   { opc(tamano, 1) opc(signo, 0) opc(endianness, 1) }
    else if (tipo == LILY_LUA_TSINT8)   { opc(tamano, 1) opc(signo, 1) opc(endianness, 1) }
    else if (tipo == LILY_LUA_TINT16B)  { opc(tamano, 2) opc(signo, 2) opc(endianness, 0) }
    else if (tipo == LILY_LUA_TINT16L)  { opc(tamano, 2) opc(signo, 2) opc(endianness, 1) }
    else if (tipo == LILY_LUA_TUINT16B) { opc(tamano, 2) opc(signo, 0) opc(endianness, 0) }
    else if (tipo == LILY_LUA_TUINT16L) { opc(tamano, 2) opc(signo, 0) opc(endianness, 1) }
    else if (tipo == LILY_LUA_TSINT16B) { opc(tamano, 2) opc(signo, 1) opc(endianness, 0) }
    else if (tipo == LILY_LUA_TSINT16L) { opc(tamano, 2) opc(signo, 1) opc(endianness, 1) }
    else if (tipo == LILY_LUA_TINT32B)  { opc(tamano, 4) opc(signo, 2) opc(endianness, 0) }
    else if (tipo == LILY_LUA_TINT32L)  { opc(tamano, 4) opc(signo, 2) opc(endianness, 1) }
    else if (tipo == LILY_LUA_TUINT32B) { opc(tamano, 4) opc(signo, 0) opc(endianness, 0) }
    else if (tipo == LILY_LUA_TUINT32L) { opc(tamano, 4) opc(signo, 0) opc(endianness, 1) }
    else if (tipo == LILY_LUA_TSINT32B) { opc(tamano, 4) opc(signo, 1) opc(endianness, 0) }
    else if (tipo == LILY_LUA_TSINT32L) { opc(tamano, 4) opc(signo, 1) opc(endianness, 1) }
    else if (tipo == LILY_LUA_TINT64B)  { opc(tamano, 8) opc(signo, 2) opc(endianness, 0) }
    else if (tipo == LILY_LUA_TINT64L)  { opc(tamano, 8) opc(signo, 2) opc(endianness, 1) }
    else if (tipo == LILY_LUA_TUINT64B) { opc(tamano, 8) opc(signo, 0) opc(endianness, 0) }
    else if (tipo == LILY_LUA_TUINT64L) { opc(tamano, 8) opc(signo, 0) opc(endianness, 1) }
    else if (tipo == LILY_LUA_TSINT64B) { opc(tamano, 8) opc(signo, 1) opc(endianness, 0) }
    else if (tipo == LILY_LUA_TSINT64L) { opc(tamano, 8) opc(signo, 1) opc(endianness, 1) }
}

enum lily_estado lily_lua_int_comp_argumentos(lua_State* L, uint8_t* bytes, enum lily_lua_tipo tipo) {
    size_t tamano; int signo; bool endianness;
    char msg[41];
    uint64_t val;
    enum lily_estado estado;
    if (lua_gettop(L) != 1) {
        snprintf(msg, 38, "%s requiere un argumento", lily_lua_int_tipo_cad(tipo));
        lua_pushstring(L, msg);
        lua_error(L);
    }
    if (lua_type(L, 1) != LUA_TNUMBER) {
        snprintf(msg, 40, "%s requiere un argumento numérico", lily_lua_int_tipo_cad(tipo));
        lua_pushstring(L, msg);
        lua_error(L);
    }
    val = lua_tointeger(L, 1);
    lily_lua_int_tipo_valores(tipo, &tamano, &signo, &endianness);
    estado = lily_nums_codificar_num(bytes, &val, tamano, signo, endianness);
    if (estado != COD_OK) {
        snprintf(msg, 38, "Error al codificar %s: %d\n", lily_lua_int_tipo_cad(tipo), estado);
        lua_pushstring(L, msg);
        lua_error(L);
    }
    return COD_OK;
}

void lily_lua_int_crear(lua_State* L, uint8_t* bytes, enum lily_lua_tipo tipo) {
    void* buf = lua_newuserdatauv(L, 8, 3);
    int idx = lua_gettop(L);
    size_t tamano; int signo; bool endianness;
    lily_lua_int_tipo_valores(tipo, &tamano, &signo, &endianness);
    // Guardar datos del tipo
    lua_pushinteger(L, (lua_Integer) tamano);
    lua_setiuservalue(L, idx, 1);
    lua_pushinteger(L, (lua_Integer) signo);
    lua_setiuservalue(L, idx, 2);
    lua_pushinteger(L, (lua_Integer) endianness);
    lua_setiuservalue(L, idx, 3);
    // Guardar valor modificado
    memset(buf, 0, 8); // TODO: ¿esto es necesario?
    memcpy(buf, bytes, tamano);
    // Asignar metatabla
    lua_getglobal(L, "__imt");
    lua_setmetatable(L, idx);
}

int lily_lua_int_int8(lua_State* L) {
    uint8_t val;
    lily_lua_int_comp_argumentos(L, &val, LILY_LUA_TINT8);
    lily_lua_int_crear(L, &val, LILY_LUA_TINT8);
    return 1;
}

int lily_lua_int_uint8(lua_State* L) {
    uint8_t val;
    lily_lua_int_comp_argumentos(L, &val, LILY_LUA_TUINT8);
    lily_lua_int_crear(L, &val, LILY_LUA_TUINT8);
    return 1;
}

int lily_lua_int_sint8(lua_State* L) {
    uint8_t val;
    lily_lua_int_comp_argumentos(L, &val, LILY_LUA_TSINT8);
    lily_lua_int_crear(L, &val, LILY_LUA_TSINT8);
    return 1;
}

int lily_lua_int_int16b(lua_State* L) {
    uint8_t val[2];
    lily_lua_int_comp_argumentos(L, val, LILY_LUA_TINT16B);
    lily_lua_int_crear(L, val, LILY_LUA_TINT16B);
    return 1;
}

int lily_lua_int_int16l(lua_State* L) {
    uint8_t val[2];
    lily_lua_int_comp_argumentos(L, val, LILY_LUA_TINT16L);
    lily_lua_int_crear(L, val, LILY_LUA_TINT16L);
    return 1;
}

int lily_lua_int_uint16b(lua_State* L) {
    uint8_t val[2];
    lily_lua_int_comp_argumentos(L, val, LILY_LUA_TUINT16B);
    lily_lua_int_crear(L, val, LILY_LUA_TUINT16B);
    return 1;
}

int lily_lua_int_uint16l(lua_State* L) {
    uint8_t val[2];
    lily_lua_int_comp_argumentos(L, val, LILY_LUA_TUINT16L);
    lily_lua_int_crear(L, val, LILY_LUA_TUINT16L);
    return 1;
}

int lily_lua_int_sint16b(lua_State* L) {
    uint8_t val[2];
    lily_lua_int_comp_argumentos(L, val, LILY_LUA_TSINT16B);
    lily_lua_int_crear(L, val, LILY_LUA_TSINT16B);
    return 1;
}

int lily_lua_int_sint16l(lua_State* L) {
    uint8_t val[2];
    lily_lua_int_comp_argumentos(L, val, LILY_LUA_TSINT16L);
    lily_lua_int_crear(L, val, LILY_LUA_TSINT16L);
    return 1;
}

int lily_lua_int_int32b(lua_State* L) {
    uint8_t val[4];
    lily_lua_int_comp_argumentos(L, val, LILY_LUA_TINT32B);
    lily_lua_int_crear(L, val, LILY_LUA_TINT32B);
    return 1;
}

int lily_lua_int_int32l(lua_State* L) {
    uint8_t val[4];
    lily_lua_int_comp_argumentos(L, val, LILY_LUA_TINT32L);
    lily_lua_int_crear(L, val, LILY_LUA_TINT32L);
    return 1;
}

int lily_lua_int_uint32b(lua_State* L) {
    uint8_t val[4];
    lily_lua_int_comp_argumentos(L, val, LILY_LUA_TUINT32B);
    lily_lua_int_crear(L, val, LILY_LUA_TUINT32B);
    return 1;
}

int lily_lua_int_uint32l(lua_State* L) {
    uint8_t val[4];
    lily_lua_int_comp_argumentos(L, val, LILY_LUA_TUINT32L);
    lily_lua_int_crear(L, val, LILY_LUA_TUINT32L);
    return 1;
}

int lily_lua_int_sint32b(lua_State* L) {
    uint8_t val[4];
    lily_lua_int_comp_argumentos(L, val, LILY_LUA_TSINT32B);
    lily_lua_int_crear(L, val, LILY_LUA_TSINT32B);
    return 1;
}

int lily_lua_int_sint32l(lua_State* L) {
    uint8_t val[4];
    lily_lua_int_comp_argumentos(L, val, LILY_LUA_TSINT32L);
    lily_lua_int_crear(L, val, LILY_LUA_TSINT32L);
    return 1;
}

int lily_lua_int_int64b(lua_State* L) {
    uint8_t val[8];
    lily_lua_int_comp_argumentos(L, val, LILY_LUA_TINT64B);
    lily_lua_int_crear(L, val, LILY_LUA_TINT64B);
    return 1;
}

int lily_lua_int_int64l(lua_State* L) {
    uint8_t val[8];
    lily_lua_int_comp_argumentos(L, val, LILY_LUA_TINT64L);
    lily_lua_int_crear(L, val, LILY_LUA_TINT64L);
    return 1;
}

int lily_lua_int_uint64b(lua_State* L) {
    uint8_t val[8];
    lily_lua_int_comp_argumentos(L, val, LILY_LUA_TUINT64B);
    lily_lua_int_crear(L, val, LILY_LUA_TUINT64B);
    return 1;
}

int lily_lua_int_uint64l(lua_State* L) {
    uint8_t val[8];
    lily_lua_int_comp_argumentos(L, val, LILY_LUA_TUINT64L);
    lily_lua_int_crear(L, val, LILY_LUA_TUINT64L);
    return 1;
}

int lily_lua_int_sint64b(lua_State* L) {
    uint8_t val[8];
    lily_lua_int_comp_argumentos(L, val, LILY_LUA_TSINT64B);
    lily_lua_int_crear(L, val, LILY_LUA_TSINT64B);
    return 1;
}

int lily_lua_int_sint64l(lua_State* L) {
    uint8_t val[8];
    lily_lua_int_comp_argumentos(L, val, LILY_LUA_TSINT64L);
    lily_lua_int_crear(L, val, LILY_LUA_TSINT64L);
    return 1;
}

int lily_lua_int_mt_len(lua_State* L) {
    // Tamaño en bytes del entero exacto
    lua_getiuservalue(L, 1, 1);
    return 1;
}

int lily_lua_int_mt_eq(lua_State* L) {
    // Dos enteros exactos son iguales si son del mismo tipo (tamaño, signo y endianness) y
    // tienen la misma representación binaria. Lua solo llama a __eq si ambos operandos son
    // userdata, así que la comparación con un número nativo siempre es falsa
    if (!lily_lua_int_es_exacto(L, 1) || !lily_lua_int_es_exacto(L, 2)) {
        lua_pushboolean(L, false);
        return 1;
    }
    for (int i = 1; i <= 3; i++) {
        lua_getiuservalue(L, 1, i);
        lua_getiuservalue(L, 2, i);
        const bool igual = lua_rawequal(L, -1, -2);
        lua_pop(L, 2);
        if (!igual) {
            lua_pushboolean(L, false);
            return 1;
        }
    }
    lua_getiuservalue(L, 1, 1);
    const size_t tamano = (size_t) lua_tointeger(L, -1);
    lua_pop(L, 1);
    lua_pushboolean(L, memcmp(lua_touserdata(L, 1), lua_touserdata(L, 2), tamano) == 0);
    return 1;
}

int lily_lua_int_mt_index(lua_State* L) {
    uint8_t bytes[8] = {0};
    if (!lua_isinteger(L, -1)) {
        lua_pushstring(L, "La clave solo puede ser un número entero");
        lua_error(L);
    }
    uint64_t idx = lua_tointeger(L, -1);
    lua_pop(L, 1);
    lua_getiuservalue(L, -1, 1);
    uint64_t tam = lua_tointeger(L, -1);
    lua_pop(L, 1);
    if (idx >= tam) {
        lua_pushstring(L, "Se intentó pedir un índice más allá de los límites");
        lua_error(L);
    }
    uint8_t* obj = (uint8_t*) lua_touserdata(L, -1);
    bytes[0] = *(obj+idx);
    lily_lua_int_crear(L, bytes, LILY_LUA_TUINT8);
    return 1;
}

int lily_lua_int_mt_tostring(lua_State *L) {
    lua_getiuservalue(L, 1, 1);
    const size_t tamano = (size_t) lua_tointeger(L, -1);
    lua_getiuservalue(L, 1, 3);
    const bool endianness = lua_tointeger(L, -1) != 0;
    lua_pop(L, 2);
    const uint8_t* obj = (const uint8_t*) lua_touserdata(L, 1);
    static const char cifras[] = "0123456789abcdef";
    char cad[19] = "0x"; // <- max. 16 del valor, + 2 del prefijo
    size_t j = 2;
    for (size_t i = 0; i < tamano && i < 8; i++) {
        // En little-endian, el byte más significativo es el último
        const uint8_t byte = obj[endianness ? (tamano - 1 - i) : i];
        cad[j++] = cifras[byte >> 4];
        cad[j++] = cifras[byte & 0x0F];
    }
    cad[j] = 0;
    lua_pushstring(L, cad);
    return 1;
}

void lily_lua_int_preparar(lua_State* L) {
    lua_register(L, "int8", lily_lua_int_int8);
    lua_register(L, "uint8", lily_lua_int_uint8);
    lua_register(L, "sint8", lily_lua_int_sint8);
    lua_register(L, "int16b", lily_lua_int_int16b);
    lua_register(L, "int16l", lily_lua_int_int16l);
    lua_register(L, "uint16b", lily_lua_int_uint16b);
    lua_register(L, "uint16l", lily_lua_int_uint16l);
    lua_register(L, "sint16b", lily_lua_int_sint16b);
    lua_register(L, "sint16l", lily_lua_int_sint16l);
    lua_register(L, "int32b", lily_lua_int_int32b);
    lua_register(L, "int32l", lily_lua_int_int32l);
    lua_register(L, "uint32b", lily_lua_int_uint32b);
    lua_register(L, "uint32l", lily_lua_int_uint32l);
    lua_register(L, "sint32b", lily_lua_int_sint32b);
    lua_register(L, "sint32l", lily_lua_int_sint32l);
    lua_register(L, "int64b", lily_lua_int_int64b);
    lua_register(L, "int64l", lily_lua_int_int64l);
    lua_register(L, "uint64b", lily_lua_int_uint64b);
    lua_register(L, "uint64l", lily_lua_int_uint64l);
    lua_register(L, "sint64b", lily_lua_int_sint64b);
    lua_register(L, "sint64l", lily_lua_int_sint64l);
    lua_createtable(L, 0, 4);
    lua_registertable(L, "__len", lily_lua_int_mt_len);
    lua_registertable(L, "__eq", lily_lua_int_mt_eq);
    lua_registertable(L, "__index", lily_lua_int_mt_index);
    lua_registertable(L, "__tostring", lily_lua_int_mt_tostring);
    lua_setglobal(L, "__imt");
    //lua_setglobal(L, "L");
}
