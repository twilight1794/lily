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

//#include "../common/estado.h"

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
