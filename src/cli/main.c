#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include <libintl.h>

#include "../common/log.h"
#include "../common/dict.h"

#define _(STRING) gettext(STRING)

#define LILY_VERSION "1.0"

void f_help(char* name){
    printf(_("Usage: %s [<params>] <file>\n\n"), name);
    puts(_("Options:"));
    /* Misc */
    puts(_(" -h               Show this help"));
    puts(_(" -v               Show Lily version"));
    /* Stage options */
    puts(_(" -E               Preprocess only, do not assemble and link"));
    puts(_(" -c               Preprocess and compile, do not link"));
    /* Optional files options */
    puts(_(" -e<path>         Generate errors file in <path>"));
    puts(_(" -L<path>         Generate listing file in <path>"));
    puts(_(" -P               Save files generated in preprocess stage"));
    puts(_(" -o<path>         Save output file in <path>"));
    /* Preprocess stage options */
    puts(_(" -D<tag>=<value>  Define a tag <tag> with a value <value>"));
    puts(_(" -I<path>         Search for assembly files in <path>"));
    /* Assembly stage options */
    puts(_(" -a<address>      Address memory in which PC should start"));
    puts(_("\nFor more info, see the man pages."));
}

void f_version(){
    printf(_("Lily %s\n"), LILY_VERSION);
    puts(_("Copyright (C) 2024 Giovanni Alfredo Garciliano Diaz"));
    puts(_("License GNU GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>"));
    puts(_("This is free software: you are free to change and redistribute it"));
    puts(_("There is NO WARRANTY, to the extent permitted by law."));
}

int main(int argc, char **argv){
    char msg_err[100];

    // Configuracion de logs
    struct Lily_Log_Config log_cfg = {
        .use_color = true,
        .show_date = true,
        .show_time = true,
        .show_file = false,
        .level = LILY_LOG_DEBUG
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
    char* archivo_errores_ruta = NULL;
    char* archivo_listado_ruta = NULL;
    char* archivo_final_ruta = NULL;
    char* directorio_fuentes_ruta = NULL;
    size_t direccion_memoria_inicio = 0;
    struct Dict_Dict* variables = Dict_Create();
    char* archivo = NULL;// FIX: Por ahora, un solo archivo

    while ((c = getopt(argc, argv, "Ece:L:Po:D:I:a")) != -1){
        if (c == 'E') opts |= 1;
        else if (c == 'c') opts |= 2;
        else if (c == 'e') archivo_errores_ruta = optarg;
        else if (c == 'L') archivo_listado_ruta = optarg;
        else if (c == 'P') opts |= 4;
        else if (c == 'o') archivo_final_ruta = optarg;
        else if (c == 'D') ; // Hasta implementar bien los diccionarios
        else if (c == 'I') directorio_fuentes_ruta = optarg;
        else if (c == 'a') direccion_memoria_inicio = (size_t) atoi(optarg);
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
    puts(archivo);

    return 0;
}
