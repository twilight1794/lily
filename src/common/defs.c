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

#include "defs.h"

char* d_printf(char* formato, ...) {
    va_list args;
    va_start(args, formato);
    // FIX: encontrar la forma de obtener el tamaño exacto con solo una llamada a vsnprintf
    char* buffer = (char*) malloc(255);
    vsnprintf(buffer, 255, formato, args);
    va_end(args);
    return buffer;
}
