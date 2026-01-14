#include <stdbool.h>
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
#include "../common/error.h"
#include "../common/lde.h"
#include "../common/log.h"
#include "../lib/a_lexico.h"

#define _(STRING) gettext(STRING)

#ifndef LILY_VERSION
#define LILY_VERSION "???"
#endif
#ifndef LILY_COMMIT
#define LILY_COMMIT "???"
#endif
#ifndef LILY_MODIFICADO
#define LILY_MODIFICADO ""
#endif

void f_help(char* name) {
    printf(_("Usage: %s [<params>] <file>\n\n"), name);
    puts(_("Key: P (preprocessor), A (assembly),"));
    puts(_("     L (linking), D (disassembly), E (execution)"));
    /* Opciones del preprocesador */
    puts(_("\nPreprocessor options:"));
    puts(_(" -L<path>           --listing=<path>          Generate listing file in <path>"));
    puts(_(" -D<tag>[=<value>]  --define=<tag>[=<value>]  Define a tag <tag> with a value <value>"));
    puts(_(" -I<path>           --include=<path>          Search for assembly files first in <path>"));
    /* Opciones de ensamble */
    /* Opciones de enlazado */
    /* Opciones de desensamble */
    /* Opciones de ejecución */
    puts(_("\nExecution options:"));
    puts(_(" -i  --interactive  Show a simple CLI that shows the state of the hypervisor"));
    //puts(_(" -m=<mapping>  --mem-mapping=<mapping>  Map a memory address with a device"));
    //puts(_(" -s=<file>     --state=<file>           "));
    //puts(_(" -S            --step                   Execute a single mnemonic and exit, saving state."));
    /* Opciones para varias etapas */
    puts(_("\nMultistage options:"));
    puts(_(" -o<file>              --output=<file>                Output file (all)"));
    puts(_(" -x<arch>              --architecture=<arch-or-file>  Working architecture (all except L)"));
    puts(_(" -W<warning>           --warning=<warning>            Enables warning <warning> (all)"));
    puts(_(" -Wno<warning>         --warning=no<warning>          Disables warning <warning> (all)"));
    puts(_(" -E<error>             --error=<error>                Enables error <error> (all)"));
    puts(_(" -Eno<error>           --error=no<error>              Disables error <error> (all)"));
    puts(_(" -O<option>[=<value>]  --option=<option>[=<value>]    Passes option <option> to stage (all)"));
    puts(_(" -p                    --pedantic                     Enables all warnings (all)"));
    puts(_(" -P                    --superpedantic                Enables all warnings and treat them as errors (all)"));
    puts(_(" -f<format>            --input-format=<format>        Format of input object (D, E)"));
    puts(_(" -F<format>            --output-format=<format>       Format of output object (L)"));
    /* Miscelánea */
    puts(_("\nMiscellaneous options:"));
    puts(_(" -s<stage>   --stage=<stage>    Operation to perform: P, A, L, D, or E"));
    puts(_(" -l=<level>  --logging=<level>  Restrict output to <level>"));
    puts(_(" -h          --help             Show this help")); //NOTE=class[,…]
    puts(_(" -v          --version          Show Lily version"));
    puts(_("\nFor more info, see the man pages."));
}

void f_version(void){
    printf(_("Lily %s (commit %s%s)\n"), LILY_VERSION, LILY_COMMIT, LILY_MODIFICADO);
    puts(_("Copyright (C) 2024-2026 Giovanni Alfredo Garciliano Diaz"));
    puts(_("License GNU GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>"));
    puts(_("This is free software: you are free to change and redistribute it"));
    puts(_("There is NO WARRANTY, to the extent permitted by law."));

    puts(_("Report bugs to: <hola@campanita.xyz>"));
    puts(_("home page: <https://github.com/twilight1794/lily/>"));
}

int main(int argc, char **argv){
    char msg_err[100];
    enum lily_error estado;

    // Configuracion de logs
    struct lily_log_config log_cfg = {
        .colores = true,
        .incluir_fecha = true,
        .incluir_hora = true,
        .incluir_archivo = false,
        .nivel_minimo = LILY_LOG_DEBUG
    };

    // Si no hay parámetros, no hay qué
    if (argc == 1) {
        f_help(argv[0]);
        return 0;
    }

    char* archivo_listado_ruta = NULL;
    struct lily_dict_dict macros = { .raiz = NULL, .tamano = 0 };
    char* directorio_fuentes_ruta = NULL;
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
    // Parámetros
    int c;
    //opterr = 0;
    int longopt_idx = 0;
    static struct option longopt_lista[] = {
        { "listing", required_argument, NULL, 'L' },
        { "define", required_argument, NULL, 'D' },
        { "include",required_argument, NULL, 'I' },
        { "interactive", no_argument, NULL, 'i' },
        { "output", required_argument, NULL, 'o' },
        { "architecture", required_argument, NULL, 'x' },
        { "warning", required_argument, NULL, 'W' },
        { "error", required_argument, NULL, 'E' },
        { "option", required_argument, NULL, 'O' },
        { "pedantic", no_argument, NULL, 'p' },
        { "superpedantic", no_argument, NULL, 'P' },
        { "input-format", required_argument, NULL, 'f' },
        { "output-format", required_argument, NULL, 'F' },
        { "stage", required_argument, NULL, 's' },
        { "logging", required_argument, NULL, 'l' },
        { "help", no_argument, NULL, 'h' },
        { "version", no_argument, NULL, 'v' },
        { NULL, 0, NULL, 0 }
    };
    while ((c = getopt_long(argc, argv, "L:D:I:io:x:W:E:O:pPf:F:s:l:hv", longopt_lista, &longopt_idx)) != -1) {
        switch (c) {
            case 'L':
                archivo_listado_ruta = optarg;
                break;
            case 'D':
                break;
            case 'I':
                directorio_fuentes_ruta = optarg;
                break;
            case 'i':
                break;
            case 'o':
                break;
            case 'x':
                break;
            case 'W':
                break;
            case 'E':
                break;
            case 'O':
                break;
            case 'p':
                break;
            case 'P':
                break;
            case 'f':
                break;
            case 'F':
                break;
            case 's':
                break;
            case 'l':
                break;
            case 'h':
                f_help(argv[0]);
                return 0;
            case 'v':
                f_version();
                return 0;
            default:
                snprintf(msg_err, 99, _("The argument '%c' was not recognized."), c);
                log_fatal(&log_cfg, msg_err);
                exit(EXIT_FAILURE);
        }
    };

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
    int codigo = 0;/*lily_lex_lexico(p_archivo, simbolos);*/
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
