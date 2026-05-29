; This file is free and unencumbered software released into the public
; domain.

; Anyone is free to copy, modify, publish, use, compile, sell, or
; distribute this software, either in source code form or as a compiled
; binary, for any purpose, commercial or non-commercial, and by any
; means.

; In jurisdictions that recognize copyright laws, the author or authors
; of this software dedicate any and all copyright interest in the
; software to the public domain. We make this dedication for the benefit
; of the public at large and to the detriment of our heirs and
; successors. We intend this dedication to be an overt act of
; relinquishment in perpetuity of all present and future rights to this
; software under copyright law.

; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
; EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
; MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
; IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
; OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
; ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
; OTHER DEALINGS IN THE SOFTWARE.

; For more information, please refer to <https://unlicense.org/>

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Guarda en el registro A la posición del bit activo
; @param [0x0100] Byte dónde buscar el bit
; @returns %A Número de la posición
;

.CPU "zilog.z80"

; Aquí puede cargarse 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02 o 0x01
LD %A, [0x100]                  ; 3a 00 01
LD %C, 0                        ; 0e 00
buscar: CP 1                    ; fe 01
JR %Z, $fin-$pc                 ; 28 05
INC %C                          ; 0c
RLC %A                          ; cb 07
JR $buscar-$pc                  ; 18 f7
fin: HALT                       ; 76
