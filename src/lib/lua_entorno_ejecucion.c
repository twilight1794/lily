#include "lua_entorno_ejecucion.h"

void lily_lua_entorno_ejecucion_validar_params(lua_State *L, bool escritura) {
    if (escritura && lua_gettop(L) != 2) {
        lua_pushstring(L, "Se necesitan dos parámetros");
        lua_error(L);
    }
    else if (!escritura && lua_gettop(L) != 1) {
        lua_pushstring(L, "Se necesita un parámetro");
        lua_error(L);
    }
    lua_getglobal(L, "_lily_esquema");

    // Validar que en escritura, el tipo sea numérico
    if (escritura) {
        const int tipo = lua_type(L, 2);
        if (tipo != LUA_TNUMBER) {
            lua_pushliteral(L, "El segundo parámetro no es un valor numérico");
            lua_error(L);
        }
    }
}

void lily_lua_entorno_ejecucion_validar_memoria(lua_State* L, bool escritura) {
    // Validar que parámetro 1 sea una dirección de memoria disponible
    if (!lua_isinteger(L, 1)) {
        lua_pushliteral(L, "El primer parámetro no es una dirección válida de memoria");
        lua_error(L);
    }
    lua_pushliteral(L, "max_memoria"); // FIX: por ahora, esto es el tamaño de la memoria
    lua_gettable(L, -2);
    const size_t max_memoria = lua_tointeger(L, -1);
    lua_pop(L, 1);
    const size_t direccion = lua_tointeger(L, 1);
    if (direccion >= max_memoria) {
        lua_pushliteral(L, "La dirección de memoria es mayor que el tamaño disponible");
        lua_error(L);
    }
}

void lily_lua_entorno_ejecucion_validar_registro(lua_State *L, bool escritura) {
    // Validar que parámetro 1 sea un registro existente
    lua_pushliteral(L, "registros");
    lua_gettable(L, -2);
    lua_pushvalue(L, 1); // Copiar parámetro clave
    lua_gettable(L, -2);
    if (lua_isnoneornil(L, -1)) {
        lua_pushliteral(L, "El primer parámetro no es un registro válido");
        lua_error(L);
    }
    lua_pushliteral(L, "tamano");
    lua_gettable(L, -2);
    const size_t tamano = lua_tointeger(L, -1);
    lua_pop(L, 1);

    // Si es escritura, validar que el valor quepa en el registro
    // FIX: por ahora, solo enteros
    if (escritura) {
        if (ceil(log2(lua_tointeger(L, 1))) > tamano) {
            lua_pushliteral(L, "El tamaño del valor a guardar es más grande que el registro");
            lua_error(L);
        }
    }
}

int lily_lua_entorno_ejecucion_leer_memoria(lua_State* L) {
    lily_lua_entorno_ejecucion_validar_params(L, false);
    lily_lua_entorno_ejecucion_validar_memoria(L, false);
    // Obtener datos
    lua_getglobal(L, "_lily_memoria_dir");
    uint8_t* memoria = (uint8_t*) lua_tointeger(L, -1);
    lua_pop(L, 2); // -global, esquema
    // Lectura
    const size_t direccion = lua_tointeger(L, 1);
    lua_Integer valor = memoria[direccion];
    lua_pushinteger(L, valor);
    // Mensaje
    struct lily_lily_mensaje_tmemoria mensaje = {
        .direccion = direccion,
        .tamano = 1,
        .valor = valor
    };
    lua_getglobal(L, "_lily_enviar_mensaje");
    f_mensajes_ptr enviar_mensaje = (f_mensajes_ptr) lua_tointeger(L, -1);
    lua_pop(L, 1);
    enviar_mensaje(LILY_MENSAJE_TMEMORIA, LILY_MENSAJE_TMEMORIA_LECTURA, "lua_entorno_ejecucion_leer_memoria", &mensaje);
    return 1;
}

int lily_lua_entorno_ejecucion_escribir_memoria(lua_State* L) {
    lily_lua_entorno_ejecucion_validar_params(L, true);
    lily_lua_entorno_ejecucion_validar_memoria(L, true);
    // Obtener datos
    lua_getglobal(L, "_lily_memoria_dir");
    uint8_t* memoria = (uint8_t*) lua_tointeger(L, -1);
    lua_pop(L, 2); // -global, esquema
    // Escritura
    const size_t direccion = lua_tointeger(L, 1);
    lua_Integer valor = lua_tointeger(L, 2);
    memoria[direccion] = *((uint8_t*) &valor);
    // Mensaje
    struct lily_lily_mensaje_tmemoria mensaje = {
        .direccion = direccion,
        .tamano = 1,
        .valor = valor
    };
    lua_getglobal(L, "_lily_enviar_mensaje");
    f_mensajes_ptr enviar_mensaje = (f_mensajes_ptr) lua_tointeger(L, -1);
    lua_pop(L, 1);
    enviar_mensaje(LILY_MENSAJE_TMEMORIA, LILY_MENSAJE_TMEMORIA_ESCRITURA, "lua_entorno_ejecucion_escribir_memoria", &mensaje);
    return 0;
}

int lily_lua_entorno_ejecucion_leer_registro(lua_State* L) {
    lily_lua_entorno_ejecucion_validar_params(L, false);
    lily_lua_entorno_ejecucion_validar_registro(L, false);
    // Obtener datos
    lua_pushliteral(L, "tamano");
    lua_gettable(L, -2);
    const size_t tamano = lua_tointeger(L, -1);
    lua_pop(L, 1);
    lua_pushliteral(L, "desplazamiento");
    lua_gettable(L, -2);
    const size_t posicion = lua_tointeger(L, -1);
    lua_pop(L, 1);
    lua_getglobal(L, "_lily_registros_dir");
    uint8_t* registros = (uint8_t*) lua_tointeger(L, -1);
    lua_pop(L, 4); // -global, reg, registros, esquema
    // Lectura
    lua_Integer valor = 0;
    lily_bitarray_obtener(posicion, tamano, registros, (uint8_t*) &valor);
    lua_pushinteger(L, valor);
    // Mensaje
    struct lily_lily_mensaje_tregistro mensaje = {
        .desplazamiento = posicion,
        .registro = lua_tostring(L, 1),
        .tamano = tamano,
        .valor = valor
    };
    lua_getglobal(L, "_lily_enviar_mensaje");
    f_mensajes_ptr enviar_mensaje = (f_mensajes_ptr) lua_tointeger(L, -1);
    lua_pop(L, 1);
    enviar_mensaje(LILY_MENSAJE_TREGISTRO, LILY_MENSAJE_TREGISTRO_LECTURA, "lua_entorno_ejecucion_leer_registro", &mensaje);
    return 1;
}

int lily_lua_entorno_ejecucion_escribir_registro(lua_State* L) {
    lily_lua_entorno_ejecucion_validar_params(L, true);
    lily_lua_entorno_ejecucion_validar_registro(L, true);
    // Obtener datos
    lua_pushliteral(L, "tamano");
    lua_gettable(L, -2);
    const size_t tamano = lua_tointeger(L, -1);
    lua_pop(L, 1);
    lua_pushliteral(L, "desplazamiento");
    lua_gettable(L, -2);
    const size_t posicion = lua_tointeger(L, -1);
    lua_pop(L, 1);
    lua_getglobal(L, "_lily_registros_dir");
    uint8_t* registros = (uint8_t*) lua_tointeger(L, -1);
    lua_pop(L, 4); // -global, reg, registros, esquema
    // Escritura
    lua_Integer valor = lua_tointeger(L, 2);
    lily_bitarray_guardar(posicion, tamano, registros, (uint8_t*) &valor);
    // Mensaje
    struct lily_lily_mensaje_tregistro mensaje = {
        .desplazamiento = posicion,
        .registro = lua_tostring(L, 1),
        .tamano = tamano,
        .valor = valor
    };
    lua_getglobal(L, "_lily_enviar_mensaje");
    f_mensajes_ptr enviar_mensaje = (f_mensajes_ptr) lua_tointeger(L, -1);
    lua_pop(L, 1);
    enviar_mensaje(LILY_MENSAJE_TREGISTRO, LILY_MENSAJE_TREGISTRO_ESCRITURA, "lua_entorno_ejecucion_escribir_registro", &mensaje);
    return 0;
}

void lily_lua_entorno_ejecucion_preparar(lua_State* L, enum lily_estado *estado) {
    lua_createtable(L, 0, 4);
    lua_registertable(L, "leer_memoria", lily_lua_entorno_ejecucion_leer_memoria);
    lua_registertable(L, "escribir_memoria", lily_lua_entorno_ejecucion_escribir_memoria);
    lua_registertable(L, "leer_registro", lily_lua_entorno_ejecucion_leer_registro);
    lua_registertable(L, "escribir_registro", lily_lua_entorno_ejecucion_escribir_registro);
    lua_setglobal(L, "L");
}
