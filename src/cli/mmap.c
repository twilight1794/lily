#include "mmap.h"

#include <stdio.h>

struct lily_cli_archivo* lily_cli_archivo_create(const char* ruta, size_t escribir) {
    struct lily_cli_archivo* obj = (struct lily_cli_archivo*) malloc(sizeof(struct lily_cli_archivo));
    // Descriptor
    if (obj == NULL) return NULL;
    if (escribir)
        obj->fd = open(ruta, O_RDWR | O_CREAT | O_TRUNC, 0660);
    else
        obj->fd = open(ruta, O_RDONLY);
    if (obj->fd == -1) {
        free(obj);
        return NULL;
    }
    // Ampliar para escritura
    if (escribir && ftruncate(obj->fd, (long) escribir)) {
        free(obj);
        return NULL;
    }
    // Objeto stat
    fstat(obj->fd, &(obj->st));
    // Mmap
    if (escribir) {
        obj->p = mmap(NULL, escribir, PROT_READ | PROT_WRITE, MAP_SHARED, obj->fd, 0);
    } else {
        obj->p = mmap(NULL, obj->st.st_size, PROT_READ, MAP_SHARED, obj->fd, 0);
    }
    if (obj->p == MAP_FAILED) {
        free(obj);
        return NULL;
    }
    return obj;
}

int lily_cli_archivo_close(struct lily_cli_archivo* obj) {
    if (obj == NULL) return -1;
    if (munmap(obj->p, obj->st.st_size)) return -1;
    close(obj->fd);
    free(obj);
    return 0;
}
