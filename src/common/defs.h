/*
 * @file defs.h
 * Definiciones comunes
 */

#ifndef LILY_C_DEFS
#define LILY_C_DEFS

/**
 * @def breakpoint
 * @brief Instrucción para levantar una excepción que pueda ser capturada por gdb
 * From http://web.archive.org/web/20230605101122/http://hg.mozilla.org/mozilla-central/file/98fa9c0cff7a/js/src/jsutil.cpp#l66
 * @note Para continuar desde OS X, "return" y después "continue".
 * @note Para continuar desde Linux, "signal 0".
 */

#if defined(WIN32)
char* _(char *cad) { return cad; }
#define breakpoint *((int *) NULL) = 0

#elif defined(__APPLE__)
#include <signal.h>
#include <libintl.h>
#define _(STRING) gettext(STRING)
#define breakpoint                                                             \
  {                                                                            \
    *((int *)NULL) = 0;                                                        \
    raise(SIGABRT);                                                            \
  }

#else
#include <signal.h>
#include <libintl.h>
#define _(STRING) gettext(STRING)
#define breakpoint raise(SIGABRT)
#endif

#endif
#define _(STRING) gettext(STRING)
