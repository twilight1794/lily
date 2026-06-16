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
 * Clases para mensajes de Lily.
 * @module liblily.Mensaje
 */

import { LilyError } from "./Error.mjs";

class TipoMensajeError extends LilyError {
    constructor() {
        super("El tipo de mensaje debe estar entre 0 y 8");
    }
}

class SubtipoMensajeError extends LilyError {
    constructor(v) {
        super("El subtipo u operación especificado no es válido: " + toString(v));
    }
}

class Mensaje {
    static obt_clase(tipo_int) {
        switch (tipo_int) {
        case 0:
            return MensajeLog;
        case 1:
            return MensajeAdvertencia;
        case 2:
            return MensajeEtiqueta;
        case 3:
            return MensajeVariable;
        case 4:
            return MensajeMemoria;
        case 5:
            return MensajeRegistro;
        case 6:
            return MensajePila;
        case 7:
            return MensajeDispositivo;
        case 8:
            return MensajeInterrupcion;
        default:
            throw new TipoMensajeError();
        }
    }

    constructor(M, modulo) {
        if (this.constructor == Mensaje) {
            throw new Error("Abstract class");
        }
        this.M = M;
        this.modulo = M.UTF8ToString(modulo);
    }
}

class MensajeLog extends Mensaje {
    constructor(M, modulo, subtipo, obj) {
        super(M, modulo);
        if (subtipo >= 0 && subtipo < 5)
            this.subtipo = subtipo;
        else
            throw new SubtipoMensajeError(subtipo);
        this.mensaje = M.UTF8ToString(obj);
    }

    toString() {
        return this.mensaje;
    }
}

class MensajeAdvertencia extends Mensaje {
    constructor(M, modulo, subtipo) {
        super(M, modulo);
        this.codigo = subtipo;
    }

    toString() {
        return this.codigo.toString();
    }
}

class MensajeEtiqueta extends Mensaje {
    constructor(M, modulo) {
        super(M, modulo);
    }
}

class MensajeVariable extends Mensaje {
    constructor(M, modulo) {
        super(M, modulo);
    }
}

class MensajeMemoria extends Mensaje {
    constructor(M, modulo, operacion, obj) {
        super(M, modulo);
        if (operacion >= 0 && operacion < 2)
            this.operacion = operacion;
        else
            throw new SubtipoMensajeError(operacion);
        this.direccion = M.getValue(obj, "i64");
        this.valor = M.getValue(obj + 8, "i64");
        this.tamano = M.getValue(obj + 16, "i32");
    }
}

class MensajeRegistro extends Mensaje {
    constructor(M, modulo, operacion, obj) {
        super(M, modulo);
        if (operacion >= 0 && operacion < 5)
            this.operacion = operacion;
        else
            throw new SubtipoMensajeError(operacion);
        this.valor = M.getValue(obj, "i64");
        if (operacion < 2) {
            this.desplazamiento = M.getValue(obj + 8, "i64");
            let p_registro = M.getValue(obj + 16, "i32");
            this.registro = M.UTF8ToString(p_registro);
            this.tamano = M.getValue(obj + 20, "i32");
        }
    }
}

class MensajePila extends Mensaje {
    constructor(M, modulo, operacion, obj) {
        super(M, modulo);
        if (operacion >= 0 && operacion < 7)
            this.operacion = operacion;
        else
            throw new SubtipoMensajeError(operacion);
        if (operacion == 1 || operacion == 2)
            this.valor = M.getValue(obj, "i64");
        else if (operacion >= 4)
            this.direccion = M.getValue(obj, "i64");
        if (operacion == 4)
            this.tamano = M.getValue(obj + 8, "i32");
    }
}

class MensajeDispositivo extends Mensaje {
    constructor(M, modulo) {
        super(M, modulo);
    }
}

class MensajeInterrupcion extends Mensaje {
    constructor(M, modulo) {
        super(M, modulo);
    }
}

export {
    Mensaje,
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
    SubtipoMensajeError,
};
