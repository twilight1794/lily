#include <stdio.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../../src/lib/z80.h"
#include "../../src/lib/z80_exp.h"
#include "../../src/common/lde.h"

int main(int argc, char** argv){
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
        puts("");
        if (sim->expresion != NULL){
            for (size_t j=0; j<LDE_Size(sim->expresion); j++){
                struct Exp_Simbolo* nodo_exp = LDE_Get(sim->expresion, j)->valor;
                switch (nodo_exp->tipo){
                    case VAL_REGISTRO:
                        printf("\tRegistro: '%s'\n", (char*) nodo_exp->valor); break;
                    case VAL_BANDERA:
                        printf("\tBandera: '%s'\n", (char*) nodo_exp->valor); break;
                    case VAL_AMB_C:
                        printf("\tAmb_c\n"); break;
                    case VAL_NUMERO:
                        printf("\tNum: '%ld'\n", *((long*) nodo_exp->valor)); break;
                    case VAL_OP:
                        printf("\tOp: '%s'\n", (char*) nodo_exp->valor); break;
                    case VAL_ETIQUETA:
                        printf("\tEti: '%s'\n", (char*) nodo_exp->valor); break;
                    case VAL_CADENA:
                        printf("\tCadena: '%s'\n", (char*) nodo_exp->valor); break;
                    case VAL_PARENTESIS_AP:
                        printf("\tParen_ap\n"); break;
                    case VAL_PARENTESIS_CI:
                        printf("\tParen_ci\n"); break;
                    case VAL_DESP_AP:
                        printf("\tDesp_ap\n"); break;
                    case VAL_DESP_CI:
                        printf("\tDesp_ci\n"); break;
                    case VAL_SEPARADOR:
                        printf("\tSeparador\n"); break;
                    default:
                        printf("\tDesconocido\n"); break;
                }
            }
        }
    }
}
