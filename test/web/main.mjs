/*
 * This file is free and unencumbered software released into the public
 * domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <https://unlicense.org/>
 */

import * as L from "./lily/main.mjs";

function limpiar() {
    document.getElementById("pa-est").textContent = "Ready";
    document.getElementById("pa-res").textContent = "";
}

function abrir_archivo(nombre, tipo) {
    if (tipo == L.Archivo.T_ESQUEMA_PROCESADOR)
        return [ new L.Archivo(0, document.getElementById("pa-ds").value, tipo), 0 ];
    else if (tipo == L.Archivo.T_ARCHIVO_REGULAR)
        return [ undefined, 1 ];
}

function cerrar_archivo(obj) {
    // Aquí no necesitamos hacer nada de nada
    return 0;
}

function mensaje(mensaje) {
    // Por ahora, solo para pruebas
    switch (mensaje.constructor.name) {
    case "MensajeLog":
        let params = ["%c%s%c: %s", "font-weight: bold", mensaje.modulo, "font-weight: normal", mensaje.mensaje];
        switch (mensaje.subtipo) {
        case 0:
            console.log(...params); break;
        case 1:
            console.info(...params); break;
        case 2:
            console.warn(...params); break;
        default:
            console.error(...params); break;
        }
        break;
    case "MensajeMemoria":
        let memoria_tabla = document.getElementById("pe-memory");
        let direccion_s = mensaje.direccion.toString(16);
        let valor_s = mensaje.valor.toString(16);
        if (mensaje.operacion == 0)
            console.log(`Reading address 0x${direccion_s}`);
        else {
            console.log(`Writing address 0x${direccion_s} = ${valor_s}`);
        }
        break;
    case "MensajeRegistro":
        let registro_tabla = document.getElementById("pe-reg");
        switch (mensaje.subtipo) {
        case 0:
            console.log(`Reading register ${mensaje.registro}`);
        case 1:
            console.log(`Writing register ${mensaje.registro} = ${mensaje.valor.toString(16)}`);
        case 2:
            console.log(`Creating new register context ${mensaje.valor.toString(16)}`);
        case 3:
            console.log(`Restoring register context ${mensaje.valor.toString(16)}`);
        case 4:
            console.log(`Deleting register context ${mensaje.valor.toString(16)}`);
        }
        break;
/*    case "MensajePila":
        let pila_tabla = document.getElementById("pe-stack");
        switch () {
        case :
        case :
        case :
        case :
        }
        break;*/
    }
}

document.getElementById("pa-run").addEventListener("click", (e) => {
    e.preventDefault();
    if (!e.target.parentNode.checkValidity()) return;
    e.target.disabled = true;

    const e_af = document.getElementById("pa-af");
    const e_ds = document.getElementById("pa-ds");
    const e_res = document.getElementById("pa-res");
    const e_est = document.getElementById("pa-est");
    e_est.textContent = "Assemblying...";
    e_res.value = "";

    if (!window.maquinita) {
        window.maquinita = new L.Maquina(abrir_archivo, cerrar_archivo, mensaje);
    }

    // Ensamblar
    let [estado, res] = window.maquinita.ensamblar(e_af.value, document.getElementById("scheme-list").value);

    e_est.textContent = "Estado: " + estado.toString() + " (" + res.length + " bytes)";

    e_res.value = Array.from(res).map(v => v.toString(16).padStart(2, "0")).join(" ");
    e.target.disabled = false;
});

document.getElementById("pa-load").addEventListener("click", (e) => {
    e.preventDefault();
    // Iniciar máquina
    window.maquinita.iniciar(new Uint8Array(document.getElementById("pa-res").value.split(" ")), "zilog.z80");
});

document.getElementById("pa-af").addEventListener("input", limpiar);
document.getElementById("pa-ds").addEventListener("input", limpiar);

// Funciones para Ejecución

function memoria_escribir(dir, val) {

}

function pila_contexto_crear(dir, tamano) {
    let tabla = document.getElementById("pe-stack");
    Array.from(tabla.tBodies).forEach(e => { e.ariaCurrent = undefined; });
    let pagina = tabla.createTBody();
    pagina.ariaCurrent = "page";
    let dir_f = dir.toString(16).toUpperCase().padStart(4, "0");
    pagina.id = "stack-" + dir_f;
    pagina.dataset.tamano = tamano;
    let opt = document.createElement("option");
    opt.textContent = dir_f;
    let selector = document.getElementById("pe-stackpage");
    selector.options.add(opt);
    selector.value = dir_f;
}
function pila_contexto_restaurar(dir) {
    let tabla = document.getElementById("pe-stack");
    if (tabla.tBodies.length) {
        Array.from(tabla.tBodies).forEach(e => { e.ariaCurrent = undefined;
                                               });
        let dir_f = dir.toString(16).toUpperCase().padStart(4, "0");
        let nueva = document.getElementById("stack-" + dir_f);
        nueva.ariaCurrent = "page";
        let selector = document.getElementById("pe-stackpage");
        Array.from(selector.options).find(e => e.textContent == nueva.id.substring(6)).selected = true;
    }
}
function pila_contexto_eliminar(dir) {
    let tabla = document.getElementById("pe-stack");
    let dir_f = dir.toString(16).toUpperCase().padStart(4, "0");
    let pagina = document.getElementById("stack-" + dir_f);
    let es_activa = pagina.ariaCurrent == "page";
    pagina.parentNode.removeChild(pagina);
    let selector = document.getElementById("pe-stackpage");
    let selector_opt = Array.from(selector.options).find(e => e.textContent == dir_f);
    selector_opt.parentNode.removeChild(selector_opt);
    if (es_activa && tabla.tBodies.length) {
        let nueva = tabla.tBodies[tabla.tBodies.length];
        nueva.ariaCurrent = "page";
        Array.from(selector.options).find(e => e.textContent == nueva.id.substring(6)).selected = true;
    }
}
//function pila_lectura() {}
//function pila_escritura(valor) {}
function pila_anadir(val) {
    let tabla = document.getElementById("pe-stack");
    let pagina = tabla.querySelector("[aria-current='page']");
    let tamano = parseInt(pagina.dataset.tamano);
    if (pagina) {
        let dir = parseInt(pagina.id.substring(6), 16) - (pagina.rows.length * parseInt(pagina.dataset.tamano)) - 1;
        let fila = pagina.insertRow();
        let dir_e = document.createElement("th");
        // Por ahora, 4 caracteres
        dir_e.textContent = dir.toString(16).toUpperCase().padStart(4, "0");
        fila.appendChild(dir_e);
        let val_e = fila.insertCell();
        val_e.textContent = val;
    }
}
function pila_quitar() {
    let tabla = document.getElementById("pe-stack");
    let pagina = tabla.querySelector("[aria-current='page']");
    if (pagina && pagina.lastElementChild) {
        pagina.lastElementChild.parentNode.removeChild(pagina.lastElementChild);
    }
}

document.getElementById("pe-next").addEventListener("click", (e) => {
    e.preventDefault();
    window.maquinita.ejecutar();
});

document.getElementById("pe-stop").addEventListener("click", (e) => {
    e.preventDefault();
});

fetch("cpu/index")
    .then(res => {
        if (!res.ok) {
            throw new Error(`Error while fetching architecture schemes list: ${lista_cpu_r.status}`);
        }
        return res.json();
    })
    .then(lista => {
        let lista_select = document.getElementById("scheme-list");
        let lista_promesas = [];
        for (let s of lista) {
            let opcion = document.createElement("option");
            opcion.textContent = s;
            lista_select.add(opcion);
            lista_promesas.push(
                fetch(`cpu/${s}.lua`)
                    .then(async peticion => {
                        let prom = await peticion.text();
                        let ident = (new URL(peticion.url)).pathname.split("/");
                        ident.shift(); ident.shift();
                        ident = ident.join("/").split(".");
                        ident.pop();
                        ident = ident.join(".");
                        window.esquemas[ident] = prom;
                    })
            );
        }
        window.esquemas = {};
        Promise.all(lista_promesas);
    })
    .catch(e => {
        console.error(e.message);
    });
document.getElementById("scheme-list").addEventListener("change", e => {
    let campo = document.getElementById("pa-ds");
    let esquema = window.esquemas[e.target.value];
    if (!esquema) {
        campo.value = undefined;
        campo.disabled = false;
    }
    else {
        campo.value = esquema;
        campo.disabled = true;
    }
});

function estilar_pestanas(ev) {
    Array.from(document.querySelectorAll("[role='tab']")).forEach(e =>
        e.ariaSelected = e.href == ev.newURL
    );
}
if (window.location.hash == "") window.location.hash = "assembly";
window.addEventListener("hashchange", estilar_pestanas);
estilar_pestanas({ newURL: window.location.href });
