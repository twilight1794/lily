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

/**
 * @file lily.h
 * Funciones de entrada para los procesos principales
 */

#ifndef LILY_L_LILY
#define LILY_L_LILY

#include "a_lexico.h"
#include "a_semantico.h"
#include "a_sintactico.h"
#include "lua_entorno.h"
#include "lua_int.h"
#include "lua_entorno_ejecucion.h"
#include "lua_ejecucion.h"
#include "mensajes.h"
#include "../common/estado.h"

enum lily_lily_archivo_tipo {
    LILY_ARCHIVO_TFUENTE_ASM,
    LILY_ARCHIVO_TPROCESADOR,
};

/**
 * Contenedor para almacenar datos de un archivo del sistema
 */
struct lily_lily_archivo {
  void *obj;                        /**< Objeto subyacente del sistema */
  char *archivo;                    /**< Puntero al contenido */
  enum lily_lily_archivo_tipo tipo; /**< Tipo de archivo */
};

/**
 * Función de entrada para ensamblar
 * @param datos_entrada Código fuente en ensamblador
 * @param arquitectura Arquitectura forzada
 * @param opciones Listado de opciones definidas por el usuario
 * @param fun_abrir_archivo Función para solicitar un archivo al sistema cliente
 * @param fun_cerrar_archivo Función para cerrar un archivo en el sistema cliente
 * @param fun_mensaje Función para enviar un mensaje
 * @param [out] tam_salida Tamaño de la salida
 * @param [out] estado Estado de la operación
 * @param [out] ctx Contexto de la operación
 */
uint8_t* lily_lily_ensamble(const char* datos_entrada, char* arquitectura, struct lily_dict_dict* opciones, struct lily_lily_archivo* (fun_abrir_archivo)(const char*, int, int*), int(fun_cerrar_archivo)(struct lily_lily_archivo*), f_mensajes_ptr fun_mensaje, size_t* tam_salida, enum lily_estado* estado, void** ctx);

uint8_t* lily_lily_desensamble(uint8_t* bytes, f_mensajes_ptr fun_mensaje);

struct lily_lua_ejecucion_maquina* lily_lily_creacion_maquina(char* bytes, size_t tamano, char* arquitectura, struct lily_lua_ejecucion_ctx* ctx);

void lily_lily_ejecutar_instruccion(void* maquina, struct lily_lua_ejecucion_ctx* ctx);

#endif
