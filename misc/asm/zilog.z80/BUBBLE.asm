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

; Programa que ordena valores mediante el algoritmo
; BubbleSort
; @param [0xF0] Byte dónde buscar el bit
; @param [0x100..] Array de elementos a ordenar
;

.CPU "zilog.z80"

LD %A, [0xF0]
LD %D, %A
LD %B, 0

LD %IY, 0x100

ciclo1:
LD %E, 0
LD %IX, 0x100
LD %C, 0
INC %B

ciclo2:
INC %C
LD %A, [%IX, 1]
LD %H, [%IX] ; o [%IX, 0]
CP %H
JP %P, $no
LD %L, %H
LD [%IX], %A
LD [%IX, 1], %L
LD %E, 1

no:
INC %IX
LD %A, %D
SUB %B
DEC %A
CP %C
JP %P, $ciclo2
LD %A, 0
CP %E
JP %P, $salto1
INC %IY
LD %A, %D
CP %B
JP %P, $ciclo1

salto1:
        HALT
