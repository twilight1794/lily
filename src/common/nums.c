#include "nums.h"

enum lily_estado lily_nums_codificar_num(uint8_t* buf, void* val, const size_t tam, const int signo, const bool endianness) {
    // Guardar número
    union lily_simbolo_numero num;
    num.positivo = *((uint64_t*) val);

    // Validar tamaño
    if (signo == 0) {
        // unsigned
        if (tam == 1 && num.positivo > UINT8_MAX) return COD_NUMS_MAS_8;
        if (tam == 2 && num.positivo > UINT16_MAX) return COD_NUMS_MAS_16;
        if (tam == 4 && num.positivo > UINT32_MAX) return COD_NUMS_MAS_32;
    }
    else if (signo == 1) {
        // signed
        if (tam == 1 && (num.negativo < INT8_MIN || num.negativo > INT8_MAX)) return COD_NUMS_MAS_8;
        if (tam == 2 && (num.negativo < INT16_MIN || num.negativo > INT16_MAX)) return COD_NUMS_MAS_16;
        if (tam == 4 && (num.negativo  < INT32_MIN || num.negativo > INT32_MAX)) return COD_NUMS_MAS_32;
    }
    else {
        // Todo
        if (tam == 1 && (num.negativo < INT8_MIN || num.positivo > UINT8_MAX)) return COD_NUMS_MAS_8;
        if (tam == 2 && (num.negativo < INT16_MIN || num.positivo > UINT16_MAX)) return COD_NUMS_MAS_16;
        if (tam == 4 && (num.negativo  < INT32_MIN || num.positivo > UINT32_MAX)) return COD_NUMS_MAS_32;
    }

    // Escribir número
    for (size_t i = 0; i < tam; i++) {
        buf[endianness ? i : (tam-1)-i] = num.bytes[i];
    }
    return COD_OK;
}
