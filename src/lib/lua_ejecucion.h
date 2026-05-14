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

/**
 * Inicializa una máquina virtual Lily
 * @param L Sesión de Lua
 * @param enviar_mensaje Función para enviar un mensaje al host
 * @param [out] estado Estado de la ejecución
 * @param [out] ctx Contexto de la ejecución
 */
struct lily_lua_ejecucion_maquina* lily_lua_ejecucion_ini(lua_State* L, f_mensajes_ptr enviar_mensaje, enum lily_estado* estado, void** ctx);

/**
 * @brief Cargador de arranque primigenio
 * Carga una secuencia de bytes a memoria
 * @param maquina Máquina virtual sobre la que se trabajará
 * @param programa Puntero a la secuencia de bytes a cargar
 * @param tamano Tamaño de la secuencia de bytes a cargar
 */
void lily_lua_ejecucion_arrancar(struct lily_lua_ejecucion_maquina* maquina, uint8_t* programa, size_t tamano);

/**
 * Pues eso, ejecutar una máquina virtual
 * @param maquina Máquina virtual sobre la que se trabajará
 * @param enviar_mensaje Función para enviar un mensaje al host
 * @param [out] estado Estado de la ejecución
 * @param [out] ctx Contexto de la ejecución
 */
void lily_lua_ejecucion_ejecutar(struct lily_lua_ejecucion_maquina* maquina, f_mensajes_ptr enviar_mensaje, enum lily_estado* estado, void** ctx);

#endif
