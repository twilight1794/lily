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

#include "lua_ejecucion.h"

int lily_lua_ejecucion_leer_memoria(struct lily_lua_ejecucion_maquina* maquina, uint64_t direccion, uint8_t* valor, bool anunciar) {
    if (direccion >= maquina->tamano_memoria)
        return -1;
    *valor = maquina->memoria[direccion];
    // Anunciar
    if (anunciar) {
        struct lily_lily_mensaje_tmemoria mensaje = {
            .direccion = direccion,
            .tamano = 1,
            .valor = *valor
        };
        lua_getglobal(maquina->L, "_lily_enviar_mensaje");
        f_mensajes_ptr enviar_mensaje = (f_mensajes_ptr) lua_tointeger(maquina->L, -1);
        lua_pop(maquina->L, 1);
        enviar_mensaje(LILY_MENSAJE_TMEMORIA, LILY_MENSAJE_TMEMORIA_LECTURA, "lua_ejecucion_leer_memoria", &mensaje);
    }
    return 0;
}

int lily_lua_ejecucion_escribir_memoria(struct lily_lua_ejecucion_maquina* maquina, uint64_t direccion, uint8_t* valor, uint8_t* valor_anterior, bool anunciar) {
    if (direccion >= maquina->tamano_memoria)
        return -1;
    if (valor_anterior != NULL)
        *valor_anterior = maquina->memoria[direccion];
    maquina->memoria[direccion] = *valor;
    // Anunciar
    if (anunciar) {
        struct lily_lily_mensaje_tmemoria mensaje = {
            .direccion = direccion,
            .tamano = 1,
            .valor = *valor
        };
        lua_getglobal(maquina->L, "_lily_enviar_mensaje");
        f_mensajes_ptr enviar_mensaje = (f_mensajes_ptr) lua_tointeger(maquina->L, -1);
        lua_pop(maquina->L, 1);
        enviar_mensaje(LILY_MENSAJE_TMEMORIA, LILY_MENSAJE_TMEMORIA_ESCRITURA, "lua_ejecucion_escribir_memoria", &mensaje);
    }
    return 0;
}

int lily_lua_ejecucion_leer_registro(struct lily_lua_ejecucion_maquina* maquina, const char* registro, uint8_t* valor, bool anunciar) {
    lua_getglobal(maquina->L, "_lily_esquema");
    // Obtener ubicación de registro
    lua_pushliteral(maquina->L, "registros");
    lua_gettable(maquina->L, -2);
    lua_pushstring(maquina->L, registro);
    if (lua_gettable(maquina->L, -2) == LUA_TNIL) {
        lua_pop(maquina->L, 2);
        return -1;
    }
    lua_pushliteral(maquina->L, "desplazamiento");
    lua_gettable(maquina->L, -2);
    uint64_t ubicacion = lua_tointeger(maquina->L, -1);
    lua_pop(maquina->L, 1);
    lua_pushliteral(maquina->L, "tamano");
    lua_gettable(maquina->L, -2);
    uint64_t tamano = lua_tointeger(maquina->L, -1);
    lua_pop(maquina->L, 1);
    // Obtener dato
    *((uint64_t*) valor) = 0;
    lily_bitarray_obtener(ubicacion, tamano, maquina->registros, valor);
    lua_pop(maquina->L, 3);
    // Anunciar
    if (anunciar) {
        struct lily_lily_mensaje_tregistro mensaje = {
            .valor = *valor,
            .desplazamiento = ubicacion,
            .registro = registro,
            .tamano = tamano
        };
        lua_getglobal(maquina->L, "_lily_enviar_mensaje");
        f_mensajes_ptr enviar_mensaje = (f_mensajes_ptr) lua_tointeger(maquina->L, -1);
        lua_pop(maquina->L, 1);
        enviar_mensaje(LILY_MENSAJE_TREGISTRO, LILY_MENSAJE_TREGISTRO_LECTURA, "lua_ejecucion_leer_registro", &mensaje);
    }
    return 0;
}

int lily_lua_ejecucion_escribir_registro(struct lily_lua_ejecucion_maquina* maquina, const char* registro, const uint8_t* valor, uint8_t* valor_anterior, bool anunciar) {
    lua_getglobal(maquina->L, "_lily_esquema");
    // Obtener ubicación de registro
    lua_pushliteral(maquina->L, "registros");
    lua_gettable(maquina->L, -2);
    lua_pushstring(maquina->L, registro);
    if (lua_gettable(maquina->L, -2) == LUA_TNIL) {
        lua_pop(maquina->L, 2);
        return -1;
    }
    lua_pushliteral(maquina->L, "desplazamiento");
    lua_gettable(maquina->L, -2);
    uint64_t ubicacion = lua_tointeger(maquina->L, -1);
    lua_pop(maquina->L, 1);
    lua_pushliteral(maquina->L, "tamano");
    lua_gettable(maquina->L, -2);
    uint64_t tamano = lua_tointeger(maquina->L, -1);
    lua_pop(maquina->L, 1);
    // Obtener dato
    if (valor_anterior != NULL) {
        *((uint64_t*) valor_anterior) = 0;
        lily_bitarray_obtener(ubicacion, tamano, maquina->registros, valor_anterior);
    }
    // Escribir dato
    lily_bitarray_guardar(ubicacion, tamano, maquina->registros, valor);
    lua_pop(maquina->L, 3);
    // Anunciar
    if (anunciar) {
        struct lily_lily_mensaje_tregistro mensaje = {
            .valor = *valor,
            .desplazamiento = ubicacion,
            .registro = registro,
            .tamano = tamano
        };
        lua_getglobal(maquina->L, "_lily_enviar_mensaje");
        f_mensajes_ptr enviar_mensaje = (f_mensajes_ptr) lua_tointeger(maquina->L, -1);
        lua_pop(maquina->L, 1);
        enviar_mensaje(LILY_MENSAJE_TREGISTRO, LILY_MENSAJE_TREGISTRO_ESCRITURA, "lua_ejecucion_escribir_registro", &mensaje);
    }
    return 0;
}

struct lily_lua_ejecucion_maquina* lily_lua_ejecucion_ini(lua_State* L, struct lily_lua_ejecucion_ctx* ctx) {
    struct lily_lua_ejecucion_maquina* obj = (struct lily_lua_ejecucion_maquina*) malloc(sizeof(struct lily_lua_ejecucion_maquina));
    if (obj == NULL) {
      ctx->estado = COD_MALLOC_FALLO;
      return NULL;
    }
    obj->L = L;
    // Obtener espacio para memoria
    lua_pushliteral(L, "max_memoria");
    lua_gettable(L, -2);
    // FIX: por ahora, el tamaño de la memoria no es ajustable
    obj->tamano_memoria = (uint64_t) lua_tointeger(L, -1);
    obj->memoria = (uint8_t*) malloc(obj->tamano_memoria);
    if (obj->memoria == NULL) {
        free(obj);
        ctx->estado = COD_MALLOC_FALLO;
        return NULL;
    }
    lua_pop(L, 1);
    // Guardar para Lua la dirección del bloque de memoria
    lua_pushinteger(L, (lua_Integer) obj->memoria);
    lua_setglobal(L, "_lily_memoria_dir");
    // Obtener registro contador de programa
    lua_pushstring(L, "registro_programa");
    lua_gettable(L, -2);
    obj->pc = lua_tostring(L, -1);
    lua_pop(L, 1);
    // Obtener espacio para registros
    obj->tamano_registros = 0;
    lua_pushstring(L, "registros");
    lua_gettable(L, -2);
    lua_pushnil(L);
    while (lua_next(L, -2) != 0) {
        // Obtener tamaño de registro
        lua_pushstring(L, "tamano");
        lua_gettable(L, -2);
        const size_t tamano = lua_tointeger(L, -1);
        lua_pop(L, 1);
        // Obtener desplazamiento de registro
        lua_pushstring(L, "desplazamiento");
        lua_gettable(L, -2);
        const size_t desplazamiento = lua_tointeger(L, -1);
        lua_pop(L, 1);
        // Suma
        const size_t total = desplazamiento + tamano;
        if (total > obj->tamano_registros)
            obj->tamano_registros = total;
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
    obj->registros = (uint8_t*) malloc(ceil(obj->tamano_registros/8));
    if (obj->registros == NULL) {
        free(obj->memoria);
        free(obj);
        ctx->estado = COD_MALLOC_FALLO;
        return NULL;
    }
    lua_pushinteger(L, (lua_Integer) obj->registros);
    lua_setglobal(L, "_lily_registros_dir");
    lua_pushinteger(L, obj->tamano_registros);
    lua_setglobal(L, "_lily_registros_max");
    return obj;
}

void lily_lua_ejecucion_arrancar(struct lily_lua_ejecucion_maquina* maquina, uint8_t* programa, size_t tamano, struct lily_lua_ejecucion_ctx* ctx) {
    // TODO: Por ahora, solo copiamos los datos de un lado a otro desde el inicio
    // Para otros sistemas, vendrá la carga de una BIOS
    for (size_t i = 0; i < tamano; i++) {
        maquina->memoria[i] = programa[i];
        struct lily_lily_mensaje_tmemoria obj = {
            .direccion = (uint64_t) i,
            .valor = programa[i],
            .tamano = 1,
        };
        ctx->fun_mensaje(LILY_MENSAJE_TMEMORIA, LILY_MENSAJE_TMEMORIA_ESCRITURA, "lua_ejecucion_arrancar", &obj);
    }
    // Inicializar PC
    // FIX: temporalmente, iniciamos siempre en 0
    // FIX: inicializar los demás registros
    // TODO: ver si hay una forma eficiente de conservar la abstracción
    const uint64_t valor = 0;
    lily_lua_ejecucion_escribir_registro(maquina, maquina->pc, (const uint8_t*) &valor, NULL, true);
}

void lily_lua_ejecucion_ejecutar(struct lily_lua_ejecucion_maquina* maquina, struct lily_lua_ejecucion_ctx* ctx) {
    // Obtener PC
    union lily_simbolo_numero pc;
    bool ejecutado = false;
    pc.positivo = 0;
    lily_lua_ejecucion_leer_registro(maquina, maquina->pc, pc.bytes, false);
    // Obtener casos de opcodes
    int base_pila = lua_gettop(maquina->L); // Para limpiar después
    lua_pushstring(maquina->L, "opcodes");
    lua_gettable(maquina->L, -2);
    lua_len(maquina->L, -1);
    size_t tam_lista_casos = lua_tointeger(maquina->L, -1);
    lua_pop(maquina->L, 1);
    char* msg_buf = d_printf("Se encontraron %lu casos", tam_lista_casos);
    ctx->fun_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "lua_ejecucion_ejecutar", msg_buf);
    free(msg_buf);
    const size_t ptr_inicio = pc.positivo; // Inicio de la instrucción
    size_t ptr_inicio_relativo = ptr_inicio;
    size_t i = 1;
    while (!ejecutado && i <= tam_lista_casos) {
        // Por cada elemento...
        lua_geti(maquina->L, -1, i++); // <- Caso
        lua_geti(maquina->L, -1, 1); // <- Lista de coincidencias

        lua_len(maquina->L, -1);
        const size_t tam_lista_bytes = lua_tointeger(maquina->L, -1);
        lua_pop(maquina->L, 1);
        size_t tam_cad_params = 0;
        for (size_t j = 1; j <= tam_lista_bytes; j++) {
            const int tipo = lua_geti(maquina->L, -1, j);
            if (tipo == LUA_TNUMBER) tam_cad_params += 6;
            else if (tipo == LUA_TFUNCTION) tam_cad_params += 3;
            else if (tipo == LUA_TBOOLEAN) tam_cad_params += 6;
            lua_pop(maquina->L, 1);
        }
        tam_cad_params -= 1;

        // Generar mensaje de log
        char* msg_cad_params = (char*) malloc(tam_cad_params);
        if (msg_cad_params == NULL) {
            ctx->estado = COD_MALLOC_FALLO;
            return;
        }
        for (size_t j = 1, k = 0; j <= tam_lista_bytes; j++) {
            const int tipo = lua_geti(maquina->L, -1, j);
            if (tipo == LUA_TNUMBER) {
                snprintf(msg_cad_params + k, 5, "0x%02llx", lua_tointeger(maquina->L, -1));
                k += 4;
            }
            else if (tipo == LUA_TFUNCTION) {
                memcpy(msg_cad_params + k, "@", 2);
                k++;
            }
            else if (tipo == LUA_TBOOLEAN) {
                memcpy(msg_cad_params + k, "true", 5);
                k += 4;
            }
            lua_pop(maquina->L, 1);
            if (j < tam_lista_bytes) {
                memcpy(msg_cad_params + k, ", ", 2);
                k += 2;
            }
        }
        msg_buf = d_printf("Comprobando caso (%s)", msg_cad_params);
        ctx->fun_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "lua_ejecucion_ejecutar", msg_buf);
        free(msg_buf);

        // Iterar por cada elemento de coincidencia
        bool coincide = true; // Esto queda así si toda la lista de coincidencias coincide
        for (size_t j = 1; j <= tam_lista_bytes; j++) {
            const uint8_t byte_actual = maquina->memoria[ptr_inicio_relativo + j - 1];
            msg_buf = d_printf("Comparando byte 0x%02lx en dirección 0x%02lx + %lu", byte_actual, ptr_inicio_relativo, j - 1);
            ctx->fun_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "lua_ejecucion_ejecutar", msg_buf);
            free(msg_buf);
            const int tipo = lua_geti(maquina->L, -1, j);
            // Comparar
            if (tipo == LUA_TNUMBER) {
                const uint8_t val_num = (uint8_t) lua_tointeger(maquina->L, -1);
                lua_pop(maquina->L, 1);
                msg_buf = d_printf("Comparando con elemento de coincidencia 0x%02lx", val_num);
                ctx->fun_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "lua_ejecucion_ejecutar", msg_buf);
                free(msg_buf);
                if (val_num != byte_actual) {
                    coincide = false;
                    break;
                }
            }
            else if (tipo == LUA_TFUNCTION) {
                lua_pushinteger(maquina->L, byte_actual);
                lua_pcall(maquina->L, 1, 1, 0);
                const bool val_fun = lua_toboolean(maquina->L, -1);
                lua_pop(maquina->L, 1);
                msg_buf = d_printf("Comparando con elemento de coincidencia (%ld) @", j);
                ctx->fun_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "lua_ejecucion_ejecutar", msg_buf);
                free(msg_buf);
                if (!val_fun) {
                    coincide = false;
                    break;
                }
            }
            else {
                // De todos modos tenemos qué quitar el elemento de coincidencia
                lua_pop(maquina->L, 1);
            }
        }
        lua_pop(maquina->L, 1); // Quitamos la lista de coincidencias

        if (coincide) {
            // Obtener resultado de coincidencia
            msg_buf = d_printf("Seleccionado caso (%s)", msg_cad_params);
            ctx->fun_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "lua_ejecucion_ejecutar", msg_buf);
            free(msg_buf);
            const int tipo = lua_geti(maquina->L, -1, 2);
            if (tipo == LUA_TFUNCTION) {
                // ¡Valor de coincidencia terminal!: ejecutar y limpiar
                // Generar lista de bytes
                const size_t num_bytes = (ptr_inicio_relativo + tam_lista_bytes) - ptr_inicio;
                lua_createtable(maquina->L, num_bytes, 0);
                for (size_t j = 1; j <= num_bytes; j++) {
                    lua_pushinteger(maquina->L, maquina->memoria[ptr_inicio + j - 1]);
                    lua_seti(maquina->L, -2, j);
                }
                // Actualizar PC
                pc.positivo += num_bytes;
                lily_lua_ejecucion_escribir_registro(maquina, maquina->pc, pc.bytes, NULL, true);
                // Y ejecutar operación
                ejecutado = true;
                if (lua_pcall(maquina->L, 1, 0, 0)) {
                    // FIX: procesar error bien
                    ctx->estado = COD_LUA_EXC;
                    ctx->fun_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_FATAL, "lua_ejecucion_ejecutar", lua_tostring(maquina->L, -1));
                }
            }
            else if (tipo == LUA_TTABLE) {
                // Lista de casos: la cargamos y continuamos con ella
                ptr_inicio_relativo += tam_lista_bytes;
                lua_len(maquina->L, -1);
                tam_lista_casos = lua_tointeger(maquina->L, -1);
                lua_pop(maquina->L, 1);
                i = 1;
                msg_buf = d_printf("Lista de casos anidada: Se encontraron %lu casos", tam_lista_casos);
                ctx->fun_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "lua_ejecucion_ejecutar", msg_buf);
            }
        }
        else {
            // La lista de coincidencias no coincidió, pasar a otro caso si lo hay
            lua_pop(maquina->L, 1);
        }
        free(msg_cad_params);
    }
    lua_settop(maquina->L, base_pila);

    if (!ejecutado) {
        ctx->estado = COD_LUA_EJECUCION_INSTRUCCION_ILEGAL;
        // FIX: lista de bytes
    }
}
