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
