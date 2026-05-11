#include "bitarray.h"

void lily_bitarray_obtener(size_t desplazamiento, size_t tamano, const uint8_t* array, uint8_t* bytes) {
    size_t bit_fin = desplazamiento + tamano;
    size_t tam_restante = tamano;
    size_t i = 0;
    size_t ptr = desplazamiento;
    while (tam_restante) {
        size_t ventana = (tam_restante < 8)?tam_restante:8;
        tam_restante -= ventana;
        size_t v1_byte_inicio = ptr >> 3;
        size_t v1_limite_tentativo = 8 * (v1_byte_inicio + 1) - 1;
        size_t v1_limite = (bit_fin < v1_limite_tentativo)?bit_fin:v1_limite_tentativo;
        size_t v1_tamano = v1_limite - ptr + 1;
        uint8_t v1_mascara = 0xff << (8 - v1_tamano);
        uint8_t v1_val = array[v1_byte_inicio] & v1_mascara;
        bytes[i] = v1_val >> (8 - v1_tamano);
        ptr += v1_tamano;
        ventana -= v1_tamano;
        if (ventana) {
            size_t v2_byte_inicio = v1_byte_inicio + 1;
            size_t v2_tamano = ventana;
            uint8_t v2_mascara = 0xff >> (8 - v2_tamano);
            uint8_t v2_val = array[v2_byte_inicio] & v2_mascara;
            bytes[i] |= v2_val << v2_tamano;
            ptr += v2_tamano;
        }
        i++;
    }
}

void lily_bitarray_guardar(size_t desplazamiento, size_t tamano, uint8_t* array, const uint8_t* bytes) {
    size_t bit_fin = desplazamiento + tamano;
    size_t tam_restante = tamano;
    size_t i = 0;
    size_t ptr = desplazamiento;
    while (tam_restante) {
        size_t ventana = (tam_restante < 8)?tam_restante:8;
        tam_restante -= ventana;
        size_t v1_byte_inicio = ptr >> 3;
        size_t v1_limite_tentativo = 8 * (v1_byte_inicio + 1) - 1;
        size_t v1_limite = (bit_fin < v1_limite_tentativo)?bit_fin:v1_limite_tentativo;
        size_t v1_tamano = v1_limite - ptr + 1;
        uint8_t v1_mascara = 0xff >> (8 - v1_tamano);
        uint8_t v1_mascara_limpiar = ~(0xff << (8 - v1_tamano));
        uint8_t v1_val = bytes[i] & v1_mascara;
        array[v1_byte_inicio] &= v1_mascara_limpiar;
        array[v1_byte_inicio] |= v1_val << (8 - v1_tamano);
        ptr += v1_tamano;
        ventana -= v1_tamano;
        if (ventana) {
            size_t v2_byte_inicio = v1_byte_inicio + 1;
            size_t v2_tamano = ventana;
            uint8_t v2_mascara = 0xff << (8 - v2_tamano);
            uint8_t v2_mascara_limpiar = ~(0xff >> (8 - v2_tamano));
            uint8_t v2_val = bytes[i] & v2_mascara;
            array[v2_byte_inicio] &= v2_mascara_limpiar;
            array[v2_byte_inicio] |= v2_val >> v2_tamano;
            ptr += v2_tamano;
        }
        i++;
    }
}
