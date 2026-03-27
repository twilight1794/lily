/*
 * @file lua_ensamble.h
 * Funciones para ensamblar según un esquema de descripción de microprocesador
 */

#ifndef LILY_L_LUA_ENSAMBLE
#define LILY_L_LUA_ENSAMBLE

#include <ctype.h>

#include "../common/cadena.h"
#include "../common/defs.h"
#include "../common/estado.h"
#include "../common/lde.h"
#include "../common/log.h"
#include "a_semantico.h"
#include "lua_int.h"
#include "simbolo.h"

#include <lua.h>

/**
 * Ensambla un mnemónico
 * @param L Sesión de Lua
 * @param instruccion Instrucción que contiene el mnemónico a ensamblar
 * @param [out] ctx Estado de la ejecución al momento de salir de la función
 */
void lily_lua_cpu_ensamblar(lua_State* L, struct lily_simbolo_instruccion* instruccion, f_mensajes_ptr enviar_mensaje, enum lily_estado* estado, void** ctx);

#endif
