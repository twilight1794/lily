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

/*
 * @file lua_ejecucion.h
 * Funciones para ejecutar código máquina según un esquema de descripción de microprocesador
 */

#ifndef LILY_L_LUA_EJECUCION
#define LILY_L_LUA_EJECUCION

#include <math.h>
#include <stdint.h>

#include "../common/bitarray.h"
#include "../common/defs.h"
#include "../common/estado.h"
#include "../common/lde.h"
#include "../common/log.h"
#include "mensajes.h"

#include <lua.h>

struct lily_lua_ejecucion_ctx;

/**
 * @brief Máquina virtual
 * Soporte físico para ejecutar un programa: memoria y registros
 */
struct lily_lua_ejecucion_maquina {
    uint64_t tamano_registros; /**< Tamaño del bloque de memoria física para almacenar los registros de la máquina */
    uint64_t tamano_memoria; /**< Tamaño del bloque de memoria física para almacenar la memoria de la máquina */
    uint64_t tamano_pc; /**< Tamaño del registro contador de programa en bits */
    uint64_t posicion_pc; /**< Posición del registro contador de programa en el bloque, en bits */
    uint8_t* registros; /**< Bloque de memoria física para almacenar los registros de la máquina */
    uint8_t* memoria; /**< Bloque de memoria física para almacenar la memoria de la máquina */
    lua_State* L; /**< Sesión de Lua asociada */
};

int lily_lua_ejecucion_leer_memoria(struct lily_lua_ejecucion_maquina* maquina, uint64_t direccion, uint8_t* valor);
int lily_lua_ejecucion_escribir_memoria(struct lily_lua_ejecucion_maquina* maquina, uint64_t direccion, uint8_t* valor, uint8_t* valor_anterior);
int lily_lua_ejecucion_leer_registro(struct lily_lua_ejecucion_maquina* maquina, const char* registro, uint8_t* valor);
int lily_lua_ejecucion_escribir_registro(struct lily_lua_ejecucion_maquina* maquina, const char* registro, const uint8_t* valor, uint8_t* valor_anterior);

/**
 * Inicializa una máquina virtual Lily
 * @param L Sesión de Lua
 * @param [out] ctx Contexto de la ejecución
 */
struct lily_lua_ejecucion_maquina* lily_lua_ejecucion_ini(lua_State* L, struct lily_lua_ejecucion_ctx* ctx);

/**
 * @brief Cargador de arranque primigenio
 * Carga una secuencia de bytes a memoria
 * @param maquina Máquina virtual sobre la que se trabajará
 * @param programa Puntero a la secuencia de bytes a cargar
 * @param tamano Tamaño de la secuencia de bytes a cargar
 * @param ctx Contexto de la ejecución
 */
void lily_lua_ejecucion_arrancar(struct lily_lua_ejecucion_maquina* maquina, uint8_t* programa, size_t tamano, struct lily_lua_ejecucion_ctx* ctx);

typedef void (*f_ejecutora_ptr)(struct lily_lua_ejecucion_maquina* maquina, struct lily_lua_ejecucion_ctx* ctx);

/**
 * Ejecuta instrucciones en la máquina virtual
 * @param maquina Máquina virtual sobre la que se trabajará
 * @param [out] ctx Contexto de la ejecución
 */
void lily_lua_ejecucion_ejecutar(struct lily_lua_ejecucion_maquina* maquina, struct lily_lua_ejecucion_ctx* ctx);

struct lily_lua_ejecucion_ctx {
    struct lily_lily_archivo* (*fun_abrir_archivo)(const char*, int, int*);
    int (*fun_cerrar_archivo)(struct lily_lily_archivo*);
    f_mensajes_ptr fun_mensaje;
    const char* lua_msg;
    enum lily_estado estado;
    enum lily_estado estado_ejecucion;
};

#endif
