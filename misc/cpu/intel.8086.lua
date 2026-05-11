-- Especificación del microprocesador Intel 8086

mmm_vals = {
    DSBXSI = 0,
    DSBXDI = 1,
    SSBPSI = 2,
    SSBPDI = 3,
    DSSI = 4,
    DSDI = 5,
    SSBP = 6,
    DSBX = 7
}

r8_vals = {
    AL = 0,  CL = 1,
    DL = 2,  BL = 3,
    AH = 4,  CH = 5,
    DH = 6,  BH = 7
}

r16_vals = {
    AX = 0,  CX = 1,
    DX = 2,  BX = 3,
    SP = 4,  BP = 5,
    SI = 6,  DI = 7
}

s_vals = {
    ES = 0,
    CS = 1,
    SS = 2,
    DS = 3
}

a_vals = {
    AL = 0,
    AX = 1
}

function add_tr(bytes, a, b)
    if bytes[1] >= 0x80 then
        bytes[2] = (bytes[2] & 0xc7) | (b << 3)
    else
        bytes[1] = bytes[1] + a
    end
    return bytes
end

function sumar2_tr(bytes, a)
    bytes[2] = bytes[2] + (a << 3)
    return bytes
end

function borrar_sumar2_tr(bytes, a)
    bytes[2] = (bytes[2] & 0xc7) | (a << 3)
    return bytes
end

return {
    id = "intel.8086",
    max_memoria = 1048576,
    registros = {
        AL = {
            tamano = 8,
            desplazamiento = 0
        },
        AH = {
            tamano = 8,
            desplazamiento = 8
        },
        AX = {
            tamano = 16,
            desplazamiento = 0
        },
        CL = {
            tamano = 8,
            desplazamiento = 16
        },
        CH = {
            tamano = 8,
            desplazamiento = 24
        },
        CX = {
            tamano = 16,
            desplazamiento = 16
        },
        DL = {
            tamano = 8,
            desplazamiento = 32
        },
        DH = {
            tamano = 8,
            desplazamiento = 40
        },
        DX = {
            tamano = 16,
            desplazamiento = 32
        },
        BL = {
            tamano = 8,
            desplazamiento = 48
        },
        BH = {
            tamano = 8,
            desplazamiento = 56
        },
        BX = {
            tamano = 16,
            desplazamiento = 48
        },
        SP = {
            tamano = 16,
            desplazamiento = 64
        },
        BP = {
            tamano = 16,
            desplazamiento = 80
        },
        SI = {
            tamano = 16,
            desplazamiento = 96
        },
        DI = {
            tamano = 16,
            desplazamiento = 112
        },
        IP = {
            tamano = 16,
            desplazamiento = 128
        },
        ES = {
            tamano = 16,
            desplazamiento = 144
        },
        CS = {
            tamano = 16,
            desplazamiento = 160
        },
        SS = {
            tamano = 16,
            desplazamiento = 176
        },
        DS = {
            tamano = 16,
            desplazamiento = 192
        },
        CF = {
            tamano = 1,
            desplazamiento = 208
        },
        PF = {
            tamano = 1,
            desplazamiento = 209
        },
        AF = {
            tamano = 1,
            desplazamiento = 210
        },
        ZF = {
            tamano = 1,
            desplazamiento = 211
        },
        SF = {
            tamano = 1,
            desplazamiento = 212
        },
        TF = {
            tamano = 1,
            desplazamiento = 213
        },
        IF = {
            tamano = 1,
            desplazamiento = 214
        },
        DF = {
            tamano = 1,
            desplazamiento = 215
        },
        OF = {
            tamano = 1,
            desplazamiento = 216
        },
    },
    registro_programa = "IP",
    tipos = {
        addr8 = function (a) return type(a) == "number" and a >= 0 and a <= 255 end, --uint8
        r8 = function (r) return type(r) == "string" and r8_vals[r:upper()] end,
        r16 = function (r) return type(r) == "string" and r16_vals[r:upper()] end,
        s = function (s) return type(s) == "string" and s_vals[s:upper()] end,
        m8 = function (m) return type(m) == "table" and #m == 1 and type(m[1]) == "number" and m[1] >= 0 and m[1] <= 65535 end,
        m16 = function (m) return type(m) == "table" and #m == 1 and type(m[1]) == "number" and m[1] >= 0 and m[1] <= 65535 end,
        i8 = function (i) return type(i) == "number" and i >= 0 and i <= 255 end, --uint8
        i16 = function (i) return type(i) == "number" and i >= 0 and i <= 65535 end, --uint16
        d = function (d) return type(d) == "number" and d >= -128 and d <= 127 end, --sint8
        ral = function (r) return type(r) == "string" and r:upper() == "AL" end,
        rax = function (r) return type(r) == "string" and r:upper() == "AX" end,
        --in-out
        ra = function (r) return type(r) == "string" and (r:upper() == "AX" or r:upper() == "AL") end,
        ddx = function (v) return type(v) == "table" and #v == 1 and type(v[1]) == "string" and v[1]:upper() == "DX" end,
        --int
        ["3"] = function (v) return v == 3 end,
        --jccc
        d8 = function (d) return type(d) == "number" and d >= -126 and d <= 127 end,
        --shr
        ["1"] = function (v) return v == 1 end,
        rcl = function (r) return type(r) == "string" and r:upper() == "CL" end,
        -- Especificadores de tamaño de punteros
        sbyte = function (v) return type(v) == "string" and v:upper() == "BYTE" end,
        sword = function (v) return type(v) == "string" and v:upper() == "WORD" end
    },
    ensamble = {
        AAA = { 0x37 },
        AAD = { 0xd5, 0x0a },
        AAM = { 0xd4, 0x0a },
        AAS = { 0x3f },
        ADC = {
            { { "r8", "r8" }, { "ADD", function (bytes) return add_tr(bytes, 0x10, 2) end } },
            { { "m8", "r8" }, { "ADD", function (bytes) return add_tr(bytes, 0x10, 2) end } },
            { { "r16", "r16" }, { "ADD", function (bytes) return add_tr(bytes, 0x10, 2) end } },
            { { "m16", "r16" }, { "ADD", function (bytes) return add_tr(bytes, 0x10, 2) end } },
            { { "r8", "m8" }, { "ADD", function (bytes) return add_tr(bytes, 0x10, 2) end } },
            { { "r16", "m16" }, { "ADD", function (bytes) return add_tr(bytes, 0x10, 2) end } },
            { { "ral", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x10, 2) end } },
            { { "rax", "i16" }, { "ADD", function (bytes) return add_tr(bytes, 0x10, 2) end } },
            { { "r8", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x10, 2) end } },
            { { "sbyte", "m8", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x10, 2) end } },
            { { "r16", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x10, 2) end } },
            { { "sword", "m16", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x10, 2) end } },
            { { "r16", "i16" }, { "ADD", function (bytes) return add_tr(bytes, 0x10, 2) end } },
            { { "sword", "m16", "i16" }, { "ADD", function (bytes) return add_tr(bytes, 0x10, 2) end } }
        },
        ADD = {
            {
                { "r8", "r8" },
                function (r1, r2) return { 0, 0xc0 + (r8_vals[r2:upper()] << 3) + r8_vals[r1:upper()] } end
            },
            {
                { "m8", "r8" },
                function (m, r) return { 0, 6 + (r8_vals[r:upper()] << 3), uint16l(m[1]) } end
            },
            {
                { "r16", "r16" },
                function (r1, r2) return { 1, 0xc0 + (r16_vals[r2:upper()] << 3) + r16_vals[r1:upper()] } end
            },
            {
                { "m16", "r16" },
                function (m, r) return { 1, 6 + (r16_vals[r:upper()] << 3), uint16l(m[1])} end
            },
            {
                { "r8", "m8" },
                function (r, m) return { 2, 6 + (r8_vals[r:upper()] << 3), uint16l(m[1]) } end
            },
            {
                { "r16", "m16" },
                function (r, m) return { 3, 6 + (r16_vals[r:upper()] << 3), uint16l(m[1]) } end
            },
            {
                { "ral", "i8" },
                function (_, i) return { 4, int8(i) } end -- FIX: ¿se pueden negativos?
            },
            {
                { "rax", "i16" },
                function (_, i) return { 5, int16l(i) } end -- idem
            },
            {
                { "r8", "i8" },
                function (r, i) return { 0x80, 0xc0 + r8_vals[r:upper()], int8(i) } end
            },
            {
                { "sbyte", "m8", "i8" },
                function (_, m, i) return { 0x80, 6, uint16l(m[1]), int8(i) } end
            },
            { -- NOTE: esto debe ser ejemplo de que sí importa la prioridad
                { "r16", "i8" },
                function (r, i) return { 0x83, 0xc0 + r16_vals[r:upper()], int8(i) } end
            },
            {
                { "sword", "m16", "i8" },
                function (_, m, i) return { 0x83, 6, uint16l(m[1]), int8(i) } end -- FIX: definitivamente comprobar tamaño
            },
            {
                { "r16", "i16" },
                function (r, i) return { 0x81, 0xc0 + r16_vals[r:upper()], int16l(i) } end
            },
            {
                { "sword", "m16", "i16" },
                function (_, m, i) return { 0x81, 6, uint16l(m[1]), int16l(i) } end
            }
        },
        AND = {
            { { "r8", "r8" }, { "ADD", function (bytes) return add_tr(bytes, 0x20, 4) end } },
            { { "m8", "r8" }, { "ADD", function (bytes) return add_tr(bytes, 0x20, 4) end } },
            { { "r16", "r16" }, { "ADD", function (bytes) return add_tr(bytes, 0x20, 4) end } },
            { { "m16", "r16" }, { "ADD", function (bytes) return add_tr(bytes, 0x20, 4) end } },
            { { "r8", "m8" }, { "ADD", function (bytes) return add_tr(bytes, 0x20, 4) end } },
            { { "r16", "m16" }, { "ADD", function (bytes) return add_tr(bytes, 0x20, 4) end } },
            { { "ral", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x20, 4) end } },
            { { "rax", "i16" }, { "ADD", function (bytes) return add_tr(bytes, 0x20, 4) end } },
            { { "r8", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x20, 4) end } },
            { { "sbyte", "m8", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x20, 4) end } },
            { { "r16", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x20, 4) end } },
            { { "sword", "m16", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x20, 4) end } },
            { { "r16", "i16" }, { "ADD", function (bytes) return add_tr(bytes, 0x20, 4) end } },
            { { "sword", "m16", "i16" }, { "ADD", function (bytes) return add_tr(bytes, 0x20, 4) end } }
        },
        CALL = { -- FIX: revisar bien
            {
                { "d16" }, --near
                function (d) return { 0xe8, int16l(d) } end
            },
            { --FIX: pendiente
                { "d32" }, --far
                function (d) return { 0x9a, int16l(d) } end
            },
            {
                { "r16" },
                function (r) return { 0xff, 0xd0 + r16_vals[r] } end
            },
            {
                { "m16" },
                function (m) return { 0xff, 0x15, int16l(m[1]) } end
            },
            {
                { "sfar", "m16" },
                function (m) return { 0xff, 0x1d, int16l(m[1]) } end
            }
        },
        CBW = { 0x98 },
        CLC = { 0xf8 },
        CLD = { 0xfc },
        CLI = { 0xfa },
        CMC = { 0xf5 },
        CMP = {
            { { "r8", "r8" }, { "ADD", function (bytes) return add_tr(bytes, 0x38, 7) end } },
            { { "m8", "r8" }, { "ADD", function (bytes) return add_tr(bytes, 0x38, 7) end } },
            { { "r16", "r16" }, { "ADD", function (bytes) return add_tr(bytes, 0x38, 7) end } },
            { { "m16", "r16" }, { "ADD", function (bytes) return add_tr(bytes, 0x38, 7) end } },
            { { "r8", "m8" }, { "ADD", function (bytes) return add_tr(bytes, 0x38, 7) end } },
            { { "r16", "m16" }, { "ADD", function (bytes) return add_tr(bytes, 0x38, 7) end } },
            { { "ral", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x38, 7) end } },
            { { "rax", "i16" }, { "ADD", function (bytes) return add_tr(bytes, 0x38, 7) end } },
            { { "r8", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x38, 7) end } },
            { { "sbyte", "m8", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x38, 7) end } },
            { { "r16", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x38, 7) end } },
            { { "sword", "m16", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x38, 7) end } },
            { { "r16", "i16" }, { "ADD", function (bytes) return add_tr(bytes, 0x38, 7) end } },
            { { "sword", "m16", "i16" }, { "ADD", function (bytes) return add_tr(bytes, 0x38, 7) end } }
        },
        CMPSB = { 0xa6 },
        CMPSW = { 0xa7 },
        CWD = { 0x99 },
        DAA = { 0x27 },
        DAS = { 0x2f },
        DEC = {
            {
                { "r8" },
                function (r) return { 0xfe, 0xc8 + r8_vals[r] } end
            },
            {
                { "sbyte", "m8" },
                function (_, m) return { 0xfe, 0x0d, uint16(m[1]) } end
            },
            {
                { "r16" },
                function (r) return { 0x48 + r16_vals[r] } end
            },
            {
                { "sword", "m16" },
                function (_, m) return { 0xff, 0x0d, uint16(m[1]) } end
            }
        },
        DIV = {
            { { "r8" }, { "NOT", function (bytes) return borrar_sumar2_tr(bytes, 6) end } },
            { { "sbyte", "m8" }, { "NOT", function (bytes) return borrar_sumar2_tr(bytes, 6) end } },
            { { "r16"}, { "NOT", function (bytes) return borrar_sumar2_tr(bytes, 6) end } },
            { { "sword", "m16" }, { "NOT", function (bytes) return borrar_sumar2_tr(bytes, 6) end } }
        },
        HLT = { 0xf4 },
        IDIV = {
            { { "r8" }, { "NOT", function (bytes) return borrar_sumar2_tr(bytes, 7) end } },
            { { "sbyte", "m8" }, { "NOT", function (bytes) return borrar_sumar2_tr(bytes, 7) end } },
            { { "r16"}, { "NOT", function (bytes) return borrar_sumar2_tr(bytes, 7) end } },
            { { "sword", "m16" }, { "NOT", function (bytes) return borrar_sumar2_tr(bytes, 7) end } }
        },
        IMUL = {
            { { "r8" }, { "NOT", function (bytes) return borrar_sumar2_tr(bytes, 5) end } },
            { { "sbyte", "m8" }, { "NOT", function (bytes) return borrar_sumar2_tr(bytes, 5) end } },
            { { "r16"}, { "NOT", function (bytes) return borrar_sumar2_tr(bytes, 5) end } },
            { { "sword", "m16" }, { "NOT", function (bytes) return borrar_sumar2_tr(bytes, 5) end } }
        },
        IN = {
            {
                { "ra", "i8" },
                function (a, i) return { 0xe4 + a_vals[a], i } end
            },
            {
                { "ra", "ddx" },
                function (i, _) return { 0xec + a_vals[a] } end
            },
        },
        INC = {
            {
                { "r8" },
                function (r) return { 0xfe, 0xc0 + r8_vals[r] } end
            },
            {
                { "sbyte", "m8" },
                function (_, m) return { 0xfe, 5, uint16(m[1]) } end
            },
            {
                { "r16" },
                function (r) return { 0x40 + r16_vals[r] } end
            },
            {
                { "sword", "m16" },
                function (_, m) return { 0xff, 5, uint16(m[1]) } end
            }
        },
        INT = {
            {
                { "3" },
                { 0xcc }
            },
            {
                { "i8" },
                function (i) return { 0xcd, i } end
            }
        },
        INTO = { 0xce },
        IRET = { 0xcf },
        -- Saltos condicionales
        JO = { { { "d8" }, function (d) return { 0x70, d } end } },
        JNO = { { { "d8" }, function (d) return { 0x71, d } end } },
        JB = { { { "d8" }, function (d) return { 0x72, d } end } },
        JC = { { { "d8" }, function (d) return { 0x72, d } end } },
        JNAE = { { { "d8" }, function (d) return { 0x72, d } end } },
        JAE = { { { "d8" }, function (d) return { 0x73, d } end } },
        JNB = { { { "d8" }, function (d) return { 0x73, d } end } },
        JNC = { { { "d8" }, function (d) return { 0x73, d } end } },
        JE = { { { "d8" }, function (d) return { 0x74, d } end } },
        JZ = { { { "d8" }, function (d) return { 0x74, d } end } },
        JNE = { { { "d8" }, function (d) return { 0x75, d } end } },
        JNZ = { { { "d8" }, function (d) return { 0x75, d } end } },
        JBE = { { { "d8" }, function (d) return { 0x76, d } end } },
        JNA = { { { "d8" }, function (d) return { 0x76, d } end } },
        JA = { { { "d8" }, function (d) return { 0x77, d } end } },
        JNBE = { { { "d8" }, function (d) return { 0x77, d } end } },
        JS = { { { "d8" }, function (d) return { 0x78, d } end } },
        JNS = { { { "d8" }, function (d) return { 0x79, d } end } },
        JP = { { { "d8" }, function (d) return { 0x7a, d } end } },
        JPE = { { { "d8" }, function (d) return { 0x7a, d } end } },
        JNP = { { { "d8" }, function (d) return { 0x7b, d } end } },
        JPO = { { { "d8" }, function (d) return { 0x7b, d } end } },
        JL = { { { "d8" }, function (d) return { 0x7c, d } end } },
        JNGE = { { { "d8" }, function (d) return { 0x7c, d } end } },
        JGE = { { { "d8" }, function (d) return { 0x7d, d } end } },
        JNL = { { { "d8" }, function (d) return { 0x7d, d } end } },
        JLE = { { { "d8" }, function (d) return { 0x7e, d } end } },
        JNG = { { { "d8" }, function (d) return { 0x7e, d } end } },
        JG = { { { "d8" }, function (d) return { 0x7f, d } end } },
        JNLE = { { { "d8" }, function (d) return { 0x7f, d } end } },
        --
        JCXZ = { 0xe3 },
        JMP = {
	   --Short	11101011
	   --Near	11101001
	   --Far	11101010
	   --RegWord	11111111oo100mmm
	   --MemNear	11111111oo100mmm
	   --MemFar	11111111oo101mmm
        },
        LAHF = { 0x9f },
        LDS = {
            {
                { "r16", "m32" },
                { 0xc5 }
            }
        },
        LEA = {
            {
                { "r16", "mem" },
                { 0x8d }
            }
        },
        LES = {
            {
                { "r16", "m32" },
                { 0xc4 }
            }
        },
        --LOCK = { 0xf0 }, --es un prefijo
        LODSB = { 0xac },
        LODSW = { 0xad },
        LOOP = { -- short es un numero - $pc
            {
                { "short" },
        --//        { 0xe2, short }
            }
        },
        LOOPE = {
            {
                { "short" },
        --//        { 0xe1, short }
            }
        },
        LOOPNE = {
            {
                { "short" },
        --//        { 0xe0, short }
            }
        },
        LOOPNZ = {
            {
                { "short" },
        --//        { 0xe0, short }
            }
        },
        LOOPZ = {
            {
                { "short" },
        --//        { 0xe1, short }
            }
        },
        MOV = {
            {
                { "r8", "r8" },
                function (r1, r2) return { 0x88, 0xc0 + (r8_vals[r2] << 3) + r8_vals[r1] } end
            },
            {
                { "m8", "r8" },
                function (m, r) return { 0x88, 0x5 + (r8_vals[r] << 3), uint16l(m[1]) } end
            },
            {
                { "r16", "r16" },
                function (r1, r2) return { 0x89, 0xc0 + (r8_vals[r2] << 3) + r8_vals[r1] } end
            },
            {
                { "m16", "r16" },
                function (m, r) return { 0x89, 0x5 + (r8_vals[r] << 3), uint16l(m[1]) } end
            },
            {
                { "ral", "m8" },
                function (_, m) return { 0xa0, uint16l(m[1]) } end
            },
            {
                { "rax", "m16" },
                function (_, m) return{ 0xa1, uint16l(m[1]) } end
            },
            {
                { "m8", "ral" },
                function (m, _) return { 0xa2, uint16l(m[1]) } end
            },
            {
                { "m16", "rax" },
                function (m, _) return { 0xa3, uint16l(m[1]) } end
            },
            {
                { "r8", "i8" },
                function (r, i) return { 0xb0 + r8_vals[r], i } end
            },
            {
                { "r16", "i16" },
                function (r, i) return { 0xb8 + r16_vals[r], sint16l(l) } end -- FIX: revisar signo
            },
            {
                { "sbyte", "m8", "i8" },
                function (_, m, i) return { 0xc6, 5, uint16l(m[1]), i } end
            },
            {
                { "sword", "m16", "i16" },
                function (_, m, i) return { 0xc7, 5, uint16l(m[1]), sint16l(i) } end
            },
            {
                { "r16", "s" },
                function (r, s) return { 0x8c, 0x0c + (s_vals[s] << 3) +  r16_vals[r] } end
            },
            {
                { "m16", "s" },
                function (m, s) return { 0x8c, 5 + (s_vals[s] << 3), uint16l(m[1]) } end
            },
            {
                { "s", "r16" },
                function (s, r) return { 0x8e, 0x0c + (s_vals[s] << 3) + r16_vals[r] } end
            },
            {
                { "s", "m16" },
                function (s, m) return { 0x8e, 5 + (s_vals[s] << 3), uint16l(m[1]) } end
            }
        },
        MOVSB = { 0xa4 },
        MOVSW = { 0xa5 },
        MUL = {
            { { "r8" }, { "NOT", function (bytes) return borrar_sumar2_tr(bytes, 4) end } },
            { { "sbyte", "m8" }, { "NOT", function (bytes) return borrar_sumar2_tr(bytes, 4) end } },
            { { "r16"}, { "NOT", function (bytes) return borrar_sumar2_tr(bytes, 4) end } },
            { { "sword", "m16" }, { "NOT", function (bytes) return borrar_sumar2_tr(bytes, 4) end } }
        },
        NEG = {
            { { "r8" }, { "NOT", function (bytes) return borrar_sumar2_tr(bytes, 3) end } },
            { { "sbyte", "m8" }, { "NOT", function (bytes) return borrar_sumar2_tr(bytes, 3) end } },
            { { "r16"}, { "NOT", function (bytes) return borrar_sumar2_tr(bytes, 3) end } },
            { { "sword", "m16" }, { "NOT", function (bytes) return borrar_sumar2_tr(bytes, 3) end } }
        },
        NOP = { 0x90 },
        NOT = {
            {
                { "r8" },
                function (r) return { 0xf6, 0xd0 + r_vals[r] } end
            },
            {
                { "sbyte", "m8" },
                function (_, m) return { 0xf6, 0x15, uint16l(m[1]) } end
            },
            {
                { "r16"},
                function (r) return { 0xf7, 0xd0 + r_vals[r] } end
            },
            {
                { "sword", "m16" },
                function (_, m) return { 0xf7, 0x15, uint16l(m[1]) } end
            }
        },
        OR = {
            { { "r8", "r8" }, { "ADD", function (bytes) return add_tr(bytes, 8, 1) end } },
            { { "m8", "r8" }, { "ADD", function (bytes) return add_tr(bytes, 8, 1) end } },
            { { "r16", "r16" }, { "ADD", function (bytes) return add_tr(bytes, 8, 1) end } },
            { { "m16", "r16" }, { "ADD", function (bytes) return add_tr(bytes, 8, 1) end } },
            { { "r8", "m8" }, { "ADD", function (bytes) return add_tr(bytes, 8, 1) end } },
            { { "r16", "m16" }, { "ADD", function (bytes) return add_tr(bytes, 8, 1) end } },
            { { "ral", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 8, 1) end } },
            { { "rax", "i16" }, { "ADD", function (bytes) return add_tr(bytes, 8, 1) end } },
            { { "r8", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 8, 1) end } },
            { { "sbyte", "m8", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 8, 1) end } },
            { { "r16", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 8, 1) end } },
            { { "sword", "m16", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 8, 1) end } },
            { { "r16", "i16" }, { "ADD", function (bytes) return add_tr(bytes, 8, 1) end } },
            { { "sword", "m16", "i16" }, { "ADD", function (bytes) return add_tr(bytes, 8, 1) end } }
        },
        OUT = {
            {
                { "i8", "ra" },
                function (i, a) return { 0xe6 + a_vals[a], i } end
            },
            {
                { "ddx", "ra" },
                function (_, a) return { 0xee + a_vals[a] } end
            },
        },
        POP = {
            {
                { "m16" },
                function (m) return { 0x8f, 5, uint16(m[1]) } end
            },
            {
                { "r16" },
                function (r) return { 0x58 + r16_vals[r] } end
            },
            {
                { "s" },
                function (s) return { 0x07 + (s_vals[s] << 3) } end
            }
        },
        POPF = { 0x9d },
        PUSH = {
            {
                { "m16" },
                function (m) return { 0xff, 0x35, uint16(m[1]) } end
            },
            {
                { "r16" },
                function (r) return { 0x50 + r16_vals[r] } end
            },
            {
                { "s" },
                function (s) return { 0x06 + (s_vals[s] << 3) } end
            }
        },
        PUSHF = { 0x9c },
        RCL = {
            { { "r8", "1" }, { "ROL", function (bytes) return  sumar2_tr(bytes, 2) end } },
            { { "r16", "1" }, { "ROL", function (bytes) return sumar2_tr(bytes, 2) end } },
            { { "r8", "rcl" }, { "ROL", function (bytes) return sumar2_tr(bytes, 2) end } },
            { { "r16", "rcl" }, { "ROL", function (bytes) return sumar2_tr(bytes, 2) end } },
            { { "sbyte", "m8", "1" }, { "ROL", function (bytes) return sumar2_tr(bytes, 2) end } },
            { { "sword", "m16", "1" }, { "ROL", function (bytes) return sumar2_tr(bytes, 2) end } },
            { { "sbyte", "m8", "rcl" }, { "ROL", function (bytes) return sumar2_tr(bytes, 2) end } },
            { { "sword", "m16", "rcl" }, { "ROL", function (bytes) return sumar2_tr(bytes, 2) end } }
        },
        RCR = {
            { { "r8", "1" }, { "ROL", function (bytes) return  sumar2_tr(bytes, 3) end } },
            { { "r16", "1" }, { "ROL", function (bytes) return sumar2_tr(bytes, 3) end } },
            { { "r8", "rcl" }, { "ROL", function (bytes) return sumar2_tr(bytes, 3) end } },
            { { "r16", "rcl" }, { "ROL", function (bytes) return sumar2_tr(bytes, 3) end } },
            { { "sbyte", "m8", "1" }, { "ROL", function (bytes) return sumar2_tr(bytes, 3) end } },
            { { "sword", "m16", "1" }, { "ROL", function (bytes) return sumar2_tr(bytes, 3) end } },
            { { "sbyte", "m8", "rcl" }, { "ROL", function (bytes) return sumar2_tr(bytes, 3) end } },
            { { "sword", "m16", "rcl" }, { "ROL", function (bytes) return sumar2_tr(bytes, 3) end } }
        },
        ----Revisar?
        REP = { 0xf3 },
        REPE = { 0xf3 },
        REPNE = { 0xf2 },
        REPNZ = { 0xf2 },
        REPZ = { 0xf3 },
        ----
        RET = {
            {
                {},
                { 0xc3 }
            },
            {
                { "i16" },
                function (i) return { 0xc2, uint16(i) } end
            }
        },
        RETF = {
            {
                {},
                { 0xcb }
            },
            {
                { "i16" },
                function (i) return { 0xca, uint16(i) } end
            }
        },
        ROL = {
            {
                { "r8", "1" },
                function (r, _) return { 0xd0, 0xe0 + r8_vals[r] } end
            },
            {
                { "r16", "1" },
                function (r, _) return { 0xd1, 0xe0 + r16_vals[r] } end
            },
            {
                { "r8", "rcl" },
                function (r, _) return { 0xd2, 0xe0 + r8_vals[r] } end
            },
            {
                { "r16", "rcl" },
                function (r, _) return { 0xd3, 0xe0 + r16_vals[r] } end
            },
            {
                { "sbyte", "m8", "1" },
                function (_, m, __) return { 0xd0, 6, uint16(m[1]) } end
            },
            {
                { "sword", "m16", "1" },
                function (_, m, __) return { 0xd1, 6, uint16(m[1]) } end
            },
            {
                { "sbyte", "m8", "rcl" },
                function (_, m, __) return { 0xd2, 6, uint16(m[1]) } end
            },
            {
                { "sword", "m16", "rcl" },
                function (_, m, __) return { 0xd3, 6, uint16(m[1]) } end
            }
        },
        ROR = {
            { { "r8", "1" }, { "ROL", function (bytes) return sumar2_tr(bytes, 1) end } },
            { { "r16", "1" }, { "ROL", function (bytes) return sumar2_tr(bytes, 1) end } },
            { { "r8", "rcl" }, { "ROL", function (bytes) return sumar2_tr(bytes, 1) end } },
            { { "r16", "rcl" }, { "ROL", function (bytes) return sumar2_tr(bytes, 1) end } },
            { { "sbyte", "m8", "1" }, { "ROL", function (bytes) return sumar2_tr(bytes, 1) end } },
            { { "sword", "m16", "1" }, { "ROL", function (bytes) return sumar2_tr(bytes, 1) end } },
            { { "sbyte", "m8", "rcl" }, { "ROL", function (bytes) return sumar2_tr(bytes, 1) end } },
            { { "sword", "m16", "rcl" }, { "ROL", function (bytes) return sumar2_tr(bytes, 1) end } }
        },
        SAHF = { 0x9e },
        SAL = {
            { { "r8", "1" }, { "ROL", function (bytes) return sumar2_tr(bytes, 4) end } },
            { { "r16", "1" }, { "ROL", function (bytes) return sumar2_tr(bytes, 4) end } },
            { { "r8", "rcl" }, { "ROL", function (bytes) return sumar2_tr(bytes, 4) end } },
            { { "r16", "rcl" }, { "ROL", function (bytes) return sumar2_tr(bytes, 4) end } },
            { { "sbyte", "m8", "1" }, { "ROL", function (bytes) return sumar2_tr(bytes, 4) end } },
            { { "sword", "m16", "1" }, { "ROL", function (bytes) return sumar2_tr(bytes, 4) end } },
            { { "sbyte", "m8", "rcl" }, { "ROL", function (bytes) return sumar2_tr(bytes, 4) end } },
            { { "sword", "m16", "rcl" }, { "ROL", function (bytes) return sumar2_tr(bytes, 4) end } }
        },
        SAR = {
            { { "r8", "1" }, { "ROL", function (bytes) return sumar2_tr(bytes, 7) end } },
            { { "r16", "1" }, { "ROL", function (bytes) return sumar2_tr(bytes, 7) end } },
            { { "r8", "rcl" }, { "ROL", function (bytes) return sumar2_tr(bytes, 7) end } },
            { { "r16", "rcl" }, { "ROL", function (bytes) return sumar2_tr(bytes, 7) end } },
            { { "sbyte", "m8", "1" }, { "ROL", function (bytes) return sumar2_tr(bytes, 7) end } },
            { { "sword", "m16", "1" }, { "ROL", function (bytes) return sumar2_tr(bytes, 7) end } },
            { { "sbyte", "m8", "rcl" }, { "ROL", function (bytes) return sumar2_tr(bytes, 7) end } },
            { { "sword", "m16", "rcl" }, { "ROL", function (bytes) return sumar2_tr(bytes, 7) end } }
        },
        SBB = {
            { { "r8", "r8" }, { "ADD", function (bytes) return add_tr(bytes, 0x18, 3) end } },
            { { "m8", "r8" }, { "ADD", function (bytes) return add_tr(bytes, 0x18, 3) end } },
            { { "r16", "r16" }, { "ADD", function (bytes) return add_tr(bytes, 0x18, 3) end } },
            { { "m16", "r16" }, { "ADD", function (bytes) return add_tr(bytes, 0x18, 3) end } },
            { { "r8", "m8" }, { "ADD", function (bytes) return add_tr(bytes, 0x18, 3) end } },
            { { "r16", "m16" }, { "ADD", function (bytes) return add_tr(bytes, 0x18, 3) end } },
            { { "ral", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x18, 3) end } },
            { { "rax", "i16" }, { "ADD", function (bytes) return add_tr(bytes, 0x18, 3) end } },
            { { "r8", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x18, 3) end } },
            { { "sbyte", "m8", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x18, 3) end } },
            { { "r16", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x18, 3) end } },
            { { "sword", "m16", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x18, 3) end } },
            { { "r16", "i16" }, { "ADD", function (bytes) return add_tr(bytes, 0x18, 3) end } },
            { { "sword", "m16", "i16" }, { "ADD", function (bytes) return add_tr(bytes, 0x18, 3) end } }
        },
        SCASB = { 0xae },
        SCASW = { 0xaf },
        SHL = {
            { { "r8", "1" }, { "SAL", function (bytes) return bytes end } },
            { { "r16", "1" }, { "SAL", function (bytes) return bytes end } },
            { { "r8", "rcl" }, { "SAL", function (bytes) return bytes end } },
            { { "r16", "rcl" }, { "SAL", function (bytes) return bytes end } },
            { { "sbyte", "m8", "1" }, { "SAL", function (bytes) return bytes end } },
            { { "sword", "m16", "1" }, { "SAL", function (bytes) return bytes end } },
            { { "sbyte", "m8", "rcl" }, { "SAL", function (bytes) return bytes end } },
            { { "sword", "m16", "rcl" }, { "SAL", function (bytes) return bytes end } }
        },
        SHR = {
            { { "r8", "1" }, { "ROL", function (bytes) return sumar2_tr(bytes, 5) end } },
            { { "r16", "1" }, { "ROL", function (bytes) return sumar2_tr(bytes, 5) end } },
            { { "r8", "rcl" }, { "ROL", function (bytes) return sumar2_tr(bytes, 5) end } },
            { { "r16", "rcl" }, { "ROL", function (bytes) return sumar2_tr(bytes, 5) end } },
            { { "sbyte", "m8", "1" }, { "ROL", function (bytes) return sumar2_tr(bytes, 5) end } },
            { { "sword", "m16", "1" }, { "ROL", function (bytes) return sumar2_tr(bytes, 5) end } },
            { { "sbyte", "m8", "rcl" }, { "ROL", function (bytes) return sumar2_tr(bytes, 5) end } },
            { { "sword", "m16", "rcl" }, { "ROL", function (bytes) return sumar2_tr(bytes, 5) end } }
        },
        STC = { 0xf9 },
        STD = { 0xfd },
        STI = { 0xfb },
        STOSB = { 0xaa },
        STOSW = { 0xab },
        SUB = {
            { { "r8", "r8" }, { "ADD", function (bytes) return add_tr(bytes, 0x28, 5) end } },
            { { "m8", "r8" }, { "ADD", function (bytes) return add_tr(bytes, 0x28, 5) end } },
            { { "r16", "r16" }, { "ADD", function (bytes) return add_tr(bytes, 0x28, 5) end } },
            { { "m16", "r16" }, { "ADD", function (bytes) return add_tr(bytes, 0x28, 5) end } },
            { { "r8", "m8" }, { "ADD", function (bytes) return add_tr(bytes, 0x28, 5) end } },
            { { "r16", "m16" }, { "ADD", function (bytes) return add_tr(bytes, 0x28, 5) end } },
            { { "ral", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x28, 5) end } },
            { { "rax", "i16" }, { "ADD", function (bytes) return add_tr(bytes, 0x28, 5) end } },
            { { "r8", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x28, 5) end } },
            { { "sbyte", "m8", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x28, 5) end } },
            { { "r16", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x28, 5) end } },
            { { "sword", "m16", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x28, 5) end } },
            { { "r16", "i16" }, { "ADD", function (bytes) return add_tr(bytes, 0x28, 5) end } },
            { { "sword", "m16", "i16" }, { "ADD", function (bytes) return add_tr(bytes, 0x28, 5) end } }
        },
        TEST = {
            --Reg,Reg 	0x84+w oorrrmmm
            --Mem,Reg 	0x84+w oorrrmmm
            --Reg,Mem 	0x84+w oorrrmmm
            --Acc,Imm 	0xa8+w
            --Reg,Imm 	0xf6+w oo000mmm
            --Mem,Imm 	0xf6+w oo000mmm
        },
        WAIT = { 0x9b },
        XCHG = {
            --AccWord,RegWord 	0x90+rrr
            --RegWord,AccWord 	0x90+rrr
            --Reg,Reg 	0x86+w oorrrmmm
            --Mem,Reg 	0x86+w oorrrmmm
            --Reg,Mem 	0x86+w oorrrmmm
        },
        XLAT = { 0xd7 },
        XOR = {
            { { "r8", "r8" }, { "ADD", function (bytes) return add_tr(bytes, 0x30, 6) end } },
            { { "m8", "r8" }, { "ADD", function (bytes) return add_tr(bytes, 0x30, 6) end } },
            { { "r16", "r16" }, { "ADD", function (bytes) return add_tr(bytes, 0x30, 6) end } },
            { { "m16", "r16" }, { "ADD", function (bytes) return add_tr(bytes, 0x30, 6) end } },
            { { "r8", "m8" }, { "ADD", function (bytes) return add_tr(bytes, 0x30, 6) end } },
            { { "r16", "m16" }, { "ADD", function (bytes) return add_tr(bytes, 0x30, 6) end } },
            { { "ral", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x30, 6) end } },
            { { "rax", "i16" }, { "ADD", function (bytes) return add_tr(bytes, 0x30, 6) end } },
            { { "r8", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x30, 6) end } },
            { { "sbyte", "m8", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x30, 6) end } },
            { { "r16", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x30, 6) end } },
            { { "sword", "m16", "i8" }, { "ADD", function (bytes) return add_tr(bytes, 0x30, 6) end } },
            { { "r16", "i16" }, { "ADD", function (bytes) return add_tr(bytes, 0x30, 6) end } },
            { { "sword", "m16", "i16" }, { "ADD", function (bytes) return add_tr(bytes, 0x30, 6) end } }
        },
	-- ver qué es este asunto
        --ES: 	0x26
        --CS: 	0x2e
        --SS: 	0x36
        --DS: 	0x3e
    },
    opcodes = {
    },
    desensamble = {
    }
}
