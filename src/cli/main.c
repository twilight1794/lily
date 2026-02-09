#include <stdbool.h>
#include <stddef.h>
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
char* obt_extension(const char *nombre);
void obt_nombre_archivo(char* nombre, enum lily_main_etapa etapa, char* archivo_salida);
char* obt_etapa_str(enum lily_main_etapa etapa);

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
    char* archivo_entrada = NULL;
    char* archivo_salida = NULL;
    char* arquitectura = NULL;
    struct lily_lde_lde avisos = { .inicio = NULL, .final = NULL, .tamano = 0 };
    struct lily_lde_lde avisos_no = { .inicio = NULL, .final = NULL, .tamano = 0 };
    struct lily_lde_lde errores = { .inicio = NULL, .final = NULL, .tamano = 0 };
    struct lily_lde_lde errores_no = { .inicio = NULL, .final = NULL, .tamano = 0 };
    struct lily_dict_dict opciones = { .raiz = NULL, .tamano = 0 };
    char* formato_entrada = NULL;
    char* formato_salida = NULL;
    enum lily_main_etapa etapa_inicial = LILY_MAIN_INDETERMINADO;
    enum lily_main_etapa etapa_final = LILY_MAIN_INDETERMINADO;
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
                if (!strcmp(optarg, "p")) {
                    etapa_inicial = LILY_MAIN_PREPROCESADO;
                    etapa_final = LILY_MAIN_PREPROCESADO;
                }
                else if (!strcmp(optarg, "a")) {
                    etapa_inicial = LILY_MAIN_ENSAMBLADO;
                    etapa_final = LILY_MAIN_ENSAMBLADO;
                }
                else if (!strcmp(optarg, "l")) {
                    etapa_inicial = LILY_MAIN_ENLAZADO;
                    etapa_final = LILY_MAIN_ENLAZADO;
                }
                else if (!strcmp(optarg, "d")) {
                    etapa_inicial = LILY_MAIN_DESENSAMBLADO;
                    etapa_final = LILY_MAIN_DESENSAMBLADO;
                }
                else if (!strcmp(optarg, "e")) {
                    etapa_inicial = LILY_MAIN_EJECUCION;
                    etapa_final = LILY_MAIN_EJECUCION;
                }
                else {
                    snprintf(msg_err, MSG_BUFFER, _("The value \"%s\" for parameter stage was not recognized"), optarg);
                    log_fatal(&log_cfg, msg_err);
                    exit(EXIT_FAILURE);
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
        log_fatal(&log_cfg, _("An input file was not specified."));
        exit(EXIT_FAILURE);
    } else archivo_entrada = argv[optind++];

    // Abrimos archivo
    int archivo_entrada_fd = open(archivo_entrada, O_RDONLY);
    if (archivo_entrada_fd == -1){
        snprintf(msg_err, MSG_BUFFER, _("File %s cannot be open."), archivo_entrada);
        log_fatal(&log_cfg, msg_err);
        exit(EXIT_FAILURE);
    }
    struct stat archivo_entrada_st;
    fstat(archivo_entrada_fd, &archivo_entrada_st);
    char* archivo_entrada_p = (char*) mmap(NULL, archivo_entrada_st.st_size, PROT_READ, MAP_SHARED, archivo_entrada_fd, 0);

    // Determinar operación inicial y final
    /// Parámetro origen->origen, destino, arriba en la lectura de argumentos
    /// Extensión origen->origen
    if (etapa_inicial == LILY_MAIN_INDETERMINADO) {
        char* extension = obt_extension(archivo_entrada);
        if (!strcmp(extension, "asm")) etapa_inicial = LILY_MAIN_PREPROCESADO;
        else if (!strcmp(extension, "s")) etapa_inicial = LILY_MAIN_ENSAMBLADO;
        else if (!strcmp(extension, "o")) etapa_inicial = LILY_MAIN_ENLAZADO;
        else if (!strcmp(extension, "com")) etapa_inicial = LILY_MAIN_EJECUCION;
    }
    /// Extensión destino->destino, origen si no determinado
    if (etapa_final == LILY_MAIN_INDETERMINADO && archivo_salida != NULL) {
        char* extension = obt_extension(archivo_salida);
        if (!strcmp(extension, "s")) {
            etapa_final = LILY_MAIN_PREPROCESADO;
            if (etapa_inicial == LILY_MAIN_INDETERMINADO) etapa_inicial = LILY_MAIN_PREPROCESADO;
        }
        else if (!strcmp(extension, "o")) {
            etapa_final = LILY_MAIN_ENSAMBLADO;
            if (etapa_inicial == LILY_MAIN_INDETERMINADO) etapa_inicial = LILY_MAIN_PREPROCESADO;
        }
        else if (!strcmp(extension, "com")) {
            etapa_final = LILY_MAIN_ENLAZADO;
            if (etapa_inicial == LILY_MAIN_INDETERMINADO) etapa_inicial = LILY_MAIN_ENLAZADO;
        }
    }
    /// Consecuente->destino
    if (etapa_inicial != LILY_MAIN_INDETERMINADO &&
        etapa_final == LILY_MAIN_INDETERMINADO &&
        archivo_salida == NULL) {
        if (etapa_inicial == LILY_MAIN_PREPROCESADO) etapa_final = LILY_MAIN_ENSAMBLADO;
        else if (etapa_inicial == LILY_MAIN_ENSAMBLADO) etapa_final = LILY_MAIN_ENSAMBLADO;
        else if (etapa_inicial == LILY_MAIN_ENLAZADO) etapa_final = LILY_MAIN_ENLAZADO;
        else if (etapa_inicial == LILY_MAIN_EJECUCION) etapa_final = LILY_MAIN_EJECUCION;
    }

    // En este punto, ya debemos saber qué haremos
    if (etapa_inicial == LILY_MAIN_INDETERMINADO) {
        log_fatal(&log_cfg, _("Initial stage cannot be determined."));
        exit(EXIT_FAILURE);
    }
    snprintf(msg_err, MSG_BUFFER, _("Initial stage set to %s."), obt_etapa_str(etapa_inicial));
    log_info(&log_cfg, msg_err);
    if (etapa_final < LILY_MAIN_ENSAMBLADO || etapa_final < etapa_inicial) {
        log_fatal(&log_cfg, _("Final stage cannot be determined."));
        exit(EXIT_FAILURE);
    }
    snprintf(msg_err, MSG_BUFFER, _("Final stage set to %s."), obt_etapa_str(etapa_final));
    log_info(&log_cfg, msg_err);

    // Determinar nombre de archivo destino, si no existe
    if (archivo_salida == NULL) {
        archivo_salida = (char*) calloc(strlen(archivo_entrada)+4, 1);
        if (archivo_salida == NULL) {
            log_fatal(&log_cfg, _("Error while determining output filename."));
            exit(EXIT_FAILURE);
        }
        obt_nombre_archivo(archivo_entrada, etapa_final, archivo_salida);
    }
    snprintf(msg_err, MSG_BUFFER, _("Output file: '%s'."), archivo_salida);
    log_info(&log_cfg, msg_err);

    // Empezamos análisis

    /// Análisis léxico
    struct lily_lde_lde* simbolos = lily_lde_create();
    struct lily_lex_error_ctx ctx;
    int codigo = lily_lex_lexico(archivo_entrada_p, simbolos, &ctx);
    snprintf(msg_err, MSG_BUFFER, _("lily_lex_lexico: %d."), codigo);
    log_info(&log_cfg, msg_err);
    if (codigo != COD_OK) {
        char caracter_prob = archivo_entrada_p[ctx.i_desp];
        snprintf(msg_err, MSG_BUFFER, _("type=%d, initial_i=%u, offset_i=%u (0x%x \"%c\")."), ctx.tipo, ctx.i_inicial, ctx.i_desp, caracter_prob, isprint(caracter_prob)?caracter_prob:'?');
        log_info(&log_cfg, msg_err);
    }

    munmap(archivo_entrada_p, archivo_entrada_st.st_size);
    close(archivo_entrada_fd);

    //struct lily_lde_lde* ast = lily_lde_create();
    //codigo = z80_sintactico(simbolos, ast);
    //if (codigo) return codigo;

    //struct lily_lde_lde* objeto = lily_lde_create();
    //codigo = z80_semantico(ast, objeto);
    //if (codigo) return codigo;

    return codigo;
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

char* obt_extension(const char *nombre) {
    char* punto = strrchr(nombre, '.');
    if (punto == NULL || punto == nombre) return nombre + strlen(nombre);
    return punto+1;
}

void obt_nombre_archivo(char* nombre, enum lily_main_etapa etapa, char* archivo_salida) {
    char* punto = strrchr(nombre, '.');
    ptrdiff_t nombre_tam;
    if (punto == NULL || punto == nombre) nombre_tam = (ptrdiff_t) strlen(nombre);
    else nombre_tam = punto - nombre;
    // Copiar nombre base
    memcpy(archivo_salida, nombre, nombre_tam);
    // Colocar extensión
    if (etapa == LILY_MAIN_PREPROCESADO) strcpy(archivo_salida+nombre_tam, ".s");
    else if (etapa == LILY_MAIN_ENSAMBLADO) strcpy(archivo_salida+nombre_tam, ".o");
    else if (etapa == LILY_MAIN_ENLAZADO) strcpy(archivo_salida+nombre_tam, "");
    else if (etapa == LILY_MAIN_DESENSAMBLADO) strcpy(archivo_salida+nombre_tam, ".s");
    return;
}

char* obt_etapa_str(enum lily_main_etapa etapa) {
    switch (etapa) {
    case LILY_MAIN_PREPROCESADO:
        return _("preprocessing");
    case LILY_MAIN_ENSAMBLADO:
        return _("assembly");
    case LILY_MAIN_ENLAZADO:
        return _("linking");
    case LILY_MAIN_DESENSAMBLADO:
        return _("disassembly");
    case LILY_MAIN_EJECUCION:
        return _("execution");
    }
    return "";
}
