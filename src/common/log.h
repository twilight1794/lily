/**
 * @file log.h
 * Funciones para generar mensajes de log
 */

#ifndef LILY_C_LOG
#define LILY_C_LOG

#include <stdio.h>
#include <stdbool.h>
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
    LILY_LOG_FATAL ///< Nivel fatal, para eventos que detienen la ejecución de todo el sistema.
};

/**
 * @private Función base para generar los mensajes de log
 * Normalmente no se usa directamente, sino a través de las macros definidas más
 * adelante
 * @param conf Estructura para configurar la función
 * @param tipo Nivel de mensaje a emitir
 * @param archivo Nombre de archivo donde ocurre el mensaje
 * @param linea Línea donde ocurre el mensaje
 * @param msg Mensaje a mostrar
 */
void lily_log(struct lily_log_config* conf, enum lily_log_nivel tipo, const char* archivo, unsigned int linea, const char* msg);

/**
 * Imprime un mensaje de nivel de depuración
 * @param CONF Estructura para configurar la función
 * @param MSG Mensaje a mostrar
 */
#define log_debug(CONF, MSG) lily_log(CONF, LILY_LOG_DEBUG, __FILE__, __LINE__, MSG)

/**
 * Imprime un mensaje de nivel informativo
 * @param CONF Estructura para configurar la función
 * @param MSG Mensaje a mostrar
 */
#define log_info(CONF, MSG) lily_log(CONF, LILY_LOG_INFO, __FILE__, __LINE__, MSG)

/**
 * Imprime un mensaje de nivel de advertencia
 * @param CONF Estructura para configurar la función
 * @param MSG Mensaje a mostrar
 */
#define log_warn(CONF, MSG) lily_log(CONF, LILY_LOG_WARN, __FILE__, __LINE__, MSG)

/**
 * Imprime un mensaje de nivel de error
 * @param CONF Estructura para configurar la función
 * @param MSG Mensaje a mostrar
 */
#define log_error(CONF, MSG) lily_log(CONF, LILY_LOG_ERROR, __FILE__, __LINE__, MSG)

/**
 * Imprime un mensaje de nivel fatal
 * @param CONF Estructura para configurar la función
 * @param MSG Mensaje a mostrar
 */
#define log_fatal(CONF, MSG) lily_log(CONF, LILY_LOG_FATAL, __FILE__, __LINE__, MSG)

#endif
