"use strict";
let lily;

function limpiar(){
    document.getElementById("pa-est").textContent = "Ready";
    document.getElementById("pa-res").textContent = "";
}

Module().then(Module => {
    lily = Module;
    document.getElementById("pa-run").addEventListener("click", (e) => {
        e.preventDefault();
        if (!e.target.parentNode.checkValidity()) return;
        e.target.disabled = true;

        const e_af = document.getElementById("pa-af");
        const e_ds = document.getElementById("pa-ds");
        const e_res = document.getElementById("pa-res");
        const e_est = document.getElementById("pa-est");
        e_est.textContent = "Assemblying...";
        e_res.textContent = "";

        // Crear configuración de log
        const p_log = Module._malloc(8);
        Module.setValue(p_log, 0, "i8");  // bool colores;
        Module.setValue(p_log + 1, 0, "i8");  // bool incluir_fecha;
        Module.setValue(p_log + 2, 0, "i8");  // bool incluir_hora;
        Module.setValue(p_log + 3, 0, "i8");  // bool incluir_archivo;
        Module.setValue(p_log + 4, 0, "i32"); // enum lily_log_nivel nivel_minimo;

        // Establecer parámetros para uint8_t* lily_lily_ensamble
        /// const char* datos_entrada
        const p_datos_entrada = Module._malloc(Module.lengthBytesUTF8(e_af.value) + 1);
        Module.stringToUTF8(e_af.value, p_datos_entrada, Module.lengthBytesUTF8(e_af.value) + 1);
        /// char* arquitectura
        const p_arquitectura = 0; // <- Esto será nulo siempre acá
        /// struct lily_lily_archivo* (fun_abrir_archivo)(const char*, int, struct lily_ctx*),
        const p_fun_abrir_archivo = Module.addFunction((param_nombre, param_tipo, param_ctx) => {
            const nombre = Module.UTF8ToString(param_nombre);
            console.log("Querying for Lua scheme ", nombre);
            //// Primero, la cadena...
            const p_fun_abrir_archivo_d = Module._malloc(Module.lengthBytesUTF8(e_ds.value) + 1);
            Module.stringToUTF8(e_ds.value, p_fun_abrir_archivo_d, Module.lengthBytesUTF8(e_ds.value) + 1);
            //// ...y el struct_lily_lily_archivo que la contendrá
            const p_fun_abrir_archivo_ret = Module._malloc(12);
            Module.setValue(p_fun_abrir_archivo_ret, 0, "i32");                     // void* obj;
            Module.setValue(p_fun_abrir_archivo_ret+4, p_fun_abrir_archivo_d, "i32"); // char* archivo;
            Module.setValue(p_fun_abrir_archivo_ret+8, 0, "i32");               // int tipo;
            return p_fun_abrir_archivo_ret;
        }, "piii");
        /// int (fun_cerrar_archivo)(struct lily_lily_archivo*),
        const p_fun_cerrar_archivo = Module.addFunction((param_nombre, param_tipo, param_ctx) => {
            return 0;
        }, "ii");
        /// size_t* tam_salida
        const p_tam_salida = Module._malloc(4);
        Module.setValue(p_tam_salida, 0, "i32");
        /// struct lily_ctx* ctx
        const p_ctx = Module._malloc(48);
        Module.setValue(p_ctx, 0, "i32");     // enum lily_estado codigo
        Module.setValue(p_ctx + 4, 0, "i32"); // enum lily_simbolo_tipo tipo
        Module.setValue(p_ctx + 8, 0, "i32"); // size_t i_inicial
        Module.setValue(p_ctx + 12, 0, "i32"); // size_t i_desp
        Module.setValue(p_ctx + 16, 0, "i32"); // struct lily_simbolo_simbolo* ultimo
        Module.setValue(p_ctx + 20, 0, "i32"); // char* lua_msg
        Module.setValue(p_ctx + 24, p_log, "i32"); // void* log_cfg

        // Llamar a lily_lily_ensamble
        try {
            const p_res = Module.ccall("lily_lily_ensamble",
                "number",
                ["number", "number", "number", "number", "number", "number"],
                [p_datos_entrada, p_arquitectura, p_fun_abrir_archivo, p_fun_cerrar_archivo, p_tam_salida, p_ctx]
            );
            const estado = Module.getValue(p_ctx, "i32");
            const tam_salida = Module.getValue(p_tam_salida, "i32");
            e_est.textContent = "Estado: " + estado.toString() + " (" + tam_salida + " bytes)";
            for (let i = 0; i<tam_salida; i++) {
                const v = Module.getValue(p_res + i, "i8");
                e_res.textContent += v.toString(16).padStart(2, "0") + " ";
            }
        } finally {
            Module._free(p_log);
            Module._free(p_datos_entrada);
            //Module._free(p_fun_abrir_archivo_d);
            //Module._free(p_fun_abrir_archivo_ret);
            Module._free(p_tam_salida);
            Module._free(p_ctx);
            Module.removeFunction(p_fun_abrir_archivo);
            Module.removeFunction(p_fun_cerrar_archivo);
        }
        e.target.disabled = false;
    });
    document.getElementById("pa-af").addEventListener("input", limpiar);
    document.getElementById("pa-ds").addEventListener("input", limpiar);
});
