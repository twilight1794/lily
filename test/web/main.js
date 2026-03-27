"use strict";
/**
 * Interfaz principal para manipular máquinas virtuales Lily.
 * @module liblily
 */

import Module from './liblily.mjs';
const M = await Module();
window.modulito = M;

// Funciones auxiliares

/**
 * Evalúa si una variable es un número entero.
 * @function
 * @param {number} variable Variable a analizar.
 * @returns {boolean} true si sí almacena un número entero.
 */
function esEntero(variable) { return typeof(variable) == "number" && Number.isInteger(variable); }

// Errores

class LilyError extends Error {
    constructor(mensaje) {
        super(mensaje);
        this.name = this.constructor.name;
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

class TipoMensajeError extends LilyError {
    constructor() {
        super("El tipo de mensaje debe estar entre 0 y 8");
    }
}

class SubtipoMensajeError extends LilyError {
    constructor() {
        super("El subtipo de mensaje debe ser un valor entero");
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

class Mensaje {
    static T_LOG = 0;
    static T_ADVERTENCIA = 1;
    static T_ETIQUETA = 2;
    static T_VARIABLE = 3;
    static T_MEMORIA = 4;
    static T_REGISTRO = 5;
    static T_PILA = 6;
    static T_DISPOSITIVO = 7;
    static T_INTERRUPCION = 8;

    constructor(tipo, subtipo, modulo, obj) {
        if (!esEntero(tipo) || tipo < 0 || tipo > 8)
            throw new TipoMensajeError();
        this.tipo = tipo
        if (!esEntero(subtipo)) {
            throw new SubtipoMensajeError();
        }
        this.subtipo = subtipo;
        this.modulo = modulo;
        this.obj = obj;
    }

    mostrar_mensaje() {
        if (this.tipo == this.T_LOG) {
            return M.UTF8ToString(this.obj);
        }
        else if (this.tipo == this.T_ADVERTENCIA) {
        // FIX: parametrizar esto, por ahora solo hay una opción
            if (subtipo == 25) //< COD_A_SEMANTICO_INSTRUCCION_SIN_ETI
                return "La directiva procesada necesita una etiqueta, y no ha sido provista";
        }
        return "";
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
 * @callback Maquina~mensaje_f
 * @param {number} tipo Tipo del mensaje a recibir.
 * @param {number} subtipo Tipo más específico del mensaje.
 * @param {string} modulo Módulo del cual proviene el mensaje.
 * @param {object} detalles Objeto que contiene datos adicionales sobre el mensaje.
 * @return {number} Código de estado. 0 si todo salió bien.
 */

/**
 * Instancia de una máquina virtual.
 * @class
 * @param {Maquina~carga_archivo_f} cargar_archivo: Función llamada por Lily para pedir al sistema cliente un archivo nuevo.
 * @param {Maquina~cierre_archivo_f} cerrar_archivo Función llamada por Lily para cerrar un archivo.
 * @param {Maquina~mensaje_f} mensaje Función llamada por Lily para procesar mensajes.
 */
class Maquina {
    constructor(cargar_archivo, cerrar_archivo, mensaje) {
        // Funciones para cargar archivo
        if (typeof(cargar_archivo) != "function")
            throw new CargarArchivoFaltanteError();
        this.cargar_archivo = cargar_archivo;
        if (typeof(cerrar_archivo) != "function")
            throw new CerrarArchivoFaltanteError();
        this.cerrar_archivo = cerrar_archivo;
        if (typeof(mensaje) != "function")
            throw new MensajeFaltanteError();
        this.mensaje = mensaje;
    }

    /**
     * Ensambla un bloque de código en ensamblador.
     * @param {string} datos_entrada Código fuente a ensamblar.
     * @param {string} arquitectura Identificador del esquema del microprocesador a utilizar.
     * @return {Uint8Array} Código objeto resultante.
     */
    ensamblar(datos_entrada, arquitectura) {
        let array_salida = undefined;

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

        // struct lily_lily_archivo* (fun_abrir_archivo)(const char*, int, int*)
        this.p_fun_cargar_archivo = M.addFunction((param_nombre, param_tipo, param_codigo) => {
            // Deserializar parámetros
            const nombre = M.UTF8ToString(param_nombre);
            // Llamar al cliente por la estructura
            console.log(this.cargar_archivo);
            const [obj, codigo] = this.cargar_archivo(nombre, param_tipo);
            M.setValue(param_codigo, codigo, "i32");
            return obj.ptr;
        }, "iiii");

        // int (fun_cerrar_archivo)(struct lily_lily_archivo*)
        this.p_fun_cerrar_archivo = M.addFunction((param_archivo) => {
            const archivo = Archivo.desde_puntero(param_archivo);
            const estado = this.cerrar_archivo();
            //archivo.destructor();
            return estado;
        }, "ii");

        // int (fun_mensaje)(enum lily_lily_mensaje_tipo, int, char*, void*)
        this.p_fun_mensaje = M.addFunction((param_tipo, param_subtipo, param_modulo, param_obj) => {
            const modulo = M.UTF8ToString(param_modulo);
            let mensaje = new Mensaje(param_tipo, param_subtipo, modulo, param_obj);
            const estado = this.mensaje(mensaje);
            return estado;
        }, "iiiii");

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
        let estado = 0;
        try {
            const p_res = M.ccall("lily_lily_ensamble",
                                       "number",
                                       ["number", "number", "number", "number", "number", "number", "number", "number"],
                                       [this.p_datos_entrada, this.p_arquitectura, this.p_fun_cargar_archivo, this.p_fun_cerrar_archivo, this.p_fun_mensaje, this.p_tam_salida, this.p_estado, this.pp_ctx]
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
            //M.removeFunction(this.ptr_fun_cargar_archivo);
            //M.removeFunction(this.ptr_fun_cerrar_archivo);
            //M.removeFunction(this.ptr_fun_mensaje);
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
}

export {
    Archivo,
    Maquina
};
