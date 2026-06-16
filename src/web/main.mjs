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
 * Interfaz principal para manipular máquinas virtuales Lily.
 * @module liblily
 */

import Module from "./liblily.mjs";
import { LilyError } from "./Error.mjs";
import { Mensaje, MensajeLog, MensajeAdvertencia, MensajeEtiqueta, MensajeVariable, MensajeMemoria, MensajeRegistro, MensajePila, MensajeDispositivo, MensajeInterrupcion, TipoMensajeError, SubtipoMensajeError } from "./Mensaje.mjs"

const M = await Module();

// Funciones auxiliares

/**
 * Evalúa si una variable es un número entero.
 * @function
 * @param {number} variable Variable a analizar.
 * @returns {boolean} true si sí almacena un número entero.
 */
function esEntero(variable) { return typeof(variable) == "number" && Number.isInteger(variable); }

class MaquinaApagadaError extends LilyError {
    constructor() {
        super("No hay una máquina virtual encendida.");
    }
}

class CargarArchivoFaltanteError extends LilyError {
    constructor() {
        super("Debe establecerse la función de carga de archivo.");
    }
}

class CerrarArchivoFaltanteError extends LilyError {
    constructor() {
        super("Debe establecerse la función de cierre de archivo.");
    }
}

class EjecutoraFaltanteError extends LilyError {
    constructor() {
        super("Debe establecerse la función ejecutora.");
    }
}

class MensajeFaltanteError extends LilyError {
    constructor() {
        super("Debe establecerse la función de mensajes.");
    }
}

class ArquitecturaFaltanteError extends LilyError {
    constructor() {
        super("Debe especificar el identificador de un esquema de procesador.");
    }
}

class NoImplementadoError extends LilyError {
    constructor() {
        super("Esta funcionalidad aún no ha sido implementada.");
    }
}

class IdentificadorArchivoNoEnteroError extends LilyError {
    constructor() {
        super("El parámetro 'identificador' para Archivo debe ser un entero.");
    }
}

class ContenidoArchivoNoCadenaError extends LilyError {
    constructor() {
        super("El parámetro 'contenido' para Archivo debe ser una cadena.");
    }
}

class TipoArchivoNoEnteroError extends LilyError {
    constructor() {
        super("El parámetro 'tipo' para Archivo debe ser un entero.");
    }
}

class NoEsPunteroError extends LilyError {
    constructor() {
        super("El valor pasado no es un puntero.");
    }
}

// Funciones para exportar

/**
 * Estructura para describir un archivo que será manipulado por Lily.
 * @class
 * @param {number} Identificador único para un archivo en el sistema cliente.
 * @param {string} Contenido del archivo consultado.
 * @param {number} Tipo de archivo.
 */
class Archivo {
    static T_ESQUEMA_PROCESADOR = 0;
    static T_ARCHIVO_REGULAR = 1;

    static desde_puntero(ptr) {
        this.ptr = ptr;
    }

    constructor(identificador, contenido, tipo) {
        // Comprobar identificador
        if (!esEntero(identificador))
            throw new IdentificadorArchivoNoEnteroError();

        // Comprobar contenido
        if (typeof(contenido) != "string")
            throw new ContenidoArchivoNoCadenaError();
        const tam_contenido = M.lengthBytesUTF8(contenido) + 1;
        const ptr_contenido = M._malloc(tam_contenido);
        M.stringToUTF8(contenido, ptr_contenido, tam_contenido);

        // Comprobar tipo
        if (!esEntero(tipo) || [this.T_ESQUEMA_PROCESADOR, this.T_ARCHIVO_REGULAR].includes(tipo))
            throw new TipoArchivoNoEnteroError();

        // Crear estructura
        this.ptr = M._malloc(12);
        M.setValue(this.ptr, identificador, "i32");   // void* obj;
        M.setValue(this.ptr+4, ptr_contenido, "i32"); // char* archivo;
        M.setValue(this.ptr+8, tipo, "i32");          // int tipo;
    }

    destructor() {
        const ptr_archivo = M.getValue(this.ptr + 4, "i32");
        M._free(ptr_archivo); // char* archivo
        M._free(this.ptr);    // struct lily_archivo
    }
}

/**
 * Función para acceder a un archivo en el sistema cliente.
 * @callback Maquina~carga_archivo_f
 * @param {string} x Nombre del archivo a abrir.
 * @param {number} x Tipo de archivo a abrir.
 * @return {Archivo} Objeto que representa un archivo abierto.
 */

/**
 * Función para cerrar un archivo previamente abierto con {{@link Maquina~carga_archivo}}.
 * @callback Maquina~cierre_archivo_f
 * @param {Archivo} x Objeto archivo a cerrar.
 * @return {number} Código de estado. 0 si todo salió bien.
 */

/**
 * Función para procesar un mensaje de Lily.
 * @callback Maquina~envio_mensaje_f
 * @param {number} tipo Tipo del mensaje a recibir.
 * @param {number} subtipo Tipo más específico del mensaje.
 * @param {string} modulo Módulo del cual proviene el mensaje.
 * @param {object} detalles Objeto que contiene datos adicionales sobre el mensaje.
 * @return {number} Código de estado. 0 si todo salió bien.
 */

/**
 * Instancia de una máquina virtual.
 * @class
 * @param {Maquina~carga_archivo_f} cargar_archivo Función llamada por Lily para pedir al sistema cliente un archivo nuevo.
 * @param {Maquina~cierre_archivo_f} cerrar_archivo Función llamada por Lily para cerrar un archivo.
 * @param {Maquina~envio_mensaje_f} mensaje Función llamada por Lily para procesar mensajes.
 */
class Maquina {
    constructor(cargar_archivo, cerrar_archivo, enviar_mensaje) {
        // Cargar archivo
        if (typeof(cargar_archivo) != "function")
            throw new CargarArchivoFaltanteError();
        //this.cargar_archivo = cargar_archivo;
        this.p_fun_cargar_archivo = M.addFunction((param_nombre, param_tipo, param_estado) => {
            const nombre = M.UTF8ToString(param_nombre);
            const [obj, estado] = cargar_archivo(nombre, param_tipo);
            M.setValue(param_estado, estado, "i32");
            return obj.ptr;
        }, "iiii");

        // Cerrar archivo
        if (typeof(cerrar_archivo) != "function")
            throw new CerrarArchivoFaltanteError();
        this.p_fun_cerrar_archivo = M.addFunction((param_archivo) => {
            const archivo = Archivo.desde_puntero(param_archivo);
            const estado = cerrar_archivo(archivo);
            //archivo.destructor();
            return estado;
        }, "ii");

        // Enviar mensaje
        if (typeof(enviar_mensaje) != "function")
            throw new MensajeFaltanteError();
        this.p_fun_enviar_mensaje = M.addFunction((param_tipo, param_subtipo, param_modulo, param_obj) => {
            const modulo = M.UTF8ToString(param_modulo);
            // Decidir mensaje
            let mensaje = new (Mensaje.obt_clase(param_tipo))(M, param_modulo, param_subtipo, param_obj);
            const estado = enviar_mensaje(mensaje);
            return estado;
        }, "iiiii");
    }

    /**
     * Ensambla un bloque de código en ensamblador.
     * @param {string} datos_entrada Código fuente a ensamblar.
     * @param {string} arquitectura Identificador del esquema del microprocesador a utilizar.
     * @return {Uint8Array} Código objeto resultante.
     */
    ensamblar(datos_entrada, arquitectura) {
        let array_salida;

        // const char* datos_entrada
        const tam_datos_entrada = M.lengthBytesUTF8(datos_entrada) + 1;
        this.p_datos_entrada = M._malloc(tam_datos_entrada);
        M.stringToUTF8(datos_entrada, this.p_datos_entrada, tam_datos_entrada);

        // char* arquitectura
        this.p_arquitectura = 0;
        if (arquitectura) {
            const tam_arquitectura = M.lengthBytesUTF8(arquitectura) + 1;
            this.p_arquitectura = M._malloc(tam_datos_entrada);
            M.stringToUTF8(arquitectura, this.p_arquitectura, tam_arquitectura);
        }

        // struct lily_dict_dict* opciones
        this.p_opciones = M._malloc(8);
        M.setValue(this.p_opciones, 0, "i32");
        M.setValue(this.p_opciones + 4, 0, "i32");

        // size_t* tam_salida
        this.p_tam_salida = M._malloc(4);
        M.setValue(this.p_tam_salida, 0, "i32");

        // enum lily_estado* estado
        this.p_estado = M._malloc(4);
        M.setValue(this.p_estado, 0, "i32");

        // void** ctx
        this.p_ctx = M._malloc(4);
        this.pp_ctx = M._malloc(4);
        M.setValue(this.p_ctx, 0, "i32");
        M.setValue(this.pp_ctx, this.p_ctx, "i32");

        // Llamar a lily_lily_ensamble
        let estado;
        try {
            const p_res = M.ccall("lily_lily_ensamble",
                                       "number",
                                       ["number", "number", "number", "number", "number", "number", "number", "number", "number"],
                                       [this.p_datos_entrada, this.p_arquitectura, this.p_opciones, this.p_fun_cargar_archivo, this.p_fun_cerrar_archivo, this.p_fun_enviar_mensaje, this.p_tam_salida, this.p_estado, this.pp_ctx]
                                      );
            estado = M.getValue(this.p_estado, "i32");
            array_salida = new Uint8Array(M.getValue(this.p_tam_salida, "i32"));
            M._free(this.p_tam_salida);
            for (let i = 0; i < array_salida.byteLength; i++) {
                array_salida[i] = M.getValue(p_res + i, "i8");
            }
        }
        finally {
            M._free(this.p_datos_entrada);
            M._free(this.p_arquitectura);
            M._free(this.p_estado);
            M._free(this.p_ctx);
        }
        return [estado, array_salida];
    }

    /**
     * Desensambla un bloque de código objeto.
     * @param {Uint8Array} datos_entrada Código objeto a desensamblar.
     * @param {string} arquitectura Identificador del esquema del microprocesador a utilizar.
     * @return {string} Código ensamblador resultante.
     */
    desensamblar(datos_entrada, arquitectura) {
        if (typeof(arquitectura) != "string")
            throw new ArquitecturaFaltanteError();
    }

    /**
     * Crea una máquina virtual con un programa precargado
     * @param {Uint8Array} datos_entrada Código objeto a ejecutar.
     * @param {string} arquitectura Identificador del esquema del microprocesador a utilizar.
     */
    iniciar(datos_entrada, arquitectura) {
        // size_t tamano
        let tam_datos_entrada_iniciar = datos_entrada.length; // FIX: ver el tamaño de esto

        // char* bytes
        this.p_datos_entrada_iniciar = M._malloc(tam_datos_entrada_iniciar);
        M.writeArrayToMemory(datos_entrada, this.p_datos_entrada_iniciar);

        // struct lily_lua_ejecucion_ctx* ctx
        this.p_ejecucion_ctx = M._malloc(24);
        M.setValue(this.p_ejecucion_ctx, this.p_fun_cargar_archivo, "i32");
        M.setValue(this.p_ejecucion_ctx + 4, this.p_fun_cerrar_archivo, "i32");
        M.setValue(this.p_ejecucion_ctx + 8, this.p_fun_enviar_mensaje, "i32");
        /// const char* lua_msg
        M.setValue(this.p_ejecucion_ctx + 12, 0, "i32");
        /// enum lily_estado estado
        M.setValue(this.p_ejecucion_ctx + 16, 0, "i32");
        /// bool paso_a_paso
        M.setValue(this.p_ejecucion_ctx + 20, 0, "i32");

        // Llamar a lily_lily_creacion_maquina
        let estado;
        try {
            this.p_maquina = M.ccall("lily_lily_creacion_maquina",
                                  "number",
                                  ["number", "number", "string", "number"],
                                  [this.p_datos_entrada_iniciar, tam_datos_entrada_iniciar, arquitectura, this.p_ejecucion_ctx]
                                 );
            estado = M.getValue(this.p_ejecucion_ctx + 16, "i32");
        }
        finally {}
        return estado;
    }

    /**
     * Ejecuta una instrucción y retorna
     */
    ejecutar() {
        // Llamar a lily_lily_ejecucion
        let estado;
        try {
            M.ccall("lily_lily_ejecutar_instruccion",
                    null,
                    ["number", "number"],
                    [this.p_maquina, this.p_ejecucion_ctx]
                   );
            estado = M.getValue(this.p_ejecucion_ctx + 16, "i32");
        }
        finally {
            //-M._free(this.p_datos_entrada);
            //-M._free(this.p_estado);
            //-M._free(this.p_ctx);
        }
        return estado;
    }

    leer_memoria(direccion, anunciar=false) {
        let p_valor;
        let valor;
        try {
            p_valor = M._malloc(1);
            let res = M.ccall("lily_lua_ejecucion_leer_memoria",
                              "number",
                              ["number", "number", "number", "boolean"],
                              [this.p_maquina, BigInt(direccion), p_valor, anunciar]
            );
            if (res != 0) throw new Error();
            valor = M.getValue(p_valor, "i8");
        }
        catch (e) {
            if (e instanceof ReferenceError) throw new MaquinaApagadaError();
            throw e;
        }
        finally {
            M._free(p_valor);
        }
        return valor;
    }

    escribir_memoria(direccion, valor, anunciar=false) {
        let p_valor;
        let valor_anterior;
        try {
            p_valor = M._malloc(2);
            M.setValue(p_valor, valor, "i8");
            let res = M.ccall("lily_lua_ejecucion_escribir_memoria",
                              "number",
                              ["number", "number", "number", "number", "boolean"],
                              [this.p_maquina, BigInt(direccion), p_valor, p_valor + 1, anunciar]
            );
            if (res != 0) throw new Error();
            valor_anterior = M.getValue(p_valor + 1, "i8");
        }
        catch (e) {
            if (e instanceof ReferenceError) throw new MaquinaApagadaError();
            throw e;
        }
        finally {
            M._free(p_valor);
        }
        return valor_anterior;
    }

    leer_registro(registro, anunciar=false) {
        let p_valor;
        let valor;
        try {
            p_valor = M._malloc(8);
            let res = M.ccall("lily_lua_ejecucion_leer_registro",
                              "number",
                              ["number", "string", "number", "boolean"],
                              [this.p_maquina, registro, p_valor, anunciar]
            );
            if (res != 0) throw new Error();
            valor = M.getValue(p_valor, "i64");
        }
        catch (e) {
            if (e instanceof ReferenceError) throw new MaquinaApagadaError();
            throw e;
        }
        finally {
            M._free(p_valor);
        }
        return valor;
    }

    escribir_registro(registro, valor, anunciar=false) {
        let p_valor;
        let valor_anterior;
        try {
            p_valor = M._malloc(16);
            M.setValue(p_valor, valor, "i64");
            let res = M.ccall("lily_lua_ejecucion_escribir_registro",
                          "number",
                          ["number", "string", "number", "number", "boolean"],
                          [this.p_maquina, registro, p_valor, p_valor + 8, anunciar]
            );
            if (res != 0) throw new Error();
            valor_anterior = M.getValue(p_valor + 8, "i64");
        }
        catch (e) {
            if (e instanceof ReferenceError) throw new MaquinaApagadaError();
            throw e;
        }
        finally {
            M._free(p_valor);
        }
        return valor_anterior;
    }

    destructor() {
        M.removeFunction(this.p_fun_cargar_archivo);
        M.removeFunction(this.p_fun_cerrar_archivo);
        M.removeFunction(this.p_fun_mensaje);
    }
}

export {
    // Base
    Archivo,
    Maquina,
    // Error.mjs
    LilyError,
    // Mensaje.mjs
    MensajeLog,
    MensajeAdvertencia,
    MensajeEtiqueta,
    MensajeVariable,
    MensajeMemoria,
    MensajeRegistro,
    MensajePila,
    MensajeDispositivo,
    MensajeInterrupcion,
    TipoMensajeError,
    SubtipoMensajeError
};
