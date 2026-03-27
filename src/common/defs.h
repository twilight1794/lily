/*
 * @file defs.h
 * Definiciones misceláneas
 */

#ifndef LILY_C_DEFS
#define LILY_C_DEFS

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @def breakpoint
 * @brief Instrucción para levantar una excepción que pueda ser capturada por gdb
 * From http://web.archive.org/web/20230605101122/http://hg.mozilla.org/mozilla-central/file/98fa9c0cff7a/js/src/jsutil.cpp#l66
 * @note Para continuar desde OS X, "return" y después "continue".
 * @note Para continuar desde Linux, "signal 0".
 */

#if defined(WIN32)
#define breakpoint *((int *) NULL) = 0

#elif defined(__APPLE__)
#include <signal.h>
#define breakpoint                                                             \
  {                                                                            \
    *((int *)NULL) = 0;                                                        \
    raise(SIGTRAP);                                                            \
  }

#else
#include <signal.h>
#define breakpoint raise(SIGTRAP)
#endif

/**
 * printf con memoria dinámica
 * @param formato Formato printf
 * @return Cadena formateada
 */
char* d_printf(char* formato, ...);

#endif
