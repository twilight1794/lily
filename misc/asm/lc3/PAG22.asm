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

; Ejemplo de la página 22
; x30F6 1110 0011 1111 1101    0xE3 0xFD    R1 ← PC – 3 = x30F4     LEA %R1, 0x1FD
; x30F7 0001 0100 0110 1110    0x14 0x6E    R2 ← R1 + 14 = x3102    ADD %R2, %R1, 14
; x30F8 0011 0101 1111 1011    0x35 0xFB    M[PC - 5] ← R2          ST 0x1FB, %R2
;                                           M[x30F4] ← x3102
; x30F9 0101 0100 1010 0000    0x54 0xA0    R2 ← 0                  AND %R2, %R2, 0
; x30FA 0001 0100 1010 0101    0x14 0xA5    R2 ← R2 + 5 = 5         ADD %R2, %R2, 5
; x30FB 0111 0100 0100 1110    0x74 0x4E    M[R1+14] ← R2           ST [%R1, 0xE], %R2
;                                           M[x3102] ← 5
; x30FC 1010 0111 1111 0111    0xA7 0xF7    R3 ← M[M[x30F4]]        LD %R3, [0x17F]
;                                           R3 ← M[x3102]
;                                           R3 ← 5

.CPU "lc3"
val: .DWL 0
LEA %R1, 0x1FD
ADD %R2, %R1, 14
ST 0x1FB, %R2
AND %R2, %R2, 0
ADD %R2, %R2, 5
ST [%R1, 0xE], %R2
LD %R3, [0x17F]
