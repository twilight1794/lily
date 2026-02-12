/*
 * @file lua_cpu.h
 * Funciones para cargar archivos de descripción de microprocesadores para Lily
 */
 
 #ifndef LILY_L_LUA_CPU
 #define LILY_L_LUA_CPU

#include "../common/defs.h"
#include "../common/error.h"

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

enum lily_error lily_lua_cpu_cargar(char* nombre);

#endif