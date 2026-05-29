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

; Suma dos números enteros diferentes de 0
; @param [0x0100] Primer operando entero
; @param [0x0101] Segundo operando entero
; @returns [0x0102] Resultado entero
;

.CPU "zilog.z80"

; Cargar primer operando
LD %A, [0x0100]			; 3a 00 01
CP 0 				; fe 00
eti2:
JR %Z, $eti1-$pc		; 28 0e
LD %B, %A			; 47
; Cargar segundo operando
LD %A,[0x0101]			; 3a 01 01
CP 0				; fe 00
JP %Z, $eti1			; ca 15 00

ADD %B				; 80

; Guardar resultado
LD [0x0102], %A			; 32 02 01
HALT				; 76

eti1:
LD %A, 0xFF			; 3e ff
LD [0x0102], %A			; 32 02 01
HALT				; 76
