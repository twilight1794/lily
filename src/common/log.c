#include "log.h"

void lily_log(enum lily_log_nivel tipo, const char* archivo, unsigned int linea, const char* msg) {

    // Fecha-hora
    time_t rawtime;
    time(&rawtime);
    struct tm *timeinfo = localtime(&rawtime);
    char dbuff[11];
    if (lily_log_conf.incluir_fecha) {
        strftime(dbuff, 11, "%F", timeinfo);
        printf("%s ", dbuff);
    }
    if (lily_log_conf.incluir_hora) {
        strftime(dbuff, 11, "%T", timeinfo);
        printf("%s ", dbuff);
    }

    // Tipo
    switch (tipo) {
    case LILY_LOG_DEBUG:
        if (lily_log_conf.colores) printf("\x1b[36;1mDEBUG\x1b[0m ");
        else printf("DEBUG ");
        break;
    case LILY_LOG_INFO:
        if (lily_log_conf.colores) printf("\x1b[32;1mINFO\x1b[0m  ");
        else printf("INFO  ");
        break;
    case LILY_LOG_WARN:
        if (lily_log_conf.colores) printf("\x1b[33;1mWARN\x1b[0m  ");
        else printf("WARN  ");
        break;
    case LILY_LOG_ERROR:
        if (lily_log_conf.colores) printf("\x1b[31;1mERROR\x1b[0m ");
        else printf("ERROR ");
        break;
    case LILY_LOG_FATAL:
        if (lily_log_conf.colores) printf("\x1b[35;1mFATAL\x1b[0m ");
        else printf("FATAL ");
        break;
    case LILY_LOG_NONE:
        break;
    }

    // Archivo-línea
    if (lily_log_conf.incluir_archivo && lily_log_conf.colores) printf("\x1b[90m%s:%d \x1b[0m%s\x1b[0m\n", archivo, linea, msg);
    else if (lily_log_conf.incluir_archivo) printf("%s:%d: %s\n", archivo, linea, msg);
    else printf("%s\n", msg);
}