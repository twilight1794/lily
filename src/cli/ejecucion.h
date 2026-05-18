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
