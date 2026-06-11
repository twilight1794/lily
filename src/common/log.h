/*
 * Copyright (C) 2024-2026 Giovanni Alfredo Garciliano Diaz
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
 * @file log.h
 * Funciones para generar mensajes de log.
 */

#ifndef LILY_C_LOG
#define LILY_C_LOG

#define _POSIX_C_SOURCE 199309L

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#include <unistd.h>
#endif

/**
 * Tipos de mensajes de log.
 */
enum lily_log_nivel {
    LILY_LOG_DEBUG, /**< Nivel de depuración, para mensajes de importancia durante el desarrollo. */
    LILY_LOG_INFO, /**< Nivel informativo, para eventos de interés para el usuario que no son problemáticos. */
    LILY_LOG_WARN, /**< Nivel de advertencia, para eventos que, aunque no detengan la ejecución, deberían recibir atención. */
    LILY_LOG_ERROR, /**< Nivel de error, para eventos que detienen la ejecución de una función. */
    LILY_LOG_FATAL, /**< Nivel fatal, para eventos que detienen la ejecución de todo el sistema. */
    LILY_LOG_NONE /**< Nivel para desactivar la escritura de eventos. */
};

/**
 * Objeto para almacenar la configuración de \c lily_log.
 */
struct lily_log_config {
    bool colores; /**< Indica si se usarán caracteres de escape ANSI para colorear los mensajes. */
    bool incluir_fecha; /**< Indica si se mostrará la fecha y la hora. */
    enum lily_log_nivel nivel_minimo; /**< Indica el nivel mínimo de los mensajes a mostrar. */
};

/**
 * @private Función base para generar los mensajes de log
 * Normalmente no se usa directamente, sino a través de las macros definidas más
 * adelante.
 * @param config Objeto de configuración.
 * @param tipo Nivel de mensaje a emitir.
 * @param categoria Identificador de la categoría del mensaje.
 * @param msg Mensaje a mostrar. Si es \c NULL, se considerará que es una función variádica.
 * @param formato Formato a imprimir, para una función variádica.
 */
void lily_log(const struct lily_log_config* config, enum lily_log_nivel tipo, const char* categoria, const char* msg, const char* formato, ...);

/**
 * Imprime un mensaje de nivel de depuración.
 * @param C Objeto de configuración.
 * @param CAT Categoría del mensaje.
 * @param MSG Mensaje a mostrar.
 */
#define log_debug(C, CAT, MSG) lily_log(C, LILY_LOG_DEBUG, CAT, MSG, NULL)

/**
 * Imprime un mensaje de nivel informativo.
 * @param C Objeto de configuración.
 * @param CAT Categoría del mensaje.
 * @param MSG Mensaje a mostrar.
 *
 */
#define log_info(C, CAT, MSG) lily_log(C, LILY_LOG_INFO, CAT, MSG, NULL)

/**
 * Imprime un mensaje de nivel de advertencia.
 * @param C Objeto de configuración.
 * @param CAT Categoría del mensaje.
 * @param MSG Mensaje a mostrar.
 */
#define log_warn(C, CAT, MSG) lily_log(C, LILY_LOG_WARN, CAT, MSG, NULL)

/**
 * Imprime un mensaje de nivel de error.
 * @param C Objeto de configuración.
 * @param CAT Categoría del mensaje.
 * @param MSG Mensaje a mostrar.
 */
#define log_error(C, CAT, MSG) lily_log(C, LILY_LOG_ERROR, CAT, MSG, NULL)

/**
 * Imprime un mensaje de nivel fatal.
 * @param C Objeto de configuración.
 * @param CAT Categoría del mensaje.
 * @param MSG Mensaje a mostrar.
 */
#define log_fatal(C, CAT, MSG) lily_log(C, LILY_LOG_FATAL, CAT, MSG, NULL)

// Versiones con cadenas dinámicas.
/**
 * Como \c log_debug, pero dinámico.
 * @param C Objeto de configuración.
 * @param CAT Categoría del mensaje.
 * @param FORMATO Patrón del mensaje a mostrar.
 */
#define log_debug_v(C, CAT, FORMATO, ...) lily_log(C, LILY_LOG_DEBUG, CAT, NULL, FORMATO, __VA_ARGS__)

/**
 * Como \c log_info, pero dinámico.
 * @param C Objeto de configuración.
 * @param CAT Categoría del mensaje.
 * @param FORMATO Patrón del mensaje a mostrar.
 */
#define log_info_v(C, CAT, FORMATO, ...) lily_log(C, LILY_LOG_INFO, CAT, NULL, FORMATO, __VA_ARGS__)

/**
 * Como \c log_warn, pero dinámico.
 * @param C Objeto de configuración.
 * @param CAT Categoría del mensaje.
 * @param FORMATO Patrón del mensaje a mostrar.
 */
#define log_warn_v(C, CAT, FORMATO, ...) lily_log(C, LILY_LOG_WARN, CAT, NULL, FORMATO, __VA_ARGS__)

/**
 * Como \c log_error, pero dinámico.
 * @param C Objeto de configuración.
 * @param CAT Categoría del mensaje.
 * @param FORMATO Patrón del mensaje a mostrar.
 */
#define log_error_v(C, CAT, FORMATO, ...) lily_log(C, LILY_LOG_ERROR, CAT, NULL, FORMATO, __VA_ARGS__)

/**
 * Como \c log_fatal, pero dinámico.
 * @param C Objeto de configuración.
 * @param CAT Categoría del mensaje.
 * @param FORMATO Patrón del mensaje a mostrar.
 */
#define log_fatal_v(C, CAT, FORMATO, ...) lily_log(C, LILY_LOG_FATAL, CAT, NULL, FORMATO, __VA_ARGS__)

#endif
