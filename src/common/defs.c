#include "defs.h"

char* d_printf(char* formato, ...) {
    va_list args;
    va_start(args, formato);
    // FIX: encontrar la forma de obtener el tamaño exacto con solo una llamada a vsnprintf
    char* buffer = (char*) malloc(255);
    vsnprintf(buffer, 255, formato, args);
    va_end(args);
    return buffer;
}
