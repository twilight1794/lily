/**
 * @file log.h
 * Funciones para generar mensajes de log.
 */

#ifndef LILY_C_LOG
#define LILY_C_LOG

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * Tipos de mensajes de log.
 */
enum lily_log_nivel {
    LILY_LOG_DEBUG, /**< Nivel de depuración, para mensajes de importancia durante el desarrollo. */
    LILY_LOG_INFO, /**< Nivel informativo, para eventos de interés para el usuario que no son problemáticos. */
    LILY_LOG_WARN, /**< Nivel de advertencia, para eventos que, aunque no detengan la ejecución, deberían recibir atención. */
    LILY_LOG_ERROR, /**< Nivel de error, para eventos que detienen la ejecución de una función. */
    LILY_LOG_FATAL, /**< Nivel fatal, para eventos que detienen la ejecución de todo el sistema. */
    LILY_LOG_NONE /*< Nivel para desactivar la escritura de eventos. */
};

/**
 * Objeto para almacenar la configuración de \c lily_log.
 */
struct lily_log_config {
    bool colores; /**< Indica si se usarán caracteres de escape ANSI para colorear los mensajes. */
    bool incluir_fecha; /**< Indica si se mostrará la fecha. */
    bool incluir_hora; /**< Indica si se mostrará la hora. */
    bool incluir_archivo; /**< Indica si se mostrará el número de línea. */
    enum lily_log_nivel nivel_minimo; /**< Indica el nivel mínimo de los mensajes a mostrar. */
};

/**
 * @private Función base para generar los mensajes de log
 * Normalmente no se usa directamente, sino a través de las macros definidas más
 * adelante.
 * @param config Objeto de configuración.
 * @param tipo Nivel de mensaje a emitir.
 * @param archivo Nombre de archivo donde ocurre el mensaje.
 * @param linea Línea donde ocurre el mensaje.
 * @param categoria Identificador de la categoría del mensaje.
 * @param msg Mensaje a mostrar. Si es \c NULL, se considerará que es una función variádica.
 * @param formato Formato a imprimir, para una función variádica.
 */
void lily_log(const struct lily_log_config* config, enum lily_log_nivel tipo, const char* archivo, unsigned int linea, const char* categoria, const char* msg, const char* formato, ...);

/**
 * Imprime un mensaje de nivel de depuración.
 * @param C Objeto de configuración.
 * @param CAT Categoría del mensaje.
 * @param MSG Mensaje a mostrar.
 */
#define log_debug(C, CAT, MSG) lily_log(C, LILY_LOG_DEBUG, __FILE__, __LINE__, CAT, MSG, NULL)

/**
 * Imprime un mensaje de nivel informativo.
 * @param C Objeto de configuración.
 * @param CAT Categoría del mensaje.
 * @param MSG Mensaje a mostrar.
 *
 */
#define log_info(C, CAT, MSG) lily_log(C, LILY_LOG_INFO, __FILE__, __LINE__, CAT, MSG, NULL)

/**
 * Imprime un mensaje de nivel de advertencia.
 * @param C Objeto de configuración.
 * @param CAT Categoría del mensaje.
 * @param MSG Mensaje a mostrar.
 */
#define log_warn(C, CAT, MSG) lily_log(C, LILY_LOG_WARN, __FILE__, __LINE__, CAT, MSG, NULL)

/**
 * Imprime un mensaje de nivel de error.
 * @param C Objeto de configuración.
 * @param CAT Categoría del mensaje.
 * @param MSG Mensaje a mostrar.
 */
#define log_error(C, CAT, MSG) lily_log(C, LILY_LOG_ERROR, __FILE__, __LINE__, CAT, MSG, NULL)

/**
 * Imprime un mensaje de nivel fatal.
 * @param C Objeto de configuración.
 * @param CAT Categoría del mensaje.
 * @param MSG Mensaje a mostrar.
 */
#define log_fatal(C, CAT, MSG) lily_log(C, LILY_LOG_FATAL, __FILE__, __LINE__, CAT, MSG, NULL)

// Versiones con cadenas dinámicas.
/**
 * Como \c log_debug, pero dinámico.
 * @param C Objeto de configuración.
 * @param CAT Categoría del mensaje.
 * @param FORMATO Patrón del mensaje a mostrar.
 */
#define log_debug_v(C, CAT, FORMATO, ...) lily_log(C, LILY_LOG_DEBUG, __FILE__, __LINE__, CAT, NULL, FORMATO, __VA_ARGS__)

/**
 * Como \c log_info, pero dinámico.
 * @param C Objeto de configuración.
 * @param CAT Categoría del mensaje.
 * @param FORMATO Patrón del mensaje a mostrar.
 */
#define log_info_v(C, CAT, FORMATO, ...) lily_log(C, LILY_LOG_INFO, __FILE__, __LINE__, CAT, NULL, FORMATO, __VA_ARGS__)

/**
 * Como \c log_warn, pero dinámico.
 * @param C Objeto de configuración.
 * @param CAT Categoría del mensaje.
 * @param FORMATO Patrón del mensaje a mostrar.
 */
#define log_warn_v(C, CAT, FORMATO, ...) lily_log(C, LILY_LOG_WARN, __FILE__, __LINE__, CAT, NULL, FORMATO, __VA_ARGS__)

/**
 * Como \c log_error, pero dinámico.
 * @param C Objeto de configuración.
 * @param CAT Categoría del mensaje.
 * @param FORMATO Patrón del mensaje a mostrar.
 */
#define log_error_v(C, CAT, FORMATO, ...) lily_log(C, LILY_LOG_ERROR, __FILE__, __LINE__, CAT, NULL, FORMATO, __VA_ARGS__)

/**
 * Como \c log_fatal, pero dinámico.
 * @param C Objeto de configuración.
 * @param CAT Categoría del mensaje.
 * @param FORMATO Patrón del mensaje a mostrar.
 */
#define log_fatal_v(C, CAT, FORMATO, ...) lily_log(C, LILY_LOG_FATAL, __FILE__, __LINE__, CAT, NULL, FORMATO, __VA_ARGS__)

#endif
