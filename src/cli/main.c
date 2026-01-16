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

#define MSG_BUFFER 255

enum lily_main_estricto {
    LILY_MAIN_RELAJADO,
    LILY_MAIN_ESTRICTO,
    LILY_MAIN_SUPERESTRICTO,
};

enum lily_main_etapa {
    LILY_MAIN_INDETERMINADO,
    LILY_MAIN_PREPROCESADO,
    LILY_MAIN_ENSAMBLADO,
    LILY_MAIN_ENLAZADO,
    LILY_MAIN_DESENSAMBLADO,
    LILY_MAIN_EJECUCION,
};

// Configuración de logs
static struct lily_log_config log_cfg = {
    .colores = true,
    .incluir_fecha = true,
    .incluir_hora = true,
    .incluir_archivo = false,
    .nivel_minimo = LILY_LOG_DEBUG
};
static char msg_err[MSG_BUFFER+1];
static enum lily_error estado;

void f_help(char* name);
void f_version(void);
struct lily_dict_nodo* obt_param_valorado(char* arg, struct lily_dict_dict* dict);

int main(int argc, char **argv){
    // Si no hay parámetros, no hay qué seguir
    if (argc == 1) {
        f_help(argv[0]);
        return 0;
    }

    char* archivo_listado_ruta = NULL;
    struct lily_dict_dict macros = { .raiz = NULL, .tamano = 0 };
    char* directorio_fuentes_ruta = NULL;
    bool interactivo = false;
    char* archivo_salida = NULL;
    char* archivo = NULL;
    char* arquitectura = NULL;
    struct lily_lde_lde avisos = { .inicio = NULL, .final = NULL, .tamano = 0 };
    struct lily_lde_lde avisos_no = { .inicio = NULL, .final = NULL, .tamano = 0 };
    struct lily_lde_lde errores = { .inicio = NULL, .final = NULL, .tamano = 0 };
    struct lily_lde_lde errores_no = { .inicio = NULL, .final = NULL, .tamano = 0 };
    struct lily_dict_dict opciones = { .raiz = NULL, .tamano = 0 };
    char* formato_entrada = NULL;
    char* formato_salida = NULL;
    enum lily_main_etapa etapa = LILY_MAIN_INDETERMINADO;
    enum lily_main_estricto estricto = LILY_MAIN_RELAJADO;

    // Parámetros
    int c;
    opterr = 0;
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
        struct lily_dict_nodo* opt_nodo; // <- para D
        bool es_negativo; // <- para W,E
        switch (c) {
            case 'L':
                archivo_listado_ruta = optarg;
                break;
            case 'D':
                opt_nodo = obt_param_valorado(optarg, &macros);
                if (opt_nodo == NULL) {
                    snprintf(msg_err, MSG_BUFFER, _("Error while adding macro \"%s\"."), optarg);
                    log_fatal(&log_cfg, msg_err);
                    return COD_MALLOC_FALLO;
                }
                break;
            case 'I':
                directorio_fuentes_ruta = optarg;
                break;
            case 'i':
                interactivo = true;
                break;
            case 'o':
                archivo_salida = optarg;
                break;
            case 'x':
                arquitectura = optarg;
                break;
            case 'W':
            case 'E':
                es_negativo = !strncmp("no", optarg, 2);
                struct lily_lde_lde *arr;
                if (!es_negativo) arr = (c=='W'?&avisos_no:&errores_no);
                else arr = (c=='W'?&avisos:&errores);
                const struct lily_lde_nodo* nodo = lily_lde_insert(arr, arr->tamano, optarg+(es_negativo?2:0));
                if (nodo == NULL) {
                    snprintf(msg_err, MSG_BUFFER, _("Error while adding %s \"%s\"."), (c=='W'?"warning":"error"), optarg);
                    log_fatal(&log_cfg, msg_err);
                    return COD_MALLOC_FALLO;
                }
                break;
            case 'O':
                opt_nodo = obt_param_valorado(optarg, &opciones);
                if (opt_nodo == NULL) {
                    snprintf(msg_err, MSG_BUFFER, _("Error while adding option \"%s\"."), optarg);
                    log_fatal(&log_cfg, msg_err);
                    return COD_MALLOC_FALLO;
                }
                break;
            case 'p':
                estricto = LILY_MAIN_ESTRICTO;
                break;
            case 'P':
                estricto = LILY_MAIN_SUPERESTRICTO;
                break;
            case 'f':
                formato_entrada = optarg;
                break;
            case 'F':
                formato_salida = optarg;
                break;
            case 's':
                if (!strcmp(optarg, "p")) log_cfg.nivel_minimo = LILY_MAIN_PREPROCESADO;
                else if (!strcmp(optarg, "a")) log_cfg.nivel_minimo = LILY_MAIN_ENSAMBLADO;
                else if (!strcmp(optarg, "l")) log_cfg.nivel_minimo = LILY_MAIN_ENLAZADO;
                else if (!strcmp(optarg, "d")) log_cfg.nivel_minimo = LILY_MAIN_DESENSAMBLADO;
                else if (!strcmp(optarg, "e")) log_cfg.nivel_minimo = LILY_MAIN_EJECUCION;
                else {
                    snprintf(msg_err, MSG_BUFFER, _("The value \"%s\" for parameter stage was not recognized"), optarg);
                    log_error(&log_cfg, msg_err);
                }
                break;
            case 'l':
                if (!strcmp(optarg, "debug")) log_cfg.nivel_minimo = LILY_LOG_DEBUG;
                else if (!strcmp(optarg, "info")) log_cfg.nivel_minimo = LILY_LOG_INFO;
                else if (!strcmp(optarg, "warn")) log_cfg.nivel_minimo = LILY_LOG_WARN;
                else if (!strcmp(optarg, "error")) log_cfg.nivel_minimo = LILY_LOG_ERROR;
                else if (!strcmp(optarg, "fatal")) log_cfg.nivel_minimo = LILY_LOG_FATAL;
                else {
                    snprintf(msg_err, MSG_BUFFER, _("The value \"%s\" for parameter logging was not recognized"), optarg);
                    log_error(&log_cfg, msg_err);
                }
                break;
            case 'h':
                f_help(argv[0]);
                return COD_OK;
            case 'v':
                f_version();
                return COD_OK;
            default:
                if (optopt) {
                    snprintf(msg_err, MSG_BUFFER, _("The parameter '%c' was not recognized."), optopt);
                } else
                    snprintf(msg_err, MSG_BUFFER, _("The parameter '%s' was not recognized."), argv[longopt_idx+1]);
                log_error(&log_cfg, msg_err);
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

void f_version(void) {
    printf(_("Lily %s (commit %s%s)\n"), LILY_VERSION, LILY_COMMIT, LILY_MODIFICADO);
    puts(_("Copyright (C) 2024-2026 Giovanni Alfredo Garciliano Diaz"));
    puts(_("License GNU GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>"));
    puts(_("This is free software: you are free to change and redistribute it"));
    puts(_("There is NO WARRANTY, to the extent permitted by law."));

    puts(_("Report bugs to: <hola@campanita.xyz>"));
    puts(_("home page: <https://github.com/twilight1794/lily/>"));
}

struct lily_dict_nodo* obt_param_valorado(char* arg, struct lily_dict_dict* dict) {
    char* i = arg;
    char* define_nombre = arg;
    char* define_valor = NULL;
    // Obtener nombre
    while (*i != 0) {
        if (*i == '=') {
            *i = 0;
            define_valor = ++i;
            break;
        }
        i++;
    }
    return lily_dict_insert(dict, define_nombre, define_valor, NULL);
}