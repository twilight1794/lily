#include <stdio.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../../src/lib/z80_lexico.h"
#include "../../src/common/lde.h"

int main(int argc; char** argv){
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) exit(EXIT_FAILURE);
    struct stat st;
    fstat(fd, &st);
    char* p_archivo = (char*) mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);

    // Empezamos análisis léxico
    struct LDE_LDE* simbolos = LDE_Create();
    int codigo = z80_lexico(p_archivo, simbolos);
    munmap(p_archivo, st.st_size);
    if (codigo) return codigo;

    for (size_t i=0; i<LDE_Size(simbolos); i++){
        struct LDE_Nodo* nodo = LDE_Get(simbolos, i);
        struct Z80_Lex_Simbolo* sim = nodo->valor;
        printf("Nodo %lu (tipo %d): ", i, sim->tipo);
        if (sim->etiqueta != NULL) printf("eti='%s' ", sim->etiqueta);
        if (sim->valor != NULL) printf("val='%s' ", sim->valor);
        if (sim->expresion != NULL) printf("exp='%s' ", sim->expresion);
        puts("");
    }
}
