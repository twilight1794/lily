/**
 * @file log.h
 * Funciones para generar mensajes de log
 */

#ifndef LILY_C_LOG
#define LILY_C_LOG

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * Objeto para almacenar la configuración de \c lily_log
 */
struct lily_log_config {
    bool colores; ///< Indica si se usarán caracteres de escape ANSI para colorear los mensajes.
    bool incluir_fecha; ///< Indica si se mostrará la fecha.
    bool incluir_hora; ///< Indica si se mostrará la hora.
    bool incluir_archivo; ///< Indica si se mostrará el número de línea.
    int nivel_minimo; ///< Indica el nivel mínimo de los mensajes a mostrar.
};

/**
 * Tipos de mensajes de log
 */
enum lily_log_nivel {
    LILY_LOG_DEBUG, ///< Nivel de depuración, para mensajes de importancia durante el desarrollo.
    LILY_LOG_INFO, ///< Nivel informativo, para eventos de interés para el usuario que no son problemáticos.
    LILY_LOG_WARN, ///< Nivel de advertencia, para eventos que, aunque no detengan la ejecución, deberían recibir atención.
    LILY_LOG_ERROR, ///< Nivel de error, para eventos que detienen la ejecución de una función.
    LILY_LOG_FATAL, ///< Nivel fatal, para eventos que detienen la ejecución de todo el sistema.
    LILY_LOG_NONE ///< Nivel para desactivar la escritura de eventos.
};

/**
 * Espacio para configuración
 */
extern struct lily_log_config lily_log_conf;
extern bool lily_log_msg_null;
extern char* lily_log_msg_buffer;

/**
 * @private Función base para generar los mensajes de log
 * Normalmente no se usa directamente, sino a través de las macros definidas más
 * adelante
 * @param tipo Nivel de mensaje a emitir
 * @param archivo Nombre de archivo donde ocurre el mensaje
 * @param linea Línea donde ocurre el mensaje
 * @param msg Mensaje a mostrar
 */
void lily_log(enum lily_log_nivel tipo, const char* archivo, unsigned int linea, const char* msg);

/**
 * Imprime un mensaje de nivel de depuración
 * @param MSG Mensaje a mostrar
 */
#define log_debug(MSG) lily_log(LILY_LOG_DEBUG, __FILE__, __LINE__, MSG)

/**
 * Imprime un mensaje de nivel informativo
 * @param MSG Mensaje a mostrar
 */
#define log_info(MSG) lily_log(LILY_LOG_INFO, __FILE__, __LINE__, MSG)

/**
 * Imprime un mensaje de nivel de advertencia
 * @param MSG Mensaje a mostrar
 */
#define log_warn(MSG) lily_log(LILY_LOG_WARN, __FILE__, __LINE__, MSG)

/**
 * Imprime un mensaje de nivel de error
 * @param MSG Mensaje a mostrar
 */
#define log_error(MSG) lily_log(LILY_LOG_ERROR, __FILE__, __LINE__, MSG)

/**
 * Imprime un mensaje de nivel fatal
 * @param MSG Mensaje a mostrar
 */
#define log_fatal(MSG) lily_log(LILY_LOG_FATAL, __FILE__, __LINE__, MSG)

// Versiones con cadenas dinámicas

/**
 * \c snprintf con cadenas dinámicas
 * @param FORMATO Patrón a pasar a \c snprintf
 * @param VALS Parámetros variádicos para \c snprintf
 * @return Puntero a cadena generada
 */
#define lily_log_gen_msg(FORMATO, VALS) \
lily_log_msg_buffer = malloc(snprintf(NULL, 0, FORMATO, VALS) + 1); \
if (lily_log_msg_buffer == NULL) lily_log_msg_null = true; \
else sprintf(lily_log_msg_buffer, FORMATO, VALS);

/**
 * Como \c log_debug, pero dinámico
 * @param MSG Mensaje a mostrar
 */
#define log_debug_gen(FORMATO, VALS) \
lily_log_msg_buffer = malloc(snprintf(NULL, 0, FORMATO, VALS) + 1); \
if (lily_log_msg_buffer == NULL) lily_log_msg_null = true; \
else { \
sprintf(lily_log_msg_buffer, FORMATO, VALS); \
log_debug(lily_log_msg_buffer); \
free(lily_log_msg_buffer); }

/**
 * Como \c log_info, pero dinámico
 * @param MSG Mensaje a mostrar
 */
#define log_info_gen(FORMATO, VALS) \
lily_log_msg_buffer = malloc(snprintf(NULL, 0, FORMATO, VALS) + 1); \
if (lily_log_msg_buffer == NULL) lily_log_msg_null = true; \
else { \
sprintf(lily_log_msg_buffer, FORMATO, VALS); \
log_info(lily_log_msg_buffer); \
free(lily_log_msg_buffer); }

/**
 * Como \c log_warn, pero dinámico
 * @param MSG Mensaje a mostrar
 */
#define log_warn_gen(FORMATO, VALS) \
lily_log_msg_buffer = malloc(snprintf(NULL, 0, FORMATO, VALS) + 1); \
if (lily_log_msg_buffer == NULL) lily_log_msg_null = true; \
else { \
sprintf(lily_log_msg_buffer, FORMATO, VALS); \
log_warn(lily_log_msg_buffer); \
free(lily_log_msg_buffer); }

/**
 * Como \c log_error, pero dinámico
 * @param MSG Mensaje a mostrar
 */
#define log_error_gen(FORMATO, VALS) \
lily_log_msg_buffer = malloc(snprintf(NULL, 0, FORMATO, VALS) + 1); \
if (lily_log_msg_buffer == NULL) lily_log_msg_null = true; \
else { \
sprintf(lily_log_msg_buffer, FORMATO, VALS); \
log_error(lily_log_msg_buffer); \
free(lily_log_msg_buffer); }

/**
 * Como \c log_fatal, pero dinámico
 * @param MSG Mensaje a mostrar
 */
#define log_fatal_gen(FORMATO, VALS) \
lily_log_msg_buffer = malloc(snprintf(NULL, 0, FORMATO, VALS) + 1); \
if (lily_log_msg_buffer == NULL) lily_log_msg_null = true; \
else { \
sprintf(lily_log_msg_buffer, FORMATO, VALS); \
log_fatal(lily_log_msg_buffer); \
free(lily_log_msg_buffer); }

#endif
