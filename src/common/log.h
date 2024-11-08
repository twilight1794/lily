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
 * Objeto de configuración
 */
struct Lily_Log_Config {
    bool use_color; /**< Indica si se usarán caracteres de escape ANSI para colorear los mensajes */
    bool show_date; /**< Indica si se mostrará la fecha */
    bool show_time; /**< Indica si se mostrará la hora */
    bool show_file; /**< Indica si se mostrará el número de línea */
    int level; /**< Indica el nivel mínimo de los mensajes a mostrar */
};

enum { LILY_LOG_DEBUG, LILY_LOG_INFO, LILY_LOG_WARN, LILY_LOG_ERROR, LILY_LOG_FATAL };

#define log_debug(CONF, MSG) lily_log(CONF, LILY_LOG_DEBUG, __FILE__, __LINE__, MSG)
#define log_info(CONF, MSG) lily_log(CONF, LILY_LOG_INFO, __FILE__, __LINE__, MSG)
#define log_warn(CONF, MSG) lily_log(CONF, LILY_LOG_WARN, __FILE__, __LINE__, MSG)
#define log_error(CONF, MSG) lily_log(CONF, LILY_LOG_ERROR, __FILE__, __LINE__, MSG)
#define log_fatal(CONF, MSG) lily_log(CONF, LILY_LOG_FATAL, __FILE__, __LINE__, MSG)

void lily_log(struct Lily_Log_Config*, int, const char*, unsigned int, const char*);

#endif
