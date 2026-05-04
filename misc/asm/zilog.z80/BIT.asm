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
