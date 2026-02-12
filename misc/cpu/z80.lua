-- Especificación del microprocesador Zilog Z80

--[[
  Habrá disponible un objeto global para controlar la plataforma, L
  L.leer_memoria(pos)
  L.escribir_memoria(pos, v)
  L.leer_registro(reg)
  L.escribir_registro(reg, v)
]]

-- Correspondencias valor->bits
r_vals = {
    { 0, 'B' },
    { 1, 'C' },
    { 2, 'D' },
    { 3, 'E' },
    { 4, 'H' },
    { 5, 'L' },
    { 7, 'A' }
}
dd_vals = {
    { 0, 'BC' },
    { 1, 'DE' },
    { 2, 'HL' },
    { 3, 'SP' },
}

--
return {
    id = "zilog.z80",
    registros = {
        -- Registros principales
        A = {
            tamano = 8,
            desplazamiento = 0
        },
        F = {
            tamano = 8,
            desplazamiento = 8
        },
        AF = {
            tamano = 16,
            desplazamiento = 0
        },
        B = {
            tamano = 8,
            desplazamiento = 16
        },
        C = {
            tamano = 8,
            desplazamiento = 24
        },
        BC = {
            tamano = 16,
            desplazamiento = 16
        },
        D = {
            tamano = 8,
            desplazamiento = 32
        },
        E = {
            tamano = 8,
            desplazamiento = 40
        },
        DE = {
            tamano = 16,
            desplazamiento = 32
        },
        H = {
            tamano = 8,
            desplazamiento = 48
        },
        L = {
            tamano = 8,
            desplazamiento = 56
        },
        HL = {
            tamano = 16,
            desplazamiento = 48
        },
        -- Registros de propósito especial
        IX = {
            tamano = 16,
            desplazamiento = 64
        },
        IY = {
            tamano = 16,
            desplazamiento = 80
        },
        PC = {
            tamano = 16,
            desplazamiento = 96
        },
        SP = {
            tamano = 16,
            desplazamiento = 108
        },
        I = {
            tamano = 8,
            desplazamiento = 116
        },
        R = {
            tamano = 8,
            desplazamiento = 124
        },
        -- Registros alternativos principales
        AFX = {
            tamano = 16,
            desplazamiento = 132
        },
        XX = {
            tamano = 64,
            desplazamiento = 132
        },
        -- Banderas
        CF = {
            tamano = 1,
            desplazamiento = 8
        },
        NF = {
            tamano = 1,
            desplazamiento = 9
        },
        VF = {
            tamano = 1,
            desplazamiento = 10
        },
        HF = {
            tamano = 1,
            desplazamiento = 12
        },
        ZF = {
            tamano = 1,
            desplazamiento = 14
        },
        SF = {
            tamano = 1,
            desplazamiento = 15
        },
        -- Registros de uso interno
        IM = {
            tamano = 8,
            desplazamiento = 196
        }
    },
    operandos = {},
    ensamble = {
        LD = {
            -- Grupo de carga de 8 bits
            {
                { "r", "r" },
                function (r1, r2) return 64 + (r_vals[r1] << 3) + r_vals[r2] end
            },
            {
                { "r", "n" },
                function (r, n) return { 6 + (r_vals[r] << 3), n } end
            },
            {
                { "r", "dhl" },
                function (r) return 70 + (r_vals[r] << 3) end
            },
            {
                { "r", "dix" },
                function (r, d) return { 0xDD, 70 + (r_vals[r] << 3), d } end
            },
            {
                { "r", "diy" },
                function (r, d) return { 0xFD, 70 + (r_vals[r] << 3), d } end
            },
            {
                { "dhl", "r" },
                function (r) return 112 + r_vals[r] end
            },
            {
                { "dix", "r" },
                function (d, r) return { 0xDD, 112 + r_vals[r], d } end
            },
            {
                { "diy", "r" },
                function (d, r) return { 0xFD, 112 + r_vals[r], d } end
            },
            {
                { "dhl", "n" },
                function (n) return { 0x36, n } end
            },
            {
                { "dix", "n" },
                function (d, n) return { 0xDD, 0x36, d, n } end
            },
            {
                { "diy", "n" },
                function (d, n) return { 0xFD, 0x36, d, n } end
            },
            {
                { "ra", "dbc" },
                function () return 0x0A end
            },
            {
                { "ra", "dde" },
                function () return 0x1A end
            },
            {
                { "ra", "dnn" },
                function (nn) return { 0x3A, nn & 0xFF, (nn >> 8) & 0xFF } end
            },
            {
                { "dbc", "ra" },
                function () return 0x02 end
            },
            {
                { "dde", "ra" },
                function () return 0x12 end
            },
            {
                { "dnn", "ra" },
                function (nn) return { 0x32, nn & 0xFF, (nn >> 8) & 0xFF } end
            },
            {
                { "ra", "ri" },
                function () return { 0xED, 0x57 } end
            },
            {
                { "ra", "rr" },
                function () return { 0xED, 0x5F } end
            },
            {
                { "ri", "ra" },
                function () return { 0xED, 0x47 } end
            },
            {
                { "rr", "ra" },
                function () return { 0xED, 0x4F } end
            },
            {
                { "dd", "nn" },
                function (dd, nn) return { 1 + (dd_vals[dd] << 4), nn & 0xFF, (nn >> 8) & 0xFF } end
            },
            {
                { "rix", "nn" },
                function (nn) return { 0xDD, 0x21, nn & 0xFF, (nn >> 8) & 0xFF } end
            },
            {
                { "riy", "nn" },
                function (nn) return { 0xFD, 0x21, nn & 0xFF, (nn >> 8) & 0xFF } end
            },
            {
                { "rhl", "dnn" },
                function (nn) return { 0x2A, nn & 0xFF, (nn >> 8) & 0xFF } end
            },
            {
                { "dd", "dnn" },
                function (dd, nn) return { 0xED, 75 + (dd_vals[dd] << 4), nn & 0xFF, (nn >> 8) & 0xFF } end
            },
            {
                { "rix", "dnn" },
                function (nn) return { 0xDD, 0x2A, nn & 0xFF, (nn >> 8) & 0xFF } end
            },
            {
                { "riy", "dnn" },
                function (nn) return { 0xFD, 0x2A, nn & 0xFF, (nn >> 8) & 0xFF } end
            },
            {
                { "dnn", "rhl" },
                function (nn) return { 0x22, nn & 0xFF, (nn >> 8) & 0xFF } end
            },
            {
                { "dnn", "dd" },
                function (nn, dd) return { 0xED, 67 + (dd_vals[dd] << 4), nn & 0xFF, (nn >> 8) & 0xFF } end
            },
            {
                { "dnn", "rix" },
                function (nn) return { 0xDD, 0x22, nn & 0xFF, (nn >> 8) & 0xFF } end
            },
            {
                { "dnn", "riy" },
                function (nn) return { 0xFD, 0x22, nn & 0xFF, (nn >> 8) & 0xFF } end
            },
            {
                { "rsp", "rhl" },
                function () return 0xF9 end
            },
            {
                { "rsp", "riy" },
                function () return { 0xFD, 0xF9 } end
            },
        },
        PUSH = {
            {
                { "qq" },
                function (qq)
                    local q_map = { bc = 0, de = 1, hl = 2, af = 3 }
                    return 0xC5 + (q_map[qq] << 4)
                end
            },
            {
                { "rix" },
                function () return { 0xDD, 0xE5 } end
            },
            {
                { "riy" },
                function () return { 0xFD, 0xE5 } end
            }
        },
        POP = {
            {
                { "qq" },
                function (qq)
                    local q_map = { bc = 0, de = 1, hl = 2, af = 3 }
                    return 0xC1 + (q_map[qq] << 4)
                end
            },
            {
                { "rix" },
                function () return { 0xDD, 0xE1 } end
            },
            {
                { "riy" },
                function () return { 0xFD, 0xE1 } end
            }
        },
        -- Grupo de búsqueda, intercambio y transferencia
        EX = {
            {
                { 'rde', 'rhl' },
                { 0xeb }
            },
            {
                { 'raf', 'raf' },
                { 0x08 }
            },
            {
                { 'dsp', 'rhl' },
                { 0xe3 }
            },
            {
                { 'dsp', 'rix' },
                { 0xdd, 0xe3 }
            },
            {
                { 'dsp', 'riy' },
                { 0xfd, 0xe3 }
            }
        },
        EXX = { 0xd9 },
        LDI = { 0xed, 0xa0 },
        LDIR = { 0xed, 0xb0 },
        LDD = { 0xed, 0xa8 },
        LDDR = { 0xed, 0xb8 },
        CPI = { 0xed, 0xa1 },
        CPIR = { 0xed, 0xb1 },
        CPD = { 0xed, 0xa9 },
        CPDR = { 0xed, 0xb9 },
        ADD = {
                    {
                        { "r" },
                        function (r) return 0x80 + r_vals[r] end
                    },
                    {
                        { "n" },
                function (n) return { 0xC6, n } end
            },
            {
                { "m" },
                function () return 0x86 end
            },
            {
                { "dix" },
                function (d) return { 0xDD, 0x86, d } end
            },
            {
                { "diy" },
                function (d) return { 0xFD, 0x86, d } end
            },
            {
                { "rhl", "dd" },
                function (h, ss) return 0x09 + (dd_vals[ss] << 4) end
            },
            {
                { "rix", "pp" },
                function (ix, pp)
                    local p_map = { bc = 0, de = 1, ix = 2, sp = 3 }
                    return { 0xDD, 0x09 + (p_map[pp] << 4) }
                end
            },
            {
                { "riy", "rr" },
                function (iy, rr)
                    local r_map = { bc = 0, de = 1, iy = 2, sp = 3 }
                    return { 0xFD, 0x09 + (r_map[rr] << 4) }
                end
            }
        },
       ADC = {
            {
                { "r2" },
                function (r2) return 0x88 + r_vals[r2] end
            },
            {
                { "n" },
                function (n) return { 0xCE, n } end
            },
            {
                { "dix" },
                function (d) return { 0xDD, 0x8E, d } end
            },
            {
                { "diy" },
                function (d) return { 0xFD, 0x8E, d } end
            },
            {
                { "rhl", "ss" },
                function (ss) return { 0xED, 0x4A + (dd_vals[ss] << 4) } end
            }
        },
        SUB = {
            {
                { "r2" },
                function (r2) return 0x90 + r_vals[r2] end
            },
            {
                { "n" },
                function (n) return { 0xD6, n } end
            },
            {
                { "dix" },
                function (d) return { 0xDD, 0x96, d } end
            },
            {
                { "diy" },
                function (d) return { 0xFD, 0x96, d } end
            }
        },
        SBC = {
            {
                { "r2" },
                function (r2) return 0x98 + r_vals[r2] end
            },
            {
                { "n" },
                function (n) return { 0xDE, n } end
            },
            {
                { "dix" },
                function (d) return { 0xDD, 0x9E, d } end
            },
            {
                { "diy" },
                function (d) return { 0xFD, 0x9E, d } end
            },
            {
                { "rhl", "ss" },
                function (ss) return { 0xED, 0x42 + (dd_vals[ss] << 4) } end
            }
        },
        AND = {
            {
                { "r2" },
                function (r2) return 0xA0 + r_vals[r2] end
            },
            {
                { "n" },
                function (n) return { 0xE6, n } end
            },
            {
                { "dix" },
                function (d) return { 0xDD, 0xA6, d } end
            },
            {
                { "diy" },
                function (d) return { 0xFD, 0xA6, d } end
            }
        },
        OR = {
            {
                { "r2" },
                function (r2) return 0xB0 + r_vals[r2] end
            },
            {
                { "n" },
                function (n) return { 0xF6, n } end
            },
            {
                { "dix" },
                function (d) return { 0xDD, 0xB6, d } end
            },
            {
                { "diy" },
                function (d) return { 0xFD, 0xB6, d } end
            }
        },
        XOR = {
            {
                { "r2" },
                function (r2) return 0xA8 + r_vals[r2] end
            },
            {
                { "n" },
                function (n) return { 0xEE, n } end
            },
            {
                { "dix" },
                function (d) return { 0xDD, 0xA8, d } end
            },
            {
                { "diy" },
                function (d) return { 0xFD, 0xA8, d } end
            }
        },
        CP = {
            {
                { "r2" },
                function (r2) return 0xB8 + r_vals[r2] end
            },
            {
                { "n" },
                function (n) return { 0xFE, n } end
            },
            {
                { "dix" },
                function (d) return { 0xDD, 0xBE, d } end
            },
            {
                { "diy" },
                function (d) return { 0xFD, 0xBE, d } end
            }
        },
        INC = {
            {
                { "r2" },
                function (r) return 0x04 + (r_vals[r] << 3) end
            },
            {
                { "dix" },
                function (d) return { 0xDD, 0x34, d } end
            },
            {
                { "diy" },
                function (d) return { 0xFD, 0x34, d } end
            },
            {
                { "dd" },
                function (dd) return 0x03 + (dd_vals[dd] << 4) end
            }
        },
        DEC = {
            {
                { "r2" },
                function (r) return 0x05 + (r_vals[r] << 3) end
            },
            {
                { "dix" },
                function (d) return { 0xDD, 0x35, d } end
            },
            {
                { "diy" },
                function (d) return { 0xFD, 0x35, d } end
            },
            {
                { "dd" },
                function (dd) return 0x0B + (dd_vals[dd] << 4) end
            }
        },
        -- Grupo de propósito general y control de CPU
        DAA = { 0x27 },
        CPL = { 0x2f },
        NEG = { 0xed, 0x44 },
        CCF = { 0x3f },
        SCF = { 0x37},
        NOP = { 0x00 },
        HALT = { 0x76 },
        DI = { 0xf3 },
        EI = { 0xfb },
        IM = {
            { "im" },
            function (im)
                local v = 0
                if im == 1 then v = 2
                elseif im == 2 then v = 3 end
            return { 0xed, 70+v } end
        },
        RLC = {
            {
                { "r" },
                function (r) return { 0xCB, 0x00 + r_vals[r] } end
            },
            {
                { "dhl" },
                { 0xCB, 0x06 }
            },
            {
                { "dix" },
                function (d) return { 0xDD, 0xCB, d, 0x06 } end
            },
            {
                { "diy" },
                function (d) return { 0xFD, 0xCB, d, 0x06 } end
            },
        },
        RL = {
            {
                { "r" },
                function (r) return { 0xCB, 0x10 + r_vals[r] } end
            },
            {
                { "dhl" },
                { 0xCB, 0x16 }
            },
            {
                { "dix" },
                function (d) return { 0xDD, 0xCB, d, 0x16 } end
            },
            {
                { "diy" },
                function (d) return { 0xFD, 0xCB, d, 0x16 } end
            },
        },
        RRC = {
            {
                { "r" },
                function (r) return { 0xCB, 0x08 + r_vals[r] } end
            },
            {
                { "dhl" },
                { 0xCB, 0x0E }
            },
            {
                { "dix" },
                function (d) return { 0xDD, 0xCB, d, 0x0E } end
            },
            {
                { "diy" },
                function (d) return { 0xFD, 0xCB, d, 0x0E } end
            },
        },
        RR = {
            {
                { "r" },
                function (r) return { 0xCB, 0x18 + r_vals[r] } end
            },
            {
                { "dhl" },
                { 0xCB, 0x1E }
            },
            {
                { "dix" },
                function (d) return { 0xDD, 0xCB, d, 0x1E } end
            },
            {
                { "diy" },
                function (d) return { 0xFD, 0xCB, d, 0x1E } end
            },
        },
        SLA = {
            {
                { "r" },
                function (r) return { 0xCB, 0x20 + r_vals[r] } end
            },
            {
                { "dhl" },
                { 0xCB, 0x26 }
            },
            {
                { "dix" },
                function (d) return { 0xDD, 0xCB, d, 0x26 } end
            },
            {
                { "diy" },
                function (d) return { 0xFD, 0xCB, d, 0x26 } end
            },
        },
        SRA = {
            {
                { "r" },
                function (r) return { 0xCB, 0x28 + r_vals[r] } end
            },
            {
                { "dhl" },
                { 0xCB, 0x2E }
            },
            {
                { "dix" },
                function (d) return { 0xDD, 0xCB, d, 0x2E } end
            },
            {
                { "diy" },
                function (d) return { 0xFD, 0xCB, d, 0x2E } end
            },
        },
        SRL = {
            {
                { "r" },
                function (r) return { 0xCB, 0x38 + r_vals[r] } end
            },
            {
                { "dhl" },
                { 0xCB, 0x3E }
            },
            {
                { "dix" },
                function (d) return { 0xDD, 0xCB, d, 0x3E } end
            },
            {
                { "diy" },
                function (d) return { 0xFD, 0xCB, d, 0x3E } end
            },
        },
        RLD = { { {}, { 0xED, 0x6F } } },
        RRD = { { {}, { 0xED, 0x67 } } },
        -- Grupo de bits
        BIT = {
            {
                { "b", "r" },
                function (b, r) return { 0xCB, 0x40 + (b << 3) + r_vals[r] } end
            },
            {
                { "b", "dhl" },
                function (b) return { 0xCB, 0x46 + (b << 3) } end
            },
            {
                { "b", "dix" },
                function (b, d) return { 0xDD, 0xCB, d, 0x46 + (b << 3) } end
            },
            {
                { "b", "diy" },
                function (b, d) return { 0xFD, 0xCB, d, 0x46 + (b << 3) } end
            }
        },
        SET = {
            {
                { "b", "r" },
                function (b, r) return { 0xCB, 0xC0 + (b << 3) + r_vals[r] } end
            },
            {
                { "b", "dhl" },
                function (b) return { 0xCB, 0xC6 + (b << 3) } end
            },
            {
                { "b", "dix" },
                function (b, d) return { 0xDD, 0xCB, d, 0xC6 + (b << 3) } end
            },
            {
                { "b", "diy" },
                function (b, d) return { 0xFD, 0xCB, d, 0xC6 + (b << 3) } end
            }
        },
        RES = {
            {
                { "b", "r" },
                function (b, r) return { 0xCB, 0x80 + (b << 3) + r_vals[r] } end
            },
            {
                { "b", "dhl" },
                function (b) return { 0xCB, 0x86 + (b << 3) } end
            },
            {
                { "b", "dix" },
                function (b, d) return { 0xDD, 0xCB, d, 0x86 + (b << 3) } end
            },
            {
                { "b", "diy" },
                function (b, d) return { 0xFD, 0xCB, d, 0x86 + (b << 3) } end
            }
        },
        JP = {
            {
                { "nn" },
                function (nn) return { 0xC3, nn & 0xFF, (nn >> 8) & 0xFF } end
            },
            {
                { "cc", "nn" },
                function (cc, nn)
                    local cc_map = { nz = 0, z = 1, nc = 2, c = 3, po = 4, pe = 5, p = 6, m = 7 }
                    return { 0xC2 + (cc_map[cc] << 3), nn & 0xFF, (nn >> 8) & 0xFF }
                end
            },
            {
                { "dhl" },
                { 0xE9 }
            },
            {
                { "dix" },
                { 0xDD, 0xE9 }
            },
            {
                { "diy" },
                { 0xFD, 0xE9 }
            }
        },
        JR = {
            {
                { "e" },
                function (e) return { 0x18, e } end
            },
            {
                { "fnz", "e" },
                function (e) return { 0x20, e } end
            },
            {
                { "fz", "e" },
                function (e) return { 0x28, e } end
            },
            {
                { "fnc", "e" },
                function (e) return { 0x30, e } end
            },
            {
                { "fc", "e" },
                function (e) return { 0x38, e } end
            }
        },
        DJNZ = {
            {
                { "e" },
                function (e) return { 0x10, e } end
            }
        },
        -- Grupo de llamada y retorno
        CALL = {
            {
                { "nn" },
                function (nn) return { 0xC4, nn & 0xFF, (nn >> 8) & 0xFF } end
            },
            {
                { "cc", "nn" },
                function (cc, nn)
                    local cc_map = { nz = 0, z = 1, nc = 2, c = 3, po = 4, pe = 5, p = 6, m = 7 }
                    return { 0xC4 + (cc_map[cc] << 3), nn & 0xFF, (nn >> 8) & 0xFF }
                end
            }
        },
        RET = {
            {
                {},
                { 0xC9 }
            },
            {
                { "cc" },
                function (cc)
                    local cc_map = { nz = 0, z = 1, nc = 2, c = 3, po = 4, pe = 5, p = 6, m = 7 }
                    return 0xC0 + (cc_map[cc] << 3)
                end
            }
        },
        RETI = { 0xED, 0x4D },
        RETN = { 0xED, 0x45 },
        RST = {
            {
                { "p" },
                function (p)
                    -- p son los valores 00h, 08h, 10h, 18h, 20h, 28h, 30h, 38h
                    return 0xC7 + p
                end
            }
        },
        -- Grupo de E/S
        IN = {
            {
                { "ra", "dn" },
                function (n) return { 0xDB, n } end
            },
            {
                { "r", "dc" },
                function (r) return { 0xED, 0x40 + (r_vals[r] << 3) } end
            }
        },
        INI = { 0xED, 0xA2 },
        INIR = { 0xED, 0xB2 },
        IND = { 0xED, 0xAA },
        INDR = { 0xED, 0xBA },
       OUT = {
            {
                { "dn", "ra" },
                function (n) return { 0xD3, n } end
            },
            {
                { "dc", "r" },
                function (r) return { 0xED, 0x41 + (r_vals[r] << 3) } end
            }
        },
        OUTI = { 0xED, 0xA3 },
        OTIR = { 0xED, 0xB3 },
        OUTD = { 0xED, 0xAB },
        OTDR = { 0xED, 0xBB }
    },
    opcodes = {
        -- Grupo de carga de 8 bits
        -- Grupo de carga de 16 bits
        -- Grupo de búsqueda, intercambio y transferencia
        -- Grupo aritmético de 8 bits
        -- Grupo aritmético de 16 bits
        -- Grupo de propósito general y control de CPU
        {
            { 0x76 }, -- HALT
            function ()
                lily.detener()
            end
        },
        {
            { 0xed },
            {
                {
                    function (byte)
                        return byte == 0x46 or byte == 0x56 or byte == 0x5e
                    end,
                    function (...)
                        local args = {...}
                        lily.escribir_registro('mi', math.floor(args[2]/10))
                    end
                }
            }
        }

        -- Grupo de rotación y desplazamiento
        -- Grupo de bits
        -- Grupo de salto
        -- Grupo de llamada y retorno
        -- Grupo de E/S
    },
    desensamble = {
        -- Grupo de carga de 8 bits
        -- Grupo de carga de 16 bits
        -- Grupo de búsqueda, intercambio y transferencia
        -- Grupo aritmético de 8 bits
        -- Grupo aritmético de 16 bits
        -- Grupo de propósito general y control de CPU
        {
            { 0x76 },
            "HALT"
        },
        {
            { 0xed },
            {
                {
                    function (byte)
                        return byte == 0x46 or byte == 0x56 or byte == 0x5e
                    end,
                    function (...)
                        return "IM " .. math.floor(({...})[1]/10)-7
                    end
                }
            }
        }
        -- Grupo de rotación y desplazamiento
        -- Grupo de bits
        -- Grupo de salto
        -- Grupo de llamada y retorno
        -- Grupo de E/S
    }
}
