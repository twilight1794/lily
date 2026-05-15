/**
 * @file mensajes.h
 * Funciones para comunicación con el sistema cliente.
 */

#ifndef LILY_L_MENSAJES
#define LILY_L_MENSAJES

#include <stddef.h>
#include <stdint.h>

enum lily_lily_mensaje_tipo {
    LILY_MENSAJE_TLOG, /**< Mensajes de log */
    LILY_MENSAJE_TADVERTENCIA, /**< Situaciones definidas que requieren notificarse al usuario */
    LILY_MENSAJE_TETIQUETA, /**< Definiciones de etiquetas */
    LILY_MENSAJE_TVARIABLE, /**< Definiciones de variables y constantes */
    LILY_MENSAJE_TMEMORIA, /**< Lecturas y escrituras de memoria */
    LILY_MENSAJE_TREGISTRO, /**< Lecturas y escrituras de registros */
    LILY_MENSAJE_TPILA, /**< Operaciones sobre la pila, de estar definida */
    LILY_MENSAJE_TDISPOSITIVO, /**< E/S a dispositivos */
    LILY_MENSAJE_TINTERRUPCION, /**< Eventos de interrupciones */
};

typedef int (*f_mensajes_ptr)(enum lily_lily_mensaje_tipo, int, char*, void*);

enum lily_lily_mensaje_tmemoria_subtipo {
    LILY_MENSAJE_TMEMORIA_LECTURA,
    LILY_MENSAJE_TMEMORIA_ESCRITURA
};

struct lily_lily_mensaje_tmemoria {
  uint64_t direccion; /**< Dirección de memoria involucrada */
  uint64_t valor;     /**< Valor leído o escrito */
  int tamano;         /**< Tamaño del bloque de memoria, hasta 10 bytes */
};

enum lily_lily_mensaje_tregistro_subtipo {
    LILY_MENSAJE_TREGISTRO_LECTURA,
    LILY_MENSAJE_TREGISTRO_ESCRITURA,
    LILY_MENSAJE_TREGISTRO_CREACION_CTX,
    LILY_MENSAJE_TREGISTRO_RESTAURACION_CTX,
    LILY_MENSAJE_TREGISTRO_ELIMINACION_CTX,
};

struct lily_lily_mensaje_tregistro {
  uint64_t valor; /**< Valor leído o escrito, identificador para operaciones de
                     contexto */
  uint64_t desplazamiento; /**< Desplazamiento del registro involucrado para
                              lecturas y escrituras, 0 en cualquier otro caso */
  const char* registro; /**< Registro involucrado para lecturas y escrituras, \c NULL
                     en cualquier otro caso */
  int tamano; /**< Tamaño del registro involucrado para lecturas y escrituras, 0
                 en cualquier otro caso */
};

enum lily_lily_mensaje_tpila_subtipo {
    LILY_MENSAJE_TPILA_LECTURA,
    LILY_MENSAJE_TPILA_ESCRITURA,
    LILY_MENSAJE_TPILA_CREACION_CTX,
    LILY_MENSAJE_TPILA_RESTAURACION_CTX,
    LILY_MENSAJE_TPILA_ELIMINACION_CTX,
};

struct lily_lily_mensaje_tpila {
    uint64_t valor; /**< Valor añadido, 0 al quitar, identificador para operaciones de contexto */
    size_t tamano; /**< Tamaño de los valores, al crear un contexto */
};

#endif
