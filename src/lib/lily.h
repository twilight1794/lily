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
 * @param fun_abrir_archivo Función para solicitar un archivo al sistema cliente
 * @param fun_cerrar_archivo Función para cerrar un archivo en el sistema cliente
 * @param fun_mensaje Función para enviar un mensaje
 * @param [out] tam_salida Tamaño de la salida
 * @param [out] estado Estado de la operación
 * @param [out] ctx Contexto de la operación
 */
uint8_t* lily_lily_ensamble(const char* datos_entrada, char* arquitectura, struct lily_lily_archivo* (fun_abrir_archivo)(const char*, int, int*), int(fun_cerrar_archivo)(struct lily_lily_archivo*), f_mensajes_ptr fun_mensaje, size_t* tam_salida, enum lily_estado* estado, void** ctx);

uint8_t* lily_lily_desensamble(uint8_t* bytes, f_mensajes_ptr fun_mensaje);

#endif
