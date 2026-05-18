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
 * @file mmap.h
 * Funciones para administrar archivos mapeados en memoria
 */

#ifndef LILY_CLI_MMAP
#define LILY_CLI_MMAP
#define _XOPEN_SOURCE 500

#include <stdbool.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

/**
 * Contenedor para el manejo de archivos mapeados en memoria
 */
struct lily_cli_archivo {
    int fd; /**< Descriptor de archivo */
    struct stat st; /**< Estructura stat */
    char* p; /**< Puntero al contenido del archivo */
};

struct lily_cli_archivo* lily_cli_archivo_create(const char* ruta, size_t escribir);

int lily_cli_archivo_close(struct lily_cli_archivo* obj);

#endif
