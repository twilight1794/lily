#include "lua_int.h"

enum lily_lua_tipo lily_lua_int_obj(lua_State* L) {
    enum lily_lua_tipo tipo = LILY_LUA_TINDETERMINADO;
    int t = lua_type(L, -1);
    if (t == LUA_TUSERDATA) {
        // Primero vemos si es uno de nuestros tipos enteros
        lua_getiuservalue(L, -1, 1);
        size_t tamano = lua_tointeger(L, -1);
        lua_pop(L, 1);
        lua_getiuservalue(L, -1, 2);
        int signo = lua_tointeger(L, -1);
        lua_pop(L, 1);
        lua_getiuservalue(L, -1, 3);
        bool endianness = lua_toboolean(L, -1);
        lua_pop(L, 1);
        if (tamano == 1 && signo == 0) tipo = LILY_LUA_TINT8;
        else if (tamano == 1 && signo == 1) tipo = LILY_LUA_TUINT8;
        else if (tamano == 1 && signo == 2) tipo = LILY_LUA_TSINT8;
        else if (tamano == 2 && signo == 2) tipo = LILY_LUA_TINT16B;
        else if (tamano == 2 && signo == 2 && endianness) tipo = LILY_LUA_TINT16L;
        else if (tamano == 2 && signo == 0) tipo = LILY_LUA_TUINT16B;
        else if (tamano == 2 && signo == 0 && endianness) tipo = LILY_LUA_TUINT16L;
        else if (tamano == 2 && signo == 1) tipo = LILY_LUA_TSINT16B;
        else if (tamano == 2 && signo == 1 && endianness) tipo = LILY_LUA_TSINT16L;
        else if (tamano == 4 && signo == 2) tipo = LILY_LUA_TINT32B;
        else if (tamano == 4 && signo == 2 && endianness) tipo = LILY_LUA_TINT32L;
        else if (tamano == 4 && signo == 0) tipo = LILY_LUA_TUINT32B;
        else if (tamano == 4 && signo == 0 && endianness) tipo = LILY_LUA_TUINT32L;
        else if (tamano == 4 && signo == 1) tipo = LILY_LUA_TSINT32B;
        else if (tamano == 4 && signo == 1 && endianness) tipo = LILY_LUA_TSINT32L;
        else if (tamano == 8 && signo == 2) tipo = LILY_LUA_TINT64B;
        else if (tamano == 8 && signo == 2 && endianness) tipo = LILY_LUA_TINT64L;
        else if (tamano == 8 && signo == 0) tipo = LILY_LUA_TUINT64B;
        else if (tamano == 8 && signo == 0 && endianness) tipo = LILY_LUA_TUINT64L;
        else if (tamano == 8 && signo == 1) tipo = LILY_LUA_TSINT64B;
        else if (tamano == 8 && signo == 1 && endianness) tipo = LILY_LUA_TSINT64L;
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
    char msg[38];
    uint64_t val;
    enum lily_estado estado;
    if (lua_gettop(L) != 1) {
        snprintf(msg, 38, "%s requiere un argumento", lily_lua_int_tipo_cad(tipo));
        lua_pushstring(L, msg);
        lua_error(L);
    }
    if (lua_type(L, 1) != LUA_TNUMBER) {
        snprintf(msg, 38, "%s requiere un argumento numérico", lily_lua_int_tipo_cad(tipo));
        lua_pushstring(L, msg);
        lua_error(L);
    }
    val = lua_tointeger(L, -1);
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

enum lily_lua_tipo lily_lua_int_comp_operandos(lua_State* L, uint64_t* op1, uint64_t* op2) {
    enum lily_lua_tipo t_op1, t_op2;
    int exacto = 0; // Bandera para igualdad de tipos

    // Comprobar operando 1
    t_op1 = lily_lua_int_obj(L);
    if (t_op1 == LILY_LUA_TNUMBER)
        *op1 = lua_tointeger(L, -1-(op2 != NULL));
    else if (t_op1 >= LILY_LUA_TINT8) {
        size_t tamano; int signo; bool endianness;
        union lily_simbolo_numero val;
        void* bytes = lua_touserdata(L, -1 - (op2 != NULL));
        lily_lua_int_tipo_valores(t_op1, &tamano, &signo, &endianness);
        lily_nums_decodificar_num(&val, bytes, tamano, signo, endianness);
        *op1 = val.positivo;
    }
    else {
        lua_pushstring(L, "Tipo no compatible para operaciones");
        lua_error(L);
    }

    if (op2 != NULL) {
        // Operador binario
        // Comprobar operando 2
        t_op2 = lily_lua_int_obj(L);
        if (t_op2 == LILY_LUA_TNUMBER)
            *op2 = lua_tointeger(L, -1);
        else if (t_op2 >= LILY_LUA_TINT8) {
            size_t tamano; int signo; bool endianness;
            union lily_simbolo_numero val;
            void* bytes = lua_touserdata(L, -1);
            lily_lua_int_tipo_valores(t_op2, &tamano, &signo, &endianness);
            lily_nums_decodificar_num(&val, bytes, tamano, signo, endianness);
            *op2 = val.positivo;
        }
        else {
            lua_pushstring(L, "Tipo no compatible para operaciones");
            lua_error(L);
        }
        lua_pop(L, 2);

        // Si ambos operandos son userdata, deben ser del mismo tipo
        if (exacto == 3 && t_op1 == t_op2)
            return t_op1;
        else if (exacto == 2)
            // En caso contrario, se devuelve el tipo del objeto userdata
            return t_op2;
        else
            return t_op1;
    }
    lua_pop(L, 1);
    // Operador unario
    if (exacto) return t_op1;
    return LILY_LUA_TINDETERMINADO;
}

int lily_lua_int_mt_add(lua_State* L) {
    uint64_t op1, op2, res;
    uint8_t bytes[8];
    int tipo = lily_lua_int_comp_operandos(L, &op1, &op2);
    if (tipo == LILY_LUA_TINDETERMINADO) {
        lua_pushstring(L, "Los operandos son de tipos diferentes");
        lua_error(L);
    }
    res = op1 + op2;
    lua_pushinteger(L, res);
    enum lily_estado estado = lily_lua_int_comp_argumentos(L, bytes, tipo);
    if (estado == COD_OK) lily_lua_int_crear(L, bytes, tipo);
    else {
        lua_pushstring(L, "El resultado de la operación excede los límites del tipo");
        lua_error(L);
    }
    return 1;
}

int lily_lua_int_mt_sub(lua_State* L) {
    uint64_t op1, op2, res;
    uint8_t bytes[8];
    int tipo = lily_lua_int_comp_operandos(L, &op1, &op2);
    if (tipo == LILY_LUA_TINDETERMINADO) {
        lua_pushstring(L, "Los operandos son de tipos diferentes");
        lua_error(L);
    }
    res = op1 - op2;
    lua_pushinteger(L, res);
    enum lily_estado estado = lily_lua_int_comp_argumentos(L, bytes, tipo);
    if (estado == COD_OK) lily_lua_int_crear(L, bytes, tipo);
    else {
        lua_pushstring(L, "El resultado de la operación excede los límites del tipo");
        lua_error(L);
    }
    return 1;
}

int lily_lua_int_mt_mul(lua_State* L) {
    uint64_t op1, op2, res;
    uint8_t bytes[8];
    int tipo = lily_lua_int_comp_operandos(L, &op1, &op2);
    if (tipo == LILY_LUA_TINDETERMINADO) {
        lua_pushstring(L, "Los operandos son de tipos diferentes");
        lua_error(L);
    }
    res = op1 * op2;
    lua_pushinteger(L, res);
    enum lily_estado estado = lily_lua_int_comp_argumentos(L, bytes, tipo);
    if (estado == COD_OK) lily_lua_int_crear(L, bytes, tipo);
    else {
        lua_pushstring(L, "El resultado de la operación excede los límites del tipo");
        lua_error(L);
    }
    return 1;
}

int lily_lua_int_mt_mod(lua_State* L) {
    uint64_t op1, op2, res;
    uint8_t bytes[8];
    int tipo = lily_lua_int_comp_operandos(L, &op1, &op2);
    if (tipo == LILY_LUA_TINDETERMINADO) {
        lua_pushstring(L, "Los operandos son de tipos diferentes");
        lua_error(L);
    }
    res = op1 % op2;
    lua_pushinteger(L, res);
    enum lily_estado estado = lily_lua_int_comp_argumentos(L, bytes, tipo);
    if (estado == COD_OK) lily_lua_int_crear(L, bytes, tipo);
    else {
        lua_pushstring(L, "El resultado de la operación excede los límites del tipo");
        lua_error(L);
    }
    return 1;
}

int lily_lua_int_mt_pow(lua_State* L) {
    uint64_t op1, op2, res;
    uint8_t bytes[8];
    int tipo = lily_lua_int_comp_operandos(L, &op1, &op2);
    if (tipo == LILY_LUA_TINDETERMINADO) {
        lua_pushstring(L, "Los operandos son de tipos diferentes");
        lua_error(L);
    }
    res = op1;
    for (size_t i = 0; i < op2 - 1; i++) res += op1;
    lua_pushinteger(L, res);
    enum lily_estado estado = lily_lua_int_comp_argumentos(L, bytes, tipo);
    if (estado == COD_OK) lily_lua_int_crear(L, bytes, tipo);
    else {
        lua_pushstring(L, "El resultado de la operación excede los límites del tipo");
        lua_error(L);
    }
    return 1;
}

int lily_lua_int_mt_unm(lua_State* L) {
    uint64_t op1, res;
    uint8_t bytes[8];
    int tipo = lily_lua_int_comp_operandos(L, &op1, NULL);
    if (tipo == LILY_LUA_TINDETERMINADO) {
        lua_pushstring(L, "Los operandos son de tipos diferentes");
        lua_error(L);
    }
    res = -op1;
    lua_pushinteger(L, res);
    enum lily_estado estado = lily_lua_int_comp_argumentos(L, bytes, tipo);
    if (estado == COD_OK) lily_lua_int_crear(L, bytes, tipo);
    else {
        lua_pushstring(L, "El resultado de la operación excede los límites del tipo");
        lua_error(L);
    }
    return 1;
}

int lily_lua_int_mt_idiv(lua_State* L) {
    uint64_t op1, op2, res;
    uint8_t bytes[8];
    int tipo = lily_lua_int_comp_operandos(L, &op1, &op2);
    if (tipo == LILY_LUA_TINDETERMINADO) {
        lua_pushstring(L, "Los operandos son de tipos diferentes");
        lua_error(L);
    }
    res = (uint64_t) (op1 / op2);
    lua_pushinteger(L, res);
    enum lily_estado estado = lily_lua_int_comp_argumentos(L, bytes, tipo);
    if (estado == COD_OK) lily_lua_int_crear(L, bytes, tipo);
    else {
        lua_pushstring(L, "El resultado de la operación excede los límites del tipo");
        lua_error(L);
    }
    return 1;
}

int lily_lua_int_mt_band(lua_State* L) {
    uint64_t op1, op2, res;
    uint8_t bytes[8];
    int tipo = lily_lua_int_comp_operandos(L, &op1, &op2);
    if (tipo == LILY_LUA_TINDETERMINADO) {
        lua_pushstring(L, "Los operandos son de tipos diferentes");
        lua_error(L);
    }
    res = op1 & op2;
    lua_pushinteger(L, res);
    enum lily_estado estado = lily_lua_int_comp_argumentos(L, bytes, tipo);
    if (estado == COD_OK) lily_lua_int_crear(L, bytes, tipo);
    else {
        lua_pushstring(L, "El resultado de la operación excede los límites del tipo");
        lua_error(L);
    }
    return 1;
}

int lily_lua_int_mt_bor(lua_State* L) {
    uint64_t op1, op2, res;
    uint8_t bytes[8];
    int tipo = lily_lua_int_comp_operandos(L, &op1, &op2);
    if (tipo == LILY_LUA_TINDETERMINADO) {
        lua_pushstring(L, "Los operandos son de tipos diferentes");
        lua_error(L);
    }
    res = op1 | op2;
    lua_pushinteger(L, res);
    enum lily_estado estado = lily_lua_int_comp_argumentos(L, bytes, tipo);
    if (estado == COD_OK) lily_lua_int_crear(L, bytes, tipo);
    else {
        lua_pushstring(L, "El resultado de la operación excede los límites del tipo");
        lua_error(L);
    }
    return 1;
}

int lily_lua_int_mt_bxor(lua_State* L) {
    uint64_t op1, op2, res;
    uint8_t bytes[8];
    int tipo = lily_lua_int_comp_operandos(L, &op1, &op2);
    if (tipo == LILY_LUA_TINDETERMINADO) {
        lua_pushstring(L, "Los operandos son de tipos diferentes");
        lua_error(L);
    }
    res = op1 ^ op2;
    lua_pushinteger(L, res);
    enum lily_estado estado = lily_lua_int_comp_argumentos(L, bytes, tipo);
    if (estado == COD_OK) lily_lua_int_crear(L, bytes, tipo);
    else {
        lua_pushstring(L, "El resultado de la operación excede los límites del tipo");
        lua_error(L);
    }
    return 1;
}

int lily_lua_int_mt_bnot(lua_State* L) {
    uint64_t op1, res;
    uint8_t bytes[8];
    int tipo = lily_lua_int_comp_operandos(L, &op1, NULL);
    if (tipo == LILY_LUA_TINDETERMINADO) {
        lua_pushstring(L, "Los operandos son de tipos diferentes");
        lua_error(L);
    }
    res = ~op1;
    lua_pushinteger(L, res);
    enum lily_estado estado = lily_lua_int_comp_argumentos(L, bytes, tipo);
    if (estado == COD_OK) lily_lua_int_crear(L, bytes, tipo);
    else {
        lua_pushstring(L, "El resultado de la operación excede los límites del tipo");
        lua_error(L);
    }
    return 1;
}

int lily_lua_int_mt_shl(lua_State* L) {
    uint64_t op1, op2, res;
    uint8_t bytes[8];
    int tipo = lily_lua_int_comp_operandos(L, &op1, &op2);
    if (tipo == LILY_LUA_TINDETERMINADO) {
        lua_pushstring(L, "Los operandos son de tipos diferentes");
        lua_error(L);
    }
    res = op1 << op2;
    lua_pushinteger(L, res);
    enum lily_estado estado = lily_lua_int_comp_argumentos(L, bytes, tipo);
    if (estado == COD_OK) lily_lua_int_crear(L, bytes, tipo);
    else {
        lua_pushstring(L, "El resultado de la operación excede los límites del tipo");
        lua_error(L);
    }
    return 1;
}

int lily_lua_int_mt_shr(lua_State* L) {
    uint64_t op1, op2, res;
    uint8_t bytes[8];
    int tipo = lily_lua_int_comp_operandos(L, &op1, &op2);
    if (tipo == LILY_LUA_TINDETERMINADO) {
        lua_pushstring(L, "Los operandos son de tipos diferentes");
        lua_error(L);
    }
    res = op1 >> op2;
    lua_pushinteger(L, res);
    enum lily_estado estado = lily_lua_int_comp_argumentos(L, bytes, tipo);
    if (estado == COD_OK) lily_lua_int_crear(L, bytes, tipo);
    else {
        lua_pushstring(L, "El resultado de la operación excede los límites del tipo");
        lua_error(L);
    }
    return 1;
}

int lily_lua_int_mt_len(lua_State* L) {
    uint64_t op1;
    int tipo = lily_lua_int_comp_operandos(L, &op1, NULL);
    if (tipo == LILY_LUA_TINDETERMINADO) {
        lua_pushstring(L, "Los operandos son de tipos diferentes");
        lua_error(L);
    }
    lua_getiuservalue(L, -1, 1);
    return 1;
}

int lily_lua_int_mt_eq(lua_State* L) {
    uint64_t op1, op2;
    uint8_t bytes[8];
    int tipo = lily_lua_int_comp_operandos(L, &op1, &op2);
    if (tipo == LILY_LUA_TINDETERMINADO) {
        lua_pushstring(L, "Los operandos son de tipos diferentes");
        lua_error(L);
    }
    lua_pushboolean(L, op1 == op2);
    enum lily_estado estado = lily_lua_int_comp_argumentos(L, bytes, tipo);
    if (estado == COD_OK) lily_lua_int_crear(L, bytes, tipo);
    else {
        lua_pushstring(L, "El resultado de la operación excede los límites del tipo");
        lua_error(L);
    }
    return 1;
}

int lily_lua_int_mt_lt(lua_State* L) {
    uint64_t op1, op2;
    uint8_t bytes[8];
    int tipo = lily_lua_int_comp_operandos(L, &op1, &op2);
    if (tipo == LILY_LUA_TINDETERMINADO) {
        lua_pushstring(L, "Los operandos son de tipos diferentes");
        lua_error(L);
    }
    lua_pushboolean(L, op1 < op2);
    enum lily_estado estado = lily_lua_int_comp_argumentos(L, bytes, tipo);
    if (estado == COD_OK) lily_lua_int_crear(L, bytes, tipo);
    else {
        lua_pushstring(L, "El resultado de la operación excede los límites del tipo");
        lua_error(L);
    }
    return 1;
}

int lily_lua_int_mt_le(lua_State* L) {
    uint64_t op1, op2;
    uint8_t bytes[8];
    int tipo = lily_lua_int_comp_operandos(L, &op1, &op2);
    if (tipo == LILY_LUA_TINDETERMINADO) {
        lua_pushstring(L, "Los operandos son de tipos diferentes");
        lua_error(L);
    }
    lua_pushboolean(L, op1 <= op2);
    enum lily_estado estado = lily_lua_int_comp_argumentos(L, bytes, tipo);
    if (estado == COD_OK) lily_lua_int_crear(L, bytes, tipo);
    else {
        lua_pushstring(L, "El resultado de la operación excede los límites del tipo");
        lua_error(L);
    }
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
    lua_getiuservalue(L, -1, 1);
    uint64_t tamano = lua_tointeger(L, -1);
    lua_getiuservalue(L, -1, 3);
    bool endianness = lua_tointeger(L, -1);
    lua_pop(L, 3);
    void *obj = lua_touserdata(L, -1);
    char cad[19] = {0};
    strcpy(cad, "0x");
    for (size_t i = 0, j = 2; i < tamano; i++) {
      size_t real_i = endianness ? i : (tamano - 1 - i);
      // 3, 2, 1, 0
      uint8_t byte = ((uint8_t*)obj)[real_i];
      // Primera letra
      if ((byte & 0x0F) < 0xA ) cad[j] = 0x30 + (byte & 0x0F);
      else cad[j] = 0x57 + (byte & 0x0F);
      j++;
      // Segunda letra
      if ((byte & 0xF0) < 0xA) cad[j] = 0x30 + (byte & 0xF0);
      else cad[j] = 0x57 + (byte & 0xF0);
      j++;
    }
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
    lua_createtable(L, 0, 22);
    lua_registertable(L, "__add", lily_lua_int_mt_add);
    lua_registertable(L, "__sub", lily_lua_int_mt_sub);
    lua_registertable(L, "__mul", lily_lua_int_mt_mul);
    lua_registertable(L, "__mod", lily_lua_int_mt_mod);
    lua_registertable(L, "__pow", lily_lua_int_mt_pow);
    lua_registertable(L, "__unm", lily_lua_int_mt_unm);
    lua_registertable(L, "__idiv", lily_lua_int_mt_idiv);
    lua_registertable(L, "__band", lily_lua_int_mt_band);
    lua_registertable(L, "__bor", lily_lua_int_mt_bor);
    lua_registertable(L, "__bxor", lily_lua_int_mt_bxor);
    lua_registertable(L, "__bnot", lily_lua_int_mt_bnot);
    lua_registertable(L, "__shl", lily_lua_int_mt_shl);
    lua_registertable(L, "__shr", lily_lua_int_mt_shr);
    lua_registertable(L, "__len", lily_lua_int_mt_len);
    lua_registertable(L, "__eq", lily_lua_int_mt_eq);
    lua_registertable(L, "__lt", lily_lua_int_mt_lt);
    lua_registertable(L, "__le", lily_lua_int_mt_le);
    lua_registertable(L, "__index", lily_lua_int_mt_index);
    lua_registertable(L, "__tostring", lily_lua_int_mt_tostring);
    lua_setglobal(L, "__imt");
    //lua_setglobal(L, "L");
}
