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
