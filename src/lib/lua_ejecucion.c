#include "lua_ejecucion.h"

struct lily_lua_ejecucion_maquina* lily_lua_ejecucion_ini(lua_State* L, f_mensajes_ptr enviar_mensaje, enum lily_estado* estado, void** ctx) {
    struct lily_lua_ejecucion_maquina* obj = (struct lily_lua_ejecucion_maquina*) malloc(sizeof(struct lily_lua_ejecucion_maquina));
    if (obj == NULL) {
      *estado = COD_MALLOC_FALLO;
      return NULL;
    }
    obj->L = L;
    // Obtener espacio para memoria
    lua_pushliteral(L, "max_memoria");
    lua_gettable(L, -2);
    // FIX: por ahora, el tamaño de la memoria no es ajustable
    obj->tamano_memoria = (uint64_t) lua_tointeger(L, -1);
    obj->memoria = (uint8_t*) malloc(obj->tamano_memoria);
    if (obj->memoria == NULL) {
        free(obj);
        *estado = COD_MALLOC_FALLO;
        return NULL;
    }
    lua_pop(L, 1);
    // Guardar para Lua la dirección del bloque de memoria
    lua_pushinteger(L, (lua_Integer) obj->memoria);
    lua_setglobal(L, "_lily_memoria_dir");
    // Obtener datos del registro contador de programa
    lua_pushstring(L, "registro_programa");
    lua_gettable(L, -2);
    const char* registro_programa = lua_tostring(L, -1);
    lua_pop(L, 1);
    lua_pushstring(L, "registros");
    lua_gettable(L, -2);
    lua_getfield(L, -1, registro_programa);
    /// Obtener tamaño de registro
    lua_pushstring(L, "tamano");
    lua_gettable(L, -2);
    obj->tamano_pc = lua_tointeger(L, -1);
    lua_pop(L, 1);
    /// Obtener desplazamiento de registro
    lua_pushstring(L, "desplazamiento");
    lua_gettable(L, -2);
    obj->posicion_pc = lua_tointeger(L, -1);
    lua_pop(L, 2);
    // Obtener espacio para registros
    obj->tamano_registros = 0;
    lua_pushnil(L);
    while (lua_next(L, -2) != 0) {
        // Obtener tamaño de registro
        lua_pushstring(L, "tamano");
        lua_gettable(L, -2);
        const size_t tamano = lua_tointeger(L, -1);
        lua_pop(L, 1);
        // Obtener desplazamiento de registro
        lua_pushstring(L, "desplazamiento");
        lua_gettable(L, -2);
        const size_t desplazamiento = lua_tointeger(L, -1);
        lua_pop(L, 1);
        // Suma
        const size_t total = desplazamiento + tamano;
        if (total > obj->tamano_registros)
            obj->tamano_registros = total;
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
    obj->registros = (uint8_t*) malloc(ceil(obj->tamano_registros/8));
    if (obj->registros == NULL) {
        free(obj->memoria);
        free(obj);
        *estado = COD_MALLOC_FALLO;
        return NULL;
    }
    lua_pushinteger(L, (lua_Integer) obj->registros);
    lua_setglobal(L, "_lily_registros_dir");
    lua_pushinteger(L, obj->tamano_registros);
    lua_setglobal(L, "_lily_registros_max");
    return obj;
}

void lily_lua_ejecucion_arrancar(struct lily_lua_ejecucion_maquina* maquina, uint8_t* programa, size_t tamano) {
    // TODO: Por ahora, solo copiamos los datos de un lado a otro desde el inicio
    // Para otros sistemas, vendrá la carga de una BIOS
    memcpy((void*) maquina->memoria, (void*) programa, tamano);
    // FIX: informar sobre ese hecho
}

void lily_lua_ejecucion_ejecutar(struct lily_lua_ejecucion_maquina* maquina, f_mensajes_ptr enviar_mensaje, enum lily_estado* estado, void** ctx) {
    bool ejecutado = false;

    // Obtener PC
    union lily_simbolo_numero pc;
    lily_bitarray_obtener(maquina->posicion_pc, maquina->tamano_pc, maquina->registros, pc.bytes);

    int base_pila = lua_gettop(maquina->L); // Para limpiar después
    lua_pushstring(maquina->L, "opcodes");
    lua_gettable(maquina->L, -2);
    lua_len(maquina->L, -1);
    size_t tam_lista_casos = lua_tointeger(maquina->L, -1);
    lua_pop(maquina->L, 1);
    char* msg_buf = d_printf("Se encontraron %lu casos", tam_lista_casos);
    enviar_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "lua_ejecucion_ejecutar", msg_buf);
    free(msg_buf);
    //const size_t ptr_inicio = pc.positivo; // Inicio de la instrucción
    const size_t ptr_inicio = 0; // FIX: temporalmente, iniciamos siempre en 0
    size_t ptr_inicio_relativo = ptr_inicio;
    size_t i = 1;
    while (!ejecutado && i <= tam_lista_casos) {
        // Por cada elemento...
        lua_geti(maquina->L, -1, i++); // <- Caso
        lua_geti(maquina->L, -1, 1); // <- Lista de coincidencias

        lua_len(maquina->L, -1);
        const size_t tam_lista_bytes = lua_tointeger(maquina->L, -1);
        lua_pop(maquina->L, 1);
        size_t tam_cad_params = 0;
        for (size_t j = 1; j <= tam_lista_bytes; j++) {
            const int tipo = lua_geti(maquina->L, -1, j);
            if (tipo == LUA_TNUMBER) tam_cad_params += 6;
            else if (tipo == LUA_TFUNCTION) tam_cad_params += 10;
            lua_pop(maquina->L, 1);
        }
        tam_cad_params -= 1;

        // Generar mensaje de log
        char* msg_cad_params = (char*) malloc(tam_cad_params);
        if (msg_cad_params == NULL) {
            *estado = COD_MALLOC_FALLO;
            return;
        }
        for (size_t j = 1, k = 0; j <= tam_lista_bytes; j++) {
            const int tipo = lua_geti(maquina->L, -1, j);
            if (tipo == LUA_TNUMBER) {
                snprintf(msg_cad_params + k, 5, "0x%02llx", lua_tointeger(maquina->L, -1));
                k += 4;
            }
            else if (tipo == LUA_TFUNCTION) {
                memcpy(msg_cad_params + k, "function", 8);
                k += 8;
            }
            lua_pop(maquina->L, 1);
            if (j < tam_lista_bytes - 1) {
                memcpy(msg_cad_params + k, ", ", 2);
                k += 2;
            }
        }
        msg_buf = d_printf("Comprobando caso (%s)", msg_cad_params);
        enviar_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "lua_ejecucion_ejecutar", msg_buf);
        free(msg_buf);

        // Iterar por cada elemento de coincidencia
        bool coincide = true; // Esto queda así si toda la lista de coincidencias coincide
        for (size_t j = 1; j <= tam_lista_bytes; j++) {
            const uint8_t byte_actual = maquina->memoria[ptr_inicio_relativo + j - 1];
            msg_buf = d_printf("Comparando byte 0x%02lx en dirección 0x%02lx + %lu  ", byte_actual, ptr_inicio_relativo, j - 1);
            enviar_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "lua_ejecucion_ejecutar", msg_buf);
            free(msg_buf);
            const int tipo = lua_geti(maquina->L, -1, j);
            // Comparar
            if (tipo == LUA_TNUMBER) {
                const uint8_t val_num = (uint8_t) lua_tointeger(maquina->L, -1);
                lua_pop(maquina->L, 1);
                msg_buf = d_printf("Comparando con elemento de coincidencia 0x%02lx", val_num);
                enviar_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "lua_ejecucion_ejecutar", msg_buf);
                free(msg_buf);
                if (val_num != byte_actual) {
                    coincide = false;
                    break;
                }
            }
            else if (tipo == LUA_TFUNCTION) {
                lua_pushinteger(maquina->L, byte_actual);
                lua_pcall(maquina->L, 1, 1, 0);
                const bool val_fun = lua_toboolean(maquina->L, -1);
                lua_pop(maquina->L, 1);
                msg_buf = d_printf("Comparando con elemento de coincidencia (%ld) function", j);
                enviar_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "lua_ejecucion_ejecutar", msg_buf);
                free(msg_buf);
                if (!val_fun) {
                    coincide = false;
                    break;
                }
            }
        }
        lua_pop(maquina->L, 1); // Quitamos la lista de coincidencias

        if (coincide) {
            // Obtener resultado de coincidencia
            msg_buf = d_printf("Seleccionado caso (%s)", msg_cad_params);
            enviar_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "lua_ejecucion_ejecutar", msg_buf);
            free(msg_buf);
            const int tipo = lua_geti(maquina->L, -1, 2);
            if (tipo == LUA_TFUNCTION) {
                // ¡Valor de coincidencia terminal!: ejecutar y limpiar
                // Generar lista de bytes
                const size_t num_bytes = (ptr_inicio_relativo + tam_lista_bytes) - ptr_inicio;
                lua_createtable(maquina->L, num_bytes, 0);
                for (size_t j = 1; j <= num_bytes; j++) {
                    lua_pushinteger(maquina->L, maquina->memoria[ptr_inicio + j - 1]);
                    lua_seti(maquina->L, -2, j);
                }
                ejecutado = true;
                if (lua_pcall(maquina->L, 1, 0, 0)) {
                    // FIX: procesar error bien
                    enviar_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_FATAL, "lua_ejecucion_ejecutar", lua_tostring(maquina->L, -1));
                }
            }
            else if (tipo == LUA_TTABLE) {
                // Lista de casos: la cargamos y continuamos con ella
                ptr_inicio_relativo += tam_lista_bytes;
                lua_len(maquina->L, -1);
                tam_lista_casos = lua_tointeger(maquina->L, -1);
                lua_pop(maquina->L, 1);
                i = 1;
                msg_buf = d_printf("Lista de casos anidada: Se encontraron %lu casos", tam_lista_casos);
                enviar_mensaje(LILY_MENSAJE_TLOG, LILY_LOG_DEBUG, "lua_ejecucion_ejecutar", msg_buf);
            }
        }
        else {
            // La lista de coincidencias no coincidió, pasar a otro caso si lo hay
            lua_pop(maquina->L, 1);
        }
        free(msg_cad_params);
    }
    lua_settop(maquina->L, base_pila);

    if (!ejecutado) {
        *estado = COD_LUA_EJECUCION_INSTRUCCION_ILEGAL;
        // FIX: lista de bytes
    }
}
