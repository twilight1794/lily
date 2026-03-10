#include "nums.h"

union lily_nums_num {
    int64_t neg;
    uint64_t pos;
    uint8_t byte[4];
};

enum lily_estado lily_nums_codificar_num(uint8_t* buf, void* val, const size_t tam, const bool negativo, const bool endianness) {
    // Guardar número
    union lily_nums_num num;
    if (negativo) num.neg = *((int64_t*) val);
    else num.pos = *((uint64_t*) val);

    // Validar tamaño
    if (negativo) {
        if (tam == 1 && (num.neg < INT8_MIN || num.neg > INT8_MAX)) return COD_NUMS_MAS_8;
        if (tam == 2 && (num.neg < INT16_MIN || num.neg > INT16_MAX)) return COD_NUMS_MAS_16;
        if (tam == 4 && (num.neg < INT32_MIN || num.neg > INT32_MAX)) return COD_NUMS_MAS_32;
    } else {
        if (tam == 1 && num.pos > UINT8_MAX) return COD_NUMS_MAS_8;
        if (tam == 2 && num.pos > UINT16_MAX) return COD_NUMS_MAS_16;
        if (tam == 4 && num.pos > UINT32_MAX) return COD_NUMS_MAS_32;
    }

    // Escribir número
    for (size_t i = 0; i < tam; i++) {
        buf[endianness ? i : (tam-1)-i] = num.byte[i];
    }
    return COD_OK;
}
