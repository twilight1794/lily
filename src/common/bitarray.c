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

#include "bitarray.h"

void lily_bitarray_obtener(const void* array, size_t desplazamiento, size_t tamano, void* valor) {
    const size_t tam_ventana = desplazamiento % 8; // El offset real desde el comienzo de un byte -> 0 para alineados
    const size_t tam_ventana_comp = 8 - tam_ventana; // Restante -> 8 para alineados
    size_t restantes = tamano; // Bits que faltan por leer globalmente
    bool inicio_ventana = true; // Si empezamos a escribir en un nuevo byte de destino
    uint8_t* byte_origen = ((uint8_t*) array) + (desplazamiento >> 3); // Puntero al byte actual del array origen
    uint8_t* byte_destino = ((uint8_t*) valor); // Puntero al byte actual del valor destino
    size_t restantes_reales; // Tomar en cuenta si tenemos menos de una ventana para copiar
    uint8_t mascara; // Máscara para bits a copiar
    uint8_t valor_bruto; // Copia del byte origen, con el dato a extraer
    uint8_t valor_neto; // Copia del valor bruto, ya normalizado para ser insertado en destino

    while (restantes) {
        if (inicio_ventana) {
            // 1ra ventana destino
            restantes_reales = (tam_ventana_comp > restantes) ? restantes : tam_ventana_comp;
            /// Obtener máscara
            mascara = 0xff << (8 - restantes_reales);
            mascara = mascara >> (8 - restantes_reales - tam_ventana);
            /// Aplicar máscara contra array
            valor_bruto = (*byte_origen & mascara);
            /// Aplicar a destino (con el desplazamiento correcto)
            valor_neto = valor_bruto >> tam_ventana;
            *byte_destino |= valor_neto;

            restantes -= restantes_reales;
        }
        else {
            // Evitar cálculos inecesarios cuando la 1ra ventana sea 8
            if (!tam_ventana) goto fin;
            // 2da ventana destino
            restantes_reales = (tam_ventana > restantes) ? restantes : tam_ventana;
            /// Obtener máscara
            mascara = 0xff >> (8 - restantes_reales);
            // Aplicar máscara contra array
            valor_bruto = (*(byte_origen + 1) & mascara);
            // Aplicar a destino (con el desplazamiento correcto)
            valor_neto = valor_bruto << tam_ventana_comp;
            *byte_destino |= valor_neto;

            restantes -= restantes_reales;
            fin:
            byte_origen++;
            byte_destino++;
        }
        inicio_ventana = !inicio_ventana;
    }
}

void lily_bitarray_guardar(void* array, size_t desplazamiento, size_t tamano, const void* valor) {
    const size_t tam_ventana = desplazamiento % 8; // Offset real desde el comienzo de un byte -> 0 para alineados
    const size_t tam_ventana_comp = 8 - tam_ventana; // Restante -> 8 para alineados
    size_t restantes = tamano; // Bits que faltan por leer globalmente
    bool inicio_ventana = true; // Si empezamos a escribir en un nuevo byte de destino
    uint8_t* byte_origen = ((uint8_t*) valor); // Puntero al byte actual del valor origen
    uint8_t* byte_destino = ((uint8_t*) array) + (desplazamiento >> 3); // Puntero al byte actual del array destino
    size_t restantes_reales; // Tomar en cuenta si tenemos menos de una ventana para copiar
    uint8_t mascara; // Máscara para bits a copiar
    uint8_t valor_bruto; // Copia del byte origen, solo con el dato a extraer, y ya normalizado para ser insertado en destino

    while (restantes) {
        if (inicio_ventana) {
            // 1ra ventana destino
            /// Preparar origen
            restantes_reales = (tam_ventana_comp > restantes) ? restantes : tam_ventana_comp;
            //// Obtener máscara para origen
            mascara = 0xff >> (8 - restantes_reales);
            //// Separar primera ventana de dato y desplazar
            valor_bruto = (*byte_origen & mascara) << tam_ventana;
            /// Preparar destino
            //// Obtener máscara para destino
            mascara = ~(mascara << tam_ventana);
            //// Limpiar espacio de inserción
            *byte_destino &= mascara;
            /// Aplicar
            *byte_destino |= valor_bruto;

            restantes -= restantes_reales;
        }
        else {
            // 2da ventana destino
            /// Preparar origen
            if (!tam_ventana) goto fin;
            restantes_reales = (tam_ventana > restantes) ? restantes : tam_ventana;
            //// Obtener máscara para origen
            mascara = 0xff >> (8 - restantes_reales);
            mascara <<= tam_ventana_comp;
            ////  Separar segunda ventana de dato y desplazar
            valor_bruto = (*byte_origen & mascara) >> tam_ventana_comp;
            /// Preparar destino
            //// Obtener máscara para destino
            mascara = 0xff >> (8 - restantes_reales);
            //// Limpiar espacio de inserción
            *(byte_destino + (tam_ventana?1:0)) &= mascara;
            //// Aplicar
            *(byte_destino + (tam_ventana?1:0)) |= valor_bruto;

            restantes -= restantes_reales;
            fin:
            byte_origen++;
            byte_destino++;
        }
        inicio_ventana = !inicio_ventana;
    }
}
