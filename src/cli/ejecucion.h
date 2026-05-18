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

/**
 * @file ejecucion.h
 * Funciones para dirigir la etapa de ejecución
 */

#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <termios.h>
#include <unistd.h>

#include "../common/bitarray.h"
#include "../common/estado.h"
#include "../lib/mensajes.h"
#include "../lib/lua_ejecucion.h"

#include <libintl.h>

#define _(CAD) gettext(CAD)

int f_ejecutora_interactiva(struct lily_lua_ejecucion_maquina* maquina, int* contador, f_mensajes_ptr enviar_mensaje, enum lily_estado* estado, void** ctx);

int f_ejecutora_desatendida(struct lily_lua_ejecucion_maquina* maquina, int* contador, f_mensajes_ptr enviar_mensaje, enum lily_estado* estado, void** ctx);
