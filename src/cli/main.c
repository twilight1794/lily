#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <getopt.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <libintl.h>

#include "../common/dict.h"
#include "../common/lde.h"
#include "../common/log.h"
#include "../lib/a_lexico.h"

#define _(STRING) gettext(STRING)

#define LILY_VERSION "1.0"

void f_help(char* name){
    printf(_("Usage: %s [<params>] <file>\n\n"), name);
    puts(_("Options:"));
    /* Misc */
    puts(_(" -h               Show this help"));
    puts(_(" -v               Show Lily version"));
    /* Preprocess stage options */
    puts(_(" -L<path>         Generate listing file in <path>"));
    puts(_(" -D<tag>=<value>  Define a tag <tag> with a value <value>"));
    puts(_(" -I<path>         Search for assembly files in <path>"));
    puts(_("\nFor more info, see the man pages."));
    /* Assembly stage options */
    puts(_(" -A<path>         Save file generated in assemble stage in <path>"));
}

void f_version(void){
    printf(_("Lily %s\n"), LILY_VERSION);
    puts(_("Copyright (C) 2024 Giovanni Alfredo Garciliano Diaz"));
    puts(_("License GNU GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>"));
    puts(_("This is free software: you are free to change and redistribute it"));
    puts(_("There is NO WARRANTY, to the extent permitted by law."));
}

int main(int argc, char **argv){
    char msg_err[100];

    // Configuracion de logs
    struct lily_log_config log_cfg = {
        .colores = true,
        .incluir_fecha = true,
        .incluir_hora = true,
        .incluir_archivo = false,
        .nivel_minimo = LILY_LOG_DEBUG
    };

    // Filtrar primero las opciones de ayuda y versión
    if (argc == 1 || !strcmp(argv[1], "-h")) {
        f_help(argv[0]);
        return 0;
    } else if (!strcmp(argv[1], "-v")){
        f_version();
        return 0;
    }

    // Parámetros
    int c;
    opterr = 0;

    unsigned char opts = 0;
    char* archivo_listado_ruta = NULL;
    char* archivo_ensamblado_ruta = NULL;
    char* directorio_fuentes_ruta = NULL;
    struct lily_dict_dict* variables = lily_dict_create();
    char* archivo = NULL;

    while ((c = getopt(argc, argv, "L:D:I:A:")) != -1){
        if (c == 'L') archivo_listado_ruta = optarg;
        else if (c == 'D') ; // Hasta implementar bien los diccionarios
        else if (c == 'I') directorio_fuentes_ruta = optarg;
        else if (c == 'A') archivo_ensamblado_ruta = optarg;
        else {
            snprintf(msg_err, 99, _("The argument %c was not recognized."), c);
            log_fatal(&log_cfg, msg_err);
            exit(EXIT_FAILURE);
        }
    }

    // Listas de archivos
    // FIX: Por ahora, un solo archivo
    if (optind == argc){
        log_error(&log_cfg, _("An input file was not specified."));
        exit(EXIT_FAILURE);
    } else archivo = argv[optind++];

    // Abrimos archivo
    int fd = open(archivo, O_RDONLY);
    if (fd == -1){
        snprintf(msg_err, 99, _("File %s cannot be open."), archivo);
        log_fatal(&log_cfg, msg_err);
        exit(EXIT_FAILURE);
    }
    struct stat st;
    fstat(fd, &st);
    char* p_archivo = (char*) mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);

    // Empezamos análisis
    struct lily_lde_lde* simbolos = lily_lde_create();
    int codigo = lily_lex_lexico(p_archivo, simbolos);
    munmap(p_archivo, st.st_size);
    close(fd);
    if (codigo) return codigo;

    //struct lily_lde_lde* ast = lily_lde_create();
    //codigo = z80_sintactico(simbolos, ast);
    //if (codigo) return codigo;

    //struct lily_lde_lde* objeto = lily_lde_create();
    //codigo = z80_semantico(ast, objeto);
    //if (codigo) return codigo;

    return 0;
}
