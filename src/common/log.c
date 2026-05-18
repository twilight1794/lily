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

#include "log.h"

void lily_log(const struct lily_log_config* config, enum lily_log_nivel tipo, const char* archivo, unsigned int linea, const char* categoria, const char* msg, const char* formato, ...) {
    // Relevancia mínima
    if (tipo < config->nivel_minimo) return;

    // Fecha-hora
    time_t rawtime;
    time(&rawtime);
    const struct tm *timeinfo = localtime(&rawtime);
    char dbuff[11];
    if (config->incluir_fecha) {
        strftime(dbuff, 11, "%F", timeinfo);
        printf("%s ", dbuff);
    }
    if (config->incluir_hora) {
        strftime(dbuff, 11, "%T", timeinfo);
        printf("%s ", dbuff);
    }

    // Tipo
    switch (tipo) {
    case LILY_LOG_DEBUG:
        if (config->colores) printf("\x1b[36;1mDEBUG\x1b[0m ");
        else printf("DEBUG ");
        break;
    case LILY_LOG_INFO:
        if (config->colores) printf("\x1b[32;1mINFO\x1b[0m  ");
        else printf("INFO  ");
        break;
    case LILY_LOG_WARN:
        if (config->colores) printf("\x1b[33;1mWARN\x1b[0m  ");
        else printf("WARN  ");
        break;
    case LILY_LOG_ERROR:
        if (config->colores) printf("\x1b[31;1mERROR\x1b[0m ");
        else printf("ERROR ");
        break;
    case LILY_LOG_FATAL:
        if (config->colores) printf("\x1b[35;1mFATAL\x1b[0m ");
        else printf("FATAL ");
        break;
    }
    if (config->colores) {
        if (config->incluir_archivo) printf("\x1b[90m%s:%-4d \x1b[1m%s:\x1b[0m ", archivo, linea, categoria);
        else printf("\x1b[1m%s:\x1b[0m ", categoria);
    } else {
        if (config->incluir_archivo) printf("%s:%-4d %s: ", archivo, linea, categoria);
        else printf("%s: ", categoria);
    }

    // Archivo-línea
    if (msg != NULL) {
        // Mensaje normal
        printf("%s\n", msg);
    } else {
        // Mensaje compuesto
        va_list args;
        va_start(args, formato);
        vprintf(formato, args);
        va_end(args);
        putchar('\n');
    }
}
