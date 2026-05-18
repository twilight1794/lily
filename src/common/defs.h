/*
 * Copyright (C) 2026 Giovanni Alfredo Garciliano Diaz
 *
 * This file is part of Lily.
 *
 * Lily is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Lily is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Lily. If not, see <https://www.gnu.org/licenses/>.
 */

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
