; Ejemplo de la página 22
; x30F6 1110 0011 1111 1101    0xE3 0xFD    R1 ← PC – 3 = x30F4     LEA %R1, 0x1FD
; x30F7 0001 0100 0110 1110    0x14 0x6E    R2 ← R1 + 14 = x3102    ADD %R2, %R1, 14
; x30F8 0011 0101 1111 1011    0x35 0xFB    M[PC - 5] ← R2          ST 0x1FB, %R2
;                                           M[x30F4] ← x3102
; x30F9 0101 0100 1010 0000    0x54 0xA0    R2 ← 0                  AND %R2, %R2, 0
; x30FA 0001 0100 1010 0101    0x14 0xA5    R2 ← R2 + 5 = 5         ADD %R2, 5
; x30FB 0111 0100 0100 1110    0x74 0x4E    M[R1+14] ← R2           ST [%R1, 0xE], %R2
;                                           M[x3102] ← 5
; x30FC 1010 0111 1111 0111    0xA7 0xF7    R3 ← M[M[x30F4]]        LD R3, [0x17F]
;                                           R3 ← M[x3102]
;                                           R3 ← 5

.CPU "lc3"
LEA %R1, 0x1FD
ADD %R2, %R1, 14
ST 0x1FB, %R2
AND %R2, %R2, 0
ADD %R2, 5
ST [%R1, 0xE], %R2
LD R3, [0x17F]