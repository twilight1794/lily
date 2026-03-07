#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <getopt.h>
#include <unistd.h>

#include <libintl.h>

#include "mmap.h"
#include "../common/defs.h"
#include "../common/dict.h"
#include "../common/estado.h"
#include "../common/lde.h"
#include "../common/log.h"
#include "../lib/a_lexico.h"
#include "../lib/a_sintactico.h"
#include "../lib/a_semantico.h"
#include "../lib/lua_cpu.h"

#ifndef LILY_VERSION
#define LILY_VERSION "???"
#endif
#ifndef LILY_COMMIT
#define LILY_COMMIT "???"
#endif
#ifndef LILY_MODIFICADO
#define LILY_MODIFICADO ""
#endif

enum lily_main_estricto {
    LILY_MAIN_RELAJADO,
    LILY_MAIN_ESTRICTO,
    LILY_MAIN_SUPERESTRICTO,
};

enum lily_main_etapa {
    LILY_MAIN_INDETERMINADO,
    LILY_MAIN_ENSAMBLADO,
    LILY_MAIN_ENLAZADO,
    LILY_MAIN_DESENSAMBLADO,
    LILY_MAIN_EJECUCION,
};

struct lily_log_config lily_log_conf = {
    .colores = true,
    .incluir_fecha = true,
    .incluir_hora = true,
    .incluir_archivo = true,
    .nivel_minimo = LILY_LOG_INFO
};
bool lily_log_msg_null;
char* lily_log_msg_buffer;

void f_help(char* name);
void f_version(void);
struct lily_dict_nodo* obt_param_valorado(char* arg, struct lily_dict_dict* dict);
char* obt_extension(char *nombre);
void obt_nombre_archivo(char* nombre, enum lily_main_etapa etapa, char* archivo_salida);
char* obt_etapa_str(enum lily_main_etapa etapa);

int main(int argc, char **argv){
    // Si no hay parámetros, no hay qué seguir
    if (argc == 1) {
        f_help(argv[0]);
        return 0;
    }
    struct lily_ctx ctx = {
        .codigo = COD_OK,
        .tipo = SIMB_INDETERMINADO,
        .i_inicial = 0,
        .i_desp = 0,
        .ultimo = NULL,
        .lua_msg = NULL
    };

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

    // Análisis de parámetros
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
                    log_fatal_gen(_("Error while adding macro \"%s\"."), optarg);
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
                    log_fatal_gen(_("Error while adding %s \"%s\"."), (c=='W'?"warning":"error") SEP optarg);
                    return COD_MALLOC_FALLO;
                }
                break;
            case 'O':
                opt_nodo = obt_param_valorado(optarg, &opciones);
                if (opt_nodo == NULL) {
                    log_fatal_gen(_("Error while adding option \"%s\"."), optarg);
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
                if (!strcmp(optarg, "a")) {
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
                    log_fatal_gen(_("The value \"%s\" for parameter stage was not recognized"), optarg);
                    exit(EXIT_FAILURE);
                }
                break;
            case 'l':
                if (!strcmp(optarg, "debug")) lily_log_conf.nivel_minimo = LILY_LOG_DEBUG;
                else if (!strcmp(optarg, "info")) lily_log_conf.nivel_minimo = LILY_LOG_INFO;
                else if (!strcmp(optarg, "warn")) lily_log_conf.nivel_minimo = LILY_LOG_WARN;
                else if (!strcmp(optarg, "error")) lily_log_conf.nivel_minimo = LILY_LOG_ERROR;
                else if (!strcmp(optarg, "fatal")) lily_log_conf.nivel_minimo = LILY_LOG_FATAL;
                else if (!strcmp(optarg, "none")) lily_log_conf.nivel_minimo = LILY_LOG_NONE;
                else {
                    log_error_gen(_("The value \"%s\" for parameter logging was not recognized"), optarg);
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
                    lily_log_gen_msg(_("The parameter '%c' was not recognized."), optopt);
                } else {
                    lily_log_gen_msg(_("The parameter '%s' was not recognized."), argv[longopt_idx+1]);
                }
                log_error(lily_log_msg_buffer);
                free(lily_log_msg_buffer);
        }
    };

    // Listas de archivos
    // FIX: Por ahora, un solo archivo
    if (optind == argc){
        log_fatal(_("An input file was not specified."));
        exit(EXIT_FAILURE);
    } else archivo_entrada = argv[optind++];

    // Abrimos archivo
    struct lily_cli_archivo* archivo_entrada_obj = lily_cli_archivo_create(archivo_entrada, 0);
    if (archivo_entrada_obj == NULL) {
        log_fatal_gen(_("File %s cannot be open."), archivo_entrada);
        puts(strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Determinar operación inicial y final
    /// Parámetro origen->origen, destino, arriba en la lectura de argumentos
    /// Extensión origen->origen
    if (etapa_inicial == LILY_MAIN_INDETERMINADO) {
        char* extension = obt_extension(archivo_entrada);
        if (!strcmp(extension, "asm") || !strcmp(extension, "s")) etapa_inicial = LILY_MAIN_ENSAMBLADO;
        else if (!strcmp(extension, "o")) etapa_inicial = LILY_MAIN_ENLAZADO;
        else if (!strcmp(extension, "com")) etapa_inicial = LILY_MAIN_EJECUCION;
    }
    /// Extensión destino->destino, origen si no determinado
    if (etapa_final == LILY_MAIN_INDETERMINADO && archivo_salida != NULL) {
        char* extension = obt_extension(archivo_salida);
        if (!strcmp(extension, "s")) {
            etapa_final = LILY_MAIN_DESENSAMBLADO;
            if (etapa_inicial == LILY_MAIN_INDETERMINADO) etapa_inicial = LILY_MAIN_DESENSAMBLADO;
        }
        else if (!strcmp(extension, "o")) {
            etapa_final = LILY_MAIN_ENSAMBLADO;
            if (etapa_inicial == LILY_MAIN_INDETERMINADO) etapa_inicial = LILY_MAIN_ENSAMBLADO;
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
        if (etapa_inicial == LILY_MAIN_ENSAMBLADO) etapa_final = LILY_MAIN_ENSAMBLADO;
        else if (etapa_inicial == LILY_MAIN_ENLAZADO) etapa_final = LILY_MAIN_ENLAZADO;
        else if (etapa_inicial == LILY_MAIN_EJECUCION) etapa_final = LILY_MAIN_EJECUCION;
        else if (etapa_inicial == LILY_MAIN_DESENSAMBLADO) etapa_final = LILY_MAIN_DESENSAMBLADO;
    }

    // En este punto, ya debemos saber qué haremos
    if (etapa_inicial == LILY_MAIN_INDETERMINADO) {
        log_fatal(_("Initial stage cannot be determined."));
        exit(EXIT_FAILURE);
    }
    log_info_gen(_("Initial stage set to %s."), obt_etapa_str(etapa_inicial));
    if (etapa_final < LILY_MAIN_ENSAMBLADO || etapa_final < etapa_inicial) {
        log_fatal(_("Final stage cannot be determined."));
        exit(EXIT_FAILURE);
    }
    log_info_gen(_("Final stage set to %s."), obt_etapa_str(etapa_final));

    // Determinar nombre de archivo destino, si no existe
    if (archivo_salida == NULL) {
        archivo_salida = (char*) calloc(strlen(archivo_entrada)+4, 1);
        if (archivo_salida == NULL) {
            log_fatal(_("Error while determining output filename."));
            exit(EXIT_FAILURE);
        }
        obt_nombre_archivo(archivo_entrada, etapa_final, archivo_salida);
    }
    log_info_gen(_("Output file: '%s'."), archivo_salida);

    // Análisis léxico
    struct lily_lde_lde* simbolos = lily_a_lexico(archivo_entrada_obj->p, &ctx);
    log_info_gen(_("lily_a_lexico: %d."), ctx.codigo);
    if (ctx.codigo != COD_OK) {
        char caracter_prob = archivo_entrada_obj->p[ctx.i_desp];
        log_fatal_gen(_("type=%d, initial_i=%lu, offset_i=%lu (0x%x \"%c\")."), ctx.tipo SEP ctx.i_inicial SEP ctx.i_desp SEP caracter_prob SEP isprint(caracter_prob)?caracter_prob:'?');
        exit(EXIT_FAILURE);
    }
    /*for (size_t i = 0; i < lily_lde_size(simbolos); i++) {
        char* simb_cad = lily_a_lexico_simbolo_print(lily_lde_get(simbolos,i)->valor);
        log_debug_gen("a_lexico rest [%p]: %s", lily_lde_get(simbolos,i)->valor SEP simb_cad);
        free(simb_cad);
    }*/
    lily_cli_archivo_close(archivo_entrada_obj); // Ya no necesitamos más el archivo

    // Análisis sintáctico
    struct lily_lde_lde* ast = lily_a_sintactico(simbolos, &ctx);
    log_info_gen(_("lily_a_sintactico: %d."), ctx.codigo);
    if (ctx.codigo != COD_OK) {
        log_fatal_gen(_("codigo=%d, %d.%d (%lu)"), ctx.codigo SEP ctx.ultimo->tipo SEP ctx.ultimo->subtipo SEP ctx.ultimo->linea);
        exit(EXIT_FAILURE);
    }

    // Determinar arquitectura a usar
    char* arquitectura_final = NULL;
    char* arquitectura_asm = lily_a_semantico_obt_arquitectura_declarada(ast, &ctx);
    if (arquitectura != NULL) {
        arquitectura_final = arquitectura;
        if (strcmp((arquitectura_asm == NULL)?"":arquitectura_asm, arquitectura)) {
            log_warn_gen(_("The architecture specified at parameter '%s' is different to architecture specified at file '%s'."), arquitectura SEP arquitectura_asm);
        }
    } else if (arquitectura_asm != NULL)
        arquitectura_final = arquitectura_asm;
    else {
        log_fatal(_("An architecture was not specified."));
        exit(EXIT_FAILURE);
    }
    log_info_gen(_("The architecture selected is '%s'"), arquitectura_final);
    /// Generar ruta para archivo de arquitectura
    int archivo_arquitectura_fd = open(arquitectura_final, O_RDONLY);
    if (archivo_arquitectura_fd == -1) {
        char* archivo_arquitectura_ruta = lily_cadena_create();
        if (archivo_arquitectura_ruta == NULL) {
            log_fatal(_("Error while searching for architecture description file."));
            exit(EXIT_FAILURE);
        }
        char* tmp = lily_cadena_concat(archivo_arquitectura_ruta, "misc/cpu/"); // FIX: cambiar por constante definida en Makefile
        if (tmp == NULL) {
            log_fatal(_("Error while searching for architecture description file."));
            exit(EXIT_FAILURE);
        }
        archivo_arquitectura_ruta = tmp;
        tmp = lily_cadena_concat(archivo_arquitectura_ruta, arquitectura_final);
        if (tmp == NULL) {
            log_fatal(_("Error while searching for architecture description file."));
            exit(EXIT_FAILURE);
        }
        archivo_arquitectura_ruta = tmp;
        tmp = lily_cadena_concat(archivo_arquitectura_ruta, ".lua");
        if (tmp == NULL) {
            log_fatal(_("Error while searching for architecture description file."));
            exit(EXIT_FAILURE);
        }
        archivo_arquitectura_ruta = tmp;
        archivo_arquitectura_fd = open(archivo_arquitectura_ruta, O_RDONLY);
        if (archivo_arquitectura_fd == -1) {
            log_fatal_gen(_("The architecture '%s' could not be found."), arquitectura_final);
            exit(EXIT_FAILURE);
        }
    }
    /// Abrir ruta generada
    struct stat archivo_arquitectura_st;
    fstat(archivo_arquitectura_fd, &archivo_arquitectura_st);
    char* archivo_arquitectura_p = (char*) mmap(NULL, archivo_arquitectura_st.st_size, PROT_READ, MAP_SHARED, archivo_arquitectura_fd, 0);
    lua_State* L = lily_lua_cpu_cargar(archivo_arquitectura_p, &ctx);
    log_info_gen(_("lily_lua_cpu_cargar: %d."), ctx.codigo);
    if (ctx.codigo != COD_OK) {
        log_fatal_gen(_("codigo=%d (%s)"), ctx.codigo SEP ctx.lua_msg);
        exit(EXIT_FAILURE);
    }
    munmap(archivo_arquitectura_p, archivo_arquitectura_st.st_size);
    close(archivo_arquitectura_fd);

    // Análisis semántico
    size_t tam_bytes;
    uint8_t* bytes = lily_a_semantico(ast, L, 0, &tam_bytes, &ctx);
    log_info_gen(_("lily_a_semantico: %d (%ld bytes)."), ctx.codigo SEP tam_bytes);
    if (ctx.codigo != COD_OK) {
        log_fatal_gen(_("codigo=%d, %s, Lua: '%s')"), ctx.codigo SEP lily_simbolo_simbolo_print(ctx.ultimo) SEP ctx.lua_msg);
        exit(EXIT_FAILURE);
    }

    // Salida
    struct lily_cli_archivo* archivo_salida_obj = lily_cli_archivo_create(archivo_salida, tam_bytes);
    if (archivo_salida_obj == NULL) {
        log_fatal_gen(_("File %s cannot be open: %s."), archivo_salida SEP strerror(errno));
        exit(EXIT_FAILURE);
    }
    memcpy(archivo_salida_obj->p, bytes, tam_bytes);
    lily_cli_archivo_close(archivo_salida_obj);
    exit(EXIT_SUCCESS);
}

void f_help(char* name) {
    printf(_("Usage: %s [<params>] <file>\n\n"), name);
    puts(_("Key: A (assembly), L (linking), D (disassembly), E (execution)"));
    /* Opciones de ensamble */
    puts(_("\nAssembly options:"));
    puts(_(" -L<path>           --listing=<path>          Generate listing file in <path>"));
    puts(_(" -D<tag>[=<value>]  --define=<tag>[=<value>]  Define a tag <tag> with a value <value>"));
    puts(_(" -I<path>           --include=<path>          Search for assembly files first in <path>"));
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
    puts(_(" -s<stage>   --stage=<stage>    Operation to perform: A, L, D, or E"));
    puts(_(" -l=<level>  --logging=<level>  Restrict output to <level>"));
    puts(_("                                <level> can be \"debug\", \"info\", \"warn\", \"error\", \"fatal\" or \"none\""));
    puts(_(" -h          --help             Show this help")); /* TODO: NOTE=class[,…]*/
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

char* obt_extension(char *nombre) {
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
    if (etapa == LILY_MAIN_ENSAMBLADO) strcpy(archivo_salida+nombre_tam, ".o");
    else if (etapa == LILY_MAIN_ENLAZADO) strcpy(archivo_salida+nombre_tam, "");
    else if (etapa == LILY_MAIN_DESENSAMBLADO) strcpy(archivo_salida+nombre_tam, ".s");
}

char* obt_etapa_str(enum lily_main_etapa etapa) {
    switch (etapa) {
    case LILY_MAIN_INDETERMINADO:
        return _("undefined");
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

char* obt_archivo(const char* archivo) {}